KERNEL_SRC = /usr/src/kernels/3.10.0-1127.8.2.el7.x86_64
obj-m := mem_channel.o
module-objs := mem_channel.o

all:
	$(MAKE)    -C $(KERNEL_SRC)   M=$(PWD) modules
	gcc -g -std=gnu99 mem_channel_reader.c -o mem_channel_reader
clean:
	rm *.ko *.o
	rm -rf  mem_channel_reader
	rm -rf .mem_channel*
	rm -rf modules.order  Module.symvers 