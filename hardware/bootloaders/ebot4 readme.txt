*************** make *************************************

make VID=0x2CB0  PID=0x0004 F_CPU=16000000


********************** program ***************************
avrdude -p m32u4  -c usbtiny -U flash:w:ebot4.hex -U efuse:w:0xcb:m -U hfuse:w:0xd8:m -U lfuse:w:0xff:m


//please dont forget to set environmental variables