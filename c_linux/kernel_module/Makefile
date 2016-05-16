
#ifneq ($(KERNELRELEASE), 0)
#obj-j := mymodule.o
#else
#	KDIR ?= /lib/modules/`uname -r`/build
#
#default:
#	$(MAKE) -C $(KDIR) M=$$PWD
#endif
CONFIG_MODULE_SIG=n
obj-m += mymodule.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
