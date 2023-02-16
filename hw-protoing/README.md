# Proto phase
Arduino UNO prototyping with Disobey 2017 badge.

## Connection diagram

* Badge reference [layout](https://github.com/disobeyfi/badge-2017/blob/master/hw/disobey_badge_layout_v7.pdf) and [schematics](https://github.com/disobeyfi/badge-2017/blob/master/hw/disobey_badge_sch_v4.pdf).
* Arduino reference [pinout](arduino-uno-pinout-diagram.png)


Connection between Arduino UNO <-> Badge:

| Arduino UNO | The Bagde |
| ------------- | ------------- |
| PIN GND | PIN 1 (GND) |
| PIN 10 (SS) | PIN 2 (RST) |
| PIN 11 (MOSI) | PIN 3 (MOSI) |
| PIN 13 (SCK) | PIN 4 (SCK) |
| PIN 3.3V | PIN 5 (VCC) |
| PIN 12 (MISO) | PIN 6 (MISO) |

## Arduino preparation
1. Open Arduino IDE
1. Open Arduino Sketch `File -> Examples -> 11.ArduinoISP -> ArduinoISP`
1. Select target as `Arduino UNO` and proper port (e.g `/dev/ttyACM0`). Flash it (upload).

## Flashing the Badge (Arduino IDE)
1. Download https://www.arduino.cc/en/uploads/Tutorial/breadboard-1-6-x.zip and check current Arduino Sketchbook directory (can be seen via `Preferences -> Sketchbook location`)
1. Create new directory in `mkdir -p $SKETCHBOOK_LOCATION/hardware`
1. Unzip downloaded `breadboard.zip` into new directory, should be now something like: `$SKETCHBOOK_LOCATION/hardware/breadboard/`
1. Restart Arduino IDE
1. Create empty sketch for Disobey badge code [morse.c](https://github.com/disobeyfi/badge-2017/blob/master/morse.c)
1. Select target as `ATmega328 on a breadboard (8 MHz internal clock)`, port is same as in above example, e.g. `/dev/ttyACM0`
1. Select `Programmer -> Arduino as ISP`
1. Select `Sketch -> Upload using programmer`. Note: if you get error `Failed programming: no upload port provided`, remember to select correct port!
1. After this firmware should be running in the badge

## Flashing the Bagde (avrdude)
1. Compile .hex (using Arduino IDE)
1. Run: `avrdude -v -p atmega328p -c stk500v1 -P /dev/ttyACM0 -b 19200 -U flash:w:disobey-2017-badge.ino.hex:i`
