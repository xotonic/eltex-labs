
LIBUSB MOUSE INTERRUPT
======================

DESCRIPTION
-----------

Программа отключает драйвер мыши и читает ее данные
с USB-порта

Для захвата устройства необходимо знать 
 * `VENDOR_ID`(`-v`)
 * `PRODUCT_ID`(`-p`)
 * также можно задать `endpoint address` (`-e`)
После 5 секунд простоя выходит и возвращает драйвер

USAGE
-----
```
$ lsusb
Bus 002 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub
Bus 001 Device 005: ID 0e0f:0002 VMware, Inc. Virtual USB Hub

#                    VICTIM DEVICE
#                 VENDOR_ID|PRODUCT_ID
#                      vvvv|vvvv
Bus 001 Device 014: ID 1bcf:0007 Sunplus Innovation Technology Inc. Optical Mouse

Bus 001 Device 004: ID 0e0f:0002 VMware, Inc. Virtual USB Hub
Bus 001 Device 003: ID 0e0f:0008 VMware, Inc. 
Bus 001 Device 002: ID 0e0f:0003 VMware, Inc. Virtual Mouse
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
Bus 003 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
Bus 004 Device 002: ID 0e0f:0002 VMware, Inc. Virtual USB Hub
Bus 004 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub

$ sudo ./mouse -v 0x1bcf -p 0x0007
x 0	y 1	
x -1	y 0	
x -1	y 0	
x -1	y 0	
x -1	y 0	
x -1	y -1	
x -1	y -1	
x -1	y 0	
x -1	y 0	
x 0	y -1	
x -1	y 0	
x -1	y 0	
x -1	y 0	
x -1	y 0	
x 0	y 0	
x 0	y 1	
x 0	y 1	
x 0	y 1	
x 1	y 1	
x 1	y 1	
x 1	y 1	
x 1	y 1	
x 0	y 0	RIGHT 
x 0	y 0	
x 0	y 0	LEFT 
x 0	y 0	
x 1	y 0	
Timeout
```