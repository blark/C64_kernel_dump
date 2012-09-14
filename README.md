PIC 18F4520 C64 Kernel PROM Dump
================================

This is some quick and dirty code that I wrote to read a C64 kernel rom (the chip marked 901227-02 in my C64) and dump it out to the serial terminal.

For me this was an exercise to learn about the old hardware that I used over 20 years ago -- and at the same time learn a little more about the PIC USART. It was a lot of fun, I hope that the code is useful to someone.

How it works
------------

The kernel PROM has 8kbytes of storage, and 13 address pins (A0-A12). These pins are hooked up to my PIC18F4520 like so:

A0 -> RD0... (the rest in order)
A8 -> RA0... (the rest in order)

Likewise because the chip has 8kB of storage, for each of the 0x1FFF addresses there needs to be 8 bits, so it has 8 outputs labelled O1 - O8, these outputs are attached (in order) on PORT B of the PIC.

There are three pins left, power, ground and CS (hold this one low). 

Now with all of that hooked up I just send out each address from 0x000 to 0x1FFF to the corresponding pins and read PORTB -- then spit out the output to the serial terminal. 

I used a ROM dump of this chip I found on the internet to verify that my program works correctly ;)

email me at blark -at- pwnp.al if you have any questions! 

