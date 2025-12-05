ifeq ($(KERNELRELEASE),)
# Regular build environment
KDIR ?= /lib/modules/$(shell uname -r)/build

.PHONY: all clean

all:
	$(MAKE) -C $(KDIR) M=$(PWD)

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
else
# kbuild part
obj-m := hello1.o hello2.o
ccflags-y += -I$(src)/inc
endif
