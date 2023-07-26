obj-m += data_size_ker.o 


CROSS_COMPILE=/home/zbw/Desktop/aarch64-linux-gnu/bin/aarch64-linux-gnu-
CROSS_COMPILE_ARM32=/home/zbw/Desktop/arm-linux-gnueabi/bin/arm-linux-gnueabi-
KERNEL_DIR := /home/zbw/Desktop/android_kernel_oneplus_msm8996/out/
PWD := $(shell pwd)

all: 
	make -C $(KERNEL_DIR) ARCH=arm64 CROSS_COMPILE=$(CROSS_COMPILE) CROSS_COMPILE_ARM32=$(CROSS_COMPILE_ARM32) M=$(PWD) modules
clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
