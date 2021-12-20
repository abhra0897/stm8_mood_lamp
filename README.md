# STM8S Mood Lamp

### Status: Work in progress (or is it?)

[Proper README will be written later, maybe]

This is a mood lamp powered by stm8s microcontroller. The mcu controls an array of ws2812b led. User can set different parameters like speed, brightness, pattern etc using 3 buttons.

The enclosure is designed using tinkercad (I know it for noobs, but that's what I am) and 3d printed using my Creality Ender 3 3d printer.

This is made as a Christmas gift for someone.

### Setup Guide:

1. Download and install SDCC compiler (https://sourceforge.net/projects/sdcc/files/)
2. Build stm8flash for uploading binary to stm8 mcu (https://github.com/vdudouyt/stm8flash)
3. Compile using `make` command (Optionally, clean using `make clean` before this step)
4. Upload using `make burn` command

### Features:

 1. Rainbow effect.
 2. User can modify speed, brightness, inter-pixel hue difference, breathing mode, breathing speed.
 3. Retains settings even after power cut. So, it always starts up at your last settings.
 4. Can launch nuke if buttons are pressed in a secret sequence.

### How to Operate:

Press and hold Middle button for 2 seconds to enable/disable "**breathing effect**". 

1. **Breathing Effect** is enabled:

    When enabled, all LEDS smoothly dim and then again brighten up.

    - Press (or hold) left/right button: Breathing speed decreases/increases
    - Press and hold Middle button for 2 seconds: Disable **breathing effect**

2. **Breathing Effect** is disabled:

    When disabled, all LEDS will follow rainbow effect only.

    1. **Case 1: Bottom LED is NOT blinking:**

        This is **speed change** mode. Here speed of color change can be controlled.

        - Press (or hold) left/right button: Color changing speed decreases/increases
        - Single press middle button: Go to next mode (**pattern change mode**)
        - Press and hold Middle button for 2 seconds: Enable **breathing effect**

    2. **Case 2: Bottom LED is blinking slowly with White color:**

        This is **pattern change** mode. Here inter-pixel color difference can be changed.

        - Press (or hold) left/right button: Color (hue) difference between two pixels decreases/increases
        - Single press middle button: Go to next mode (**brightness change mode**)
        - Press and hold Middle button for 2 seconds: Enable **breathing effect**

    3. **Case 3: Bottom LED is blinking very fast with White color:**

        This is **brightness change** mode.

        - Press (or hold) left/right button: Brightness decreases/increases
        - Single press middle button: Go to first mode (**Speed change mode**)
        - Press and hold Middle button for 2 seconds: Enable **breathing effect**


### Some Demo:

TBD

### TO DO:
- Add more LED effects
- The existing "Breathing effect" is $hit. Need to redesign it.
- Make the circuit in KiCAD, design PCB.
- Improve the 3D model since the current model has million flaws.
- Upload the 3D models here

### Important Resources:
- WS2812 relaxed timing (https://wp.josh.com/2014/05/13/ws2812-neopixels-are-not-so-finicky-once-you-get-to-know-them/)
- github gist on WS2812 drv (https://gist.github.com/TG9541/1761fa86b425a0c909b7bd1cc8017c2b)
- Bit banging on AVR (https://www.instructables.com/Bitbanging-step-by-step-Arduino-control-of-WS2811-/)
- Another github gist. Helpful but timimng didn't match (https://gist.github.com/stecman/eed14f9a01d50b9e376429959f35493a)
- Great resource for stm8 development (https://lujji.github.io/blog/bare-metal-programming-stm8/)
- Good examples based on SDCC compiler (https://github.com/jukkas/stm8-sdcc-examples/blob/master/spi-out-max7219/spi-out-max7219.c)
