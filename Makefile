# 告诉内核构建模块，要构建一个名为qat_driver的内核模块
obj-m += qat_driver.o

KDIR ?= /lib/modules/$(shell uname -r)/build

# -C 表示内核源码路径 M用于指定要构建的内核模块的源码目录
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules_install 