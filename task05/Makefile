MODULE_LIB := /lib/modules

KERNEL_PATH := $(MODULE_LIB)/$(shell uname -r)/build

obj-m += hotplug_study.o

all:
	make -C $(KERNEL_PATH) M=$(PWD) modules

clean:
	make -C $(KERNEL_PATH) M=$(PWD) clean
