NAME=rapd2
obj-m := $(NAME).o

all:
	$(CC) rapd2_server.c -o rapd2_server
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

load: all
	sudo insmod $(NAME).ko
	sudo chmod a+rw /dev/$(NAME)
	dmesg --color=always | tail -n 1

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

unload:
	sudo rmmod $(NAME).ko
	dmesg --color=always | tail -n 1
