#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/errno.h>

#define DRIVER_NAME "qat_dev"
#define MY_PCIE_VENDOR_ID   0x8086
#define MY_PCIE_DEVICE_ID   0x4940

static int qat_probe(struct pci_dev *pdev, const struct pci_device_id *id);
static void qat_remove(struct pci_dev *pdev);

static struct pci_device_id qat_id_table[] = {
    {PCI_DEVICE(MY_PCIE_VENDOR_ID, MY_PCIE_DEVICE_ID)},
    { 0 }
};

// Tell kernel module loading program this driver support device listed in qat_id_table
MODULE_DEVICE_TABLE(pci, qat_id_table);

static struct pci_driver qat_driver = {
    .name = DRIVER_NAME,
    .id_table = qat_id_table,
    .probe = qat_probe,
    .remove = qat_remove,
};

static int qat_probe(struct pci_dev *pdev, const struct pci_device_id *id) {
    int err;

    printk(KERN_INFO "QAT device detected!\n");

    err = pci_enable_device(pdev);

    if (err) {
        printk(KERN_ERR "Failed to enable PCIe device.\n");
        return err;
    }

    return 0;
}

static void qat_remove(struct pci_dev *pdev) {
    printk(KERN_INFO "QAT device removed.\n");
    pci_disable_device(pdev);
}

static int __init qat_init(void) {
    int err;
    
    printk(KERN_INFO "Customized QAT driver loaded.\n");
    err = pci_register_driver(&qat_driver);
    if(err) {
        printk(KERN_ERR "Failed to register driver.\n");
        return err;
    }

    return 0;

}

static void __exit qat_exit(void) {
    printk(KERN_INFO "Customized QAT driver unloaded.\n");
    pci_unregister_driver(&qat_driver);
}

module_init(qat_init);
module_exit(qat_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Li Zixuan");
MODULE_DESCRIPTION("A simplified device driver for QAT");


