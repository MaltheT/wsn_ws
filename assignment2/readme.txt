The labnote.ipynb includes the comparison plot and few comments on the observations. 

To run the code extract the RSSI.c and Makefile to your contiki ws.
For me /home/user/contiki-ng/exercises/assignment2. 

To run the code on the mote:

$ make TARGET=sky MOTES=/dev/ttyUSB0 RSSI.upload login

Or however you have setup the usb connection. 