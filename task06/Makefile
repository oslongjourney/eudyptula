KERNEL_MODULE := /lib/modules

KERNEL_PATH := $(KERNEL_MODULE)/$(shell uname -r)/build

obj-m += my_first_simple_misc.o

all:
	$(MAKE) -C $(KERNEL_PATH) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_PATH) M=$(PWD) clean
