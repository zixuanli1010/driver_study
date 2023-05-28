#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/vfio.h>

#define QAT_DEVICE_ADDRESS "0000:e8:00.0"  // Replace with your QAT device's PCI address
#define QAT_BAR_INDEX 0  // Replace with the specific BAR index you want to retrieve information for

struct vfio_iommu_type1_dma_map dma_map = { .argsz = sizeof(dma_map) };
struct vfio_group_status group_status =
                { .argsz = sizeof(group_status) };
struct vfio_device_info device_info = 
                { .argsz = sizeof(device_info) };

int main() {

    int container, group, device, i;

    // // Unbind the QAT device from any existing drivers
    // snprintf(bind_cmd, sizeof(bind_cmd), "echo \"%s\" > /sys/bus/pci/devices/%s/driver/unbind", QAT_DEVICE_ADDRESS, QAT_DEVICE_ADDRESS);
    // system(bind_cmd);

    // // Bind the QAT device to the VFIO driver
    // snprintf(bind_cmd, sizeof(bind_cmd), "echo \"%s\" > /sys/bus/pci/drivers/vfio-pci/new_id", QAT_VENDOR_DEVICE_ID);
    // system(bind_cmd);

    // Create a new container 
    container = open("/dev/vfio/vfio", O_RDWR);

    if (ioctl(container, VFIO_GET_API_VERSION) != VFIO_API_VERSION){
        perror("Unknown API version");
        exit(1);
    }

    if (!ioctl(container, VFIO_CHECK_EXTENSION, VFIO_TYPE1_IOMMU)){
        perror("Doesn't support the IOMMU driver we want");
        exit(1);
    }

    group = open("/dev/vfio/44", O_RDWR);

    // Test the group is viable and available
    ioctl(group, VFIO_GROUP_GET_STATUS, &group_status);

    if (!(group_status.flags & VFIO_GROUP_FLAGS_VIABLE)){
        perror("Group is not viable (ie, not all devices bound for vfio)");
        exit(1);
    }

    // Add the group to the container
    ioctl(group, VFIO_GROUP_SET_CONTAINER, &container);

    /* Enable the IOMMU model we want */
    ioctl(container, VFIO_SET_IOMMU, VFIO_TYPE1_IOMMU);

    // Get a file descriptor for the device
    device = ioctl(group, VFIO_GROUP_GET_DEVICE_FD, QAT_DEVICE_ADDRESS);

    // Test and setup the device
    ioctl(device, VFIO_DEVICE_GET_INFO, &device_info);

    printf("QAT BAR number is %d \n", device_info.num_regions);

    for(i = 0; i < device_info.num_regions; i++){

        struct vfio_region_info reg = { .argsz = sizeof(reg) };
        
        reg.index = i;
        
        ioctl(device, VFIO_DEVICE_GET_REGION_INFO, &reg);

        /* skip non-mmapable BARs */
        if ((reg.flags & VFIO_REGION_INFO_FLAG_MMAP) == 0)
        {
            continue;
        }

        if (reg.flags & VFIO_REGION_INFO_FLAG_WRITE)
        {
            // Map the QAT BAR's virtual address to user space
            void *bar_addr = mmap(NULL, reg.size, PROT_READ | PROT_WRITE, MAP_SHARED, device, reg.offset);
            if (bar_addr != MAP_FAILED) {
                // Print the QAT BAR's virtual address
                printf("QAT BAR %d virtual address: %p\n", reg.index, bar_addr);
                munmap(bar_addr, reg.size);
            }

        }   
            
    }


    // Cleanup
    close(device);
    close(group);
    close(container);

    return 0;
}