KERNEL_SRC = /usr/src/linux-headers-4.19.0-8-amd64/

obj-m := mem_channel.o
module-objs := mem_channel.o

all:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) modules
	#gcc -g -std=gnu99 mem_channel_reader.c -o mem_channel_reader
clean:
	rm *.ko *.o
	#rm -rf  mem_channel_reader