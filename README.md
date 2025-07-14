# zootrope
Build your own zootrope and phenakistoscope, a kinetic sculpture

# Keywords

- zootrope
- zoetrope
- phenakistiscope
- praxinoscope
- novatrope

# Circuit

BOM: 
- MOSFET FQP30N06L or any logic N-channel (the "L" is important, for Logic)
- MABUCHI RS-385SH-2270 long shaft with gear (10T 5mm)
- KY-040 rotary encoder
- Arduino pro mini
- switch button
- WS2812B Led Ring 5V
- DC jack barrel
- ON/OFF switch button
- 1k resistor
- 100uf capacitor

Look at the circuit diagram to wire the motor properly. !! Careful !! that the mosfet in the circuit diagram is DSG, while the FQP30N06L is GDS (order of the legs), as per the datasheet attached for info).

For the leds, wire the 5V directly (NOT via the arduino) and the data to the arduino pin 10 (PMW).

The microswitch is a simple wiring (GND on one side, and arduino pin A0 on the other).

The KY-040 is also straight forward : GND to GND, + to 5V, CLK to arduino pin 3, DT to arduino pin 5, SW to arduino pin 6 as we need PMW pins.


# 3D

Print the box.stl, place the led ring in the middle, and fix the rotor with screws.

Add the DC jack barrel and the on/off switch on tone side, as well as the KY-040 rotary encoder and a swtich button on the other side.

Close the box with the lid and fix it to it.


# Code

Adjust code as needed for pins, for the following values:

```
// Motor settings
const int motorMinValue = 0;
const int motorMaxValue = 255;
const int motorStepValue = 1;

// Blink settings
const int blinkMinValue = 0;
const int blinkMaxValue = 5000;
const int blinkStepValue = 100;

int motorValue = 20;
int blinkValue = 2700;
#define NUM_LEDS 40
#define LED_PIN 10
int motorPin = 9; // The pin connected to the transistor base
```

Keep the arduino plugged to the computer, and check the serial monitor in Arduino IDE to see the values of the motor and blink that will bring the best zootrope effect, and then set these values as default in the code.

You might need an FTDI232 to send the code to the arduino.


# Usage

Place the 3D model on top of the gear (adjust the slicer settings so that the 3D model is as light as possible, no need to get a lot of infils, or max details).

Turn the circuit on, wait for rotor to spin at speed, the rotary encoder and click it to save the value of the motor speed. Then click the swtich button to switch to the led blink value, and use rotary encoder to adjust the values and click the rotary encoder to save the value (i.e. the switch button allows you to toggle the config of the motor speed or the led blinking frequency).

Note, if your rotor can turn as low as a few RPM (40/50/70), then you can also print phenakistoscope images that will display nicely.

# Go further and link/references

- https://www.youtube.com/watch?v=f3xNOfmQZW0
- https://www.youtube.com/watch?app=desktop&v=5BcFZdcoYD8
- https://www.youtube.com/watch?v=7v01Rgxuyc4
- https://www.youtube.com/watch?v=h0gyoMDzWwI
- https://www.instructables.com/Blooming-Zoetrope-Sculptures/
- https://hackaday.io/project/170379-3d-zoetrope
- https://www.printables.com/model/733786-zootrope-by-lfpoulain
- https://www.yembarwar.com/blog/zoetrope-vinyl

