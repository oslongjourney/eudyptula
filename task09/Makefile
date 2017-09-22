MODULE_LIB := /lib/modules

KERNEL_PATH := $(MODULE_LIB)/$(shell uname -r)/build

obj-m += eudyptula_sysfs.o

all:
	make -C $(KERNEL_PATH) M=$(PWD) modules

clean:
	make -C $(KERNEL_PATH) M=$(PWD) clean
