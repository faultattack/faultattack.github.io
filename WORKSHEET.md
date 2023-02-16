# Worksheet for 2023 Workshop on Fault Injection Attacks 


PicoEMP basics 
==============
The PicoEMP that you received comes with 
There is a short usage guide at [PicoEMP github](https://github.com/newaetech/chipshouter-picoemp#using-the-picoemp)

Arm the device from the ARM button, and once loaded press the button labeled PULSE. 

DO NOT REMOVE THE PLASTIC COVER OR INJECTOR TIP. 


Hello Glitching 
================

Load the binary called hello.uf2 to the standalone RPI Pico. When the Pico is powered on with the BOOTSEL button depressed it will expose itself as a USB mass storage device. After loading the firmware the board should blink the LED three times when powered on via USB. 

Try to find a spot on the Pico board where it starts blinking  after injecting a pulse. Three pulses means that it crashed totally or went to a reset  due to e.g. reset line pull-down. Non-stop slow blinking means that it got out of the infinite loop successfully. Other LED sequences may also happen that indicate some type of success. You may have to occassionally reset the device as it can also hang. 

PicoEMP USB Serial 
==================

Connect the PicoEMP to a computer via USB. You should find the device 

On Mac and Linux the USB serial devices are somewhere under dev. 
$ ls /dev/\*usb\* should list all the connected USB serial devices. 

On Windows you can open device manager and look for a COM port number. Good old putty.exe can then connect to the COM port. 

Flag Binary
============

We have our own CTFSM (Capture-the-Flag Security Module) that is intended to give you a few puzzles. It is distributed both in .elf and .uf2 format. 
The source code for the application is in the source-folder as main.c. 

You can connect to the flag binary running on Pico the same way as to PicoEMP. 

Inspect the target source and the elf binary. 
There are multiple possibilities for fault injection in this binary: 
    - Loop escapes with branching or volatile exit condition 
    - Memory loads & stores 
    - Random corruption / function fall through 


Inspecting the source may give you some idea where to attack. Not all of the obvious looking stuff in the c-source can be attacked successfully. The elf should confirm your guesses once you inspect it with IDA/Ghidra/Cutter. 

For a list of typical fault attack effects there is an excellent list in the [NCC Group blog](https://research.nccgroup.com/2021/07/07/an-introduction-to-fault-injection-part-1-3/). 

Having two USB-ports and micro-USB cables allows you to connect both to  PicoEMP and flag-target at the same time. This is not strictly necessary but will make things much easier. 

If you have two cables you can try triggering a pulse and a simultaneous action of flag-target via USB-serial. It might get you one of the flags if you're lucky. The provided example is in Python, and you could have better realtime properties by writing the serial communication in Rust, C, Golang or some other language. Anyways, you probably want to use fast trigger instead. 

In the PicoEMP UI you can configure the fast trigger with a set delay in cycles. We use the target GPIO 0 to give us a pulse. There are two trigger points in the software and you can add more if you like to. Hypothetically we could solder our trigger wire to target USB pads or have some other device inspect USB traffic but that would be too much work for our short session. 


Fault Model Construction 
========================

There is also a binary called glitch_detect. It runs loops and tries to detect abnormal behavior. It reports findings at every glitch detection and has a keep alive every now and then, until it hangs. 

- Look for a pre-known sensitive spot and identify the fault behavior
- Look for a spot that has a specific impact (e.g. logic glitch, SRAM glitch)



Badge Target 
============

This device is not really that susceptible to EMFI. It runs really slowly (about 1Mhz) so timing would not be an issue but we would likely need much more power to inject a successful fault. Arduino Nano, which is built using the same CPU, glitches fine but the Disobey badge does not. Go figure. You can try to find a spot on it where it does something. It should have at least one spot that is not totally mute. 

