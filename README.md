# SimpleOled
SimpleOled is a small library that provides classes for text output on an OLED display (128 x 64 pixel) with sh1106/ssd1306 driver chip.

#### Used Resources
This is a library to use with the Arduino IDE.<br>
The library itself makes use of the **_Wire_** library delivered with the Arduino IDE.

## Why another OLED library ?
I was searching for a simple OLED library to print out some infos (e.g.: debug infos).
But all I found were libraries with much more capabilities that I don't want to have.
Therefore these libraries need many resources (e.g.: flash and RAM memory).
So I decide to write my own simple library.

The library consits of several classes: a base class and classes derived from it with specific functionality.

### DisplayBaseClass
This class provides the basic functions for communicating with the display via I2C.<br>
In addition, there are functions for positioning the drawing cursor and to draw data on the screen.
See a more [detailed description.](docs/DisplayBaseClass.md)

### SimpleDisplayClass
With this class it is possible to write text and numbers on a display.<br>
It is derived from the class **_DisplayBaseClass_**.<br>
See a more [detailed description.](docs/SimpleDisplayClass.md)

### DisplayNumberColonClass
With this class it is possible to write only numbers and the colon onto the display.<br>
It is derived from the class **_DisplayBaseClass_**.<br>
The font used must have a height of 64 pixels and the style must be defined in a derived class.<br>
See a more [detailed description](docs/DisplayNumberColonClass.md).

### Display7SegmentClass
This class is derived from the class **_DisplayNumberColonClass_** and spezifies the font for that class.<br>
The font here looks like a seven segment display.
