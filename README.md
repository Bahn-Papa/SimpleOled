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
This class provides the basic functions for communicating with the display via I2C.

### SimpleDisplayClass
With this class it is possible to write text on a display.<br>
If you want to print some data (e.g.: numbers) on the display than you have to build your output in a buffer first.<br>
Then deliver the buffer as parameter to the Print functions.<br>
(see example 'PrintText')


#### Features:
The library provides a global variable **_g_clDisplay_** of class **_SimpleDisplayClass_** to work with the OLED display.<br>
The class has the following function set:

| Function | Description |
| --- | --- |
| Init( \<chip\>, \<address\> ) | Initialize the display which \<chip\> and which I2C \<address\> is used. |
| | |
| PrintChar( \<char\> ) | Prints \<char\> at the actual cursor position on the display |
| | |
| Print( \<text\> ) | Prints \<text\> starting from the actual cursor position on the display |
| PrintLn( \<text\> ) | Prints \<text\> starting from the actual cursor position on the display and<br>sets the cursor to the beginning of the next line |
| | |
| Clear() | clears the hole display and sets the cursor to the top left position of the display |
| ClearLine() | clears just the current line and sets the cursor to the beginning of the line |
| ClearLine( \<line\> ) | clears the given line and sets the cursor to the beginning of that line |
| | |
| Home() | sets the cursor to the top left position of the display |
| SetCursor( \<line\>, \<column\> ) | sets the cursor to the given \<line\> and \<column\> of the display |
| | |
| SetInverseFont( \<inverse\> ) | sets the print mode to white background with black characters (\<inverse\>=true)<br>or black background with white characters (\<inverse\>=false) |
| | |
| SetInverse( \<inverse\> ) | inverses the entire display (\<inverse\>=true)<br>each white pixel will get black and each black pixel will get white |
| Flip( \<on\> ) | turns the output to the display by 180 degree (\<on\>=true) |
| | |
| SetPrintMode( \<mode\> ) | There are three print modes:<br>PM_SCROLL_LINE: fill the lines of the display and scroll them out to the top<br>PM_OVERWRITE_NEXT_LINE: continuously overwrite the lines of the display from the beginning to the end<br>PM_OVERWRITE_SAME_LINE: overwrite the actual line |

### How to use the class **_SimpleDisplayClass_**

Have a look into the examples and see how to use the functions.<br>
All examples assume to control an OLED display with an sh1106 chip.<br>
If your display has an ssd1306 chip you have to adapt the 'Init()' function.

| Example | Description |
| --- | --- |
| PrintText | This example shows how the cursor is placed and<br>prints some text lines in normal and inverse mode. |
| PrintMode | This example shows the different print modes.<br>The print modes are described at the top of the file 'PintMode.ino'. |
| FlashFlipDisplay | Here you can see how to use SetInverse() to flash the display.<br>And you will see how to 'flip' the display (turn the output by 180 degree) |
