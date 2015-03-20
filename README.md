# femtousb-blink-example
Blink an LED on and off using the FemtoUSB (ATSAMD21E18A).


Install the Eclipse IDE, Terry Guo's GNU ARM Embedded Tools, the GNU ARM Eclipse Plugins. 
We have the 'build-essential' apt package installed on our Ubuntu Linux machine as well.

You may use Git to checkout a copy of this example, and open it with Eclipse. 
Once you build the example, you should have a file named `led_example_flash.bin`. 
Upload it using our forked version of the BOSSA utility.

...Assuming your board is connected to /dev/ttyACM0 over USB, and assuming you are in your example project directory with a BIN file in there:
```
bossac -p ttyACM0 -w -v led_toggle_flash.bin
```
...This would connect to port ttyACM0, write and verify 'led_toggle_flash.bin'.

Power cycle your board, and your uploaded code will run. 
This example assumes you have an LED (with a resistor) wired up to PA08 and GND.

To go back into bootloader mode and upload another example, simply disconnect your board, keep PA15 connected to GND (to bring PA15 low), and plug your board back in. 
Once powered on, you may disconnect PA15. Using the bossa utility, you can now interact with the board's SAM-BA bootloader over USB again.
