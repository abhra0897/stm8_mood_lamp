## Status: Incomplete

[Proper README will be written later]

Trying to make a POV display using stm8 mcu as I'm learning stm8. But for now, just having some fun with it. I may even change my goal and make something else rather than a pov display.

The stm8 is connected to a 4Mbit Adesto spi standard flash where images/animations might be stored. Currently both the flash driver and ws2812 driver work, but yet to decide what type of led will be used finally.

### Setup Guide:

1. Download and install SDCC compiler (https://sourceforge.net/projects/sdcc/files/)
2. Build stm8flash for uploading binary to stm8 mcu (https://github.com/vdudouyt/stm8flash)
3. Compile using `make` command (Optionally, clean using `make clean` before this step)
4. Upload using `make burn` command

### Some Demo:

![demo gif](images/ws2812_demo.gif)


### Done:
- AT25SF041 flash driver
- WS2812 / WS2811 driver

### Problems:
- Biggest one: slip ring and brush based power transfer is not reliably working. I suck at mechanical designs.
- WS2812 is too slow for POV.

### TO DO:
- Load data to flash using UART from PC (images/ animation)
- Read the loaded data from the flash
- Make the circuit in KiCAD
- Select what LED to use (WS2812? or normal LED with serial LED-driver IC?)
- Read from flash and drive LEDs based on that data
- And lots of other works related to the motor selection/body design/power transfer (slip ring or wireless?) etc..

### Important Resources:
- WS2812 relaxed timing (https://wp.josh.com/2014/05/13/ws2812-neopixels-are-not-so-finicky-once-you-get-to-know-them/)
- github gist on WS2812 drv (https://gist.github.com/TG9541/1761fa86b425a0c909b7bd1cc8017c2b)
- Bit banging on AVR (https://www.instructables.com/Bitbanging-step-by-step-Arduino-control-of-WS2811-/)
- Another github gist. Helpful but timimng didn't match (https://gist.github.com/stecman/eed14f9a01d50b9e376429959f35493a)
- Great resource for stm8 development (https://lujji.github.io/blog/bare-metal-programming-stm8/)
- Good examples based on SDCC compiler (https://github.com/jukkas/stm8-sdcc-examples/blob/master/spi-out-max7219/spi-out-max7219.c)
