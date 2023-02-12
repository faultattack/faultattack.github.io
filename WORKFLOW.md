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

Flag Binary
============

We have our own CTFSM (Capture-the-Flag Security Module) that is intended to give you a few puzzles. It is distributed both in .elf and .uf2 format. 
The source code for the application is in the source-folder as main.c. 

There are multiple possibilities for fault injection in this binary: 
    - Loop escapes with branching
    - Memory loads & stores 
    - Random corruption / function fall through 

Inspecting the source may give you some idea where to attack. Not all of the obvious looking stuff in the c-source can be attacked successfully. Best is to look into the elf with IDA/Ghidra. 

Fault Model Construction 
========================

There is also a binary called glitch_detect. It runs loops and tries to detect abnormal behavior. It reports findings every now and then, until it hangs. You can use this with your glitch spots to find-out the ways you can affect the target. 



