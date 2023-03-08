The plot.pdf includes the plots and few comments on the observations. 


To run the code extract the dct.c and Makefile to your ws.
For me /home/user/contiki-ng/exercises/assignment1/dct. 

To run the code natively put commands:

$ make TARGET=native
$ ./dct.native

To run the code on the mote:

$ make TARGET=sky MOTES=/dev/ttyUSB0 dct.upload login

Or however you have setup the usb connection. 