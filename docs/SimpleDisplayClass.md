## SimpleDisplayClass

With this class it is possible to write just text on a display.<br>
The font used is a monospace 8x8 pixel font.

If you want to print some data (e.g.: numbers) on the display than you have to build your output into a buffer first.<br>
Then deliver the buffer as parameter to the Print functions.<br>
(see example 'PrintText')

### Features:
The library provides a global variable **_g_clDisplay_** of class **_SimpleDisplayClass_** to work with the OLED display.

The class has the following function set:

| Function | Description |
| --- | --- |
| Init( chipType, address ) | Initialize the display to use `chipType` and I2C `address`. |
| | |
| PrintChar( char ) | Print `char` at the actual cursor position onto the display. |
| | |
| Print( text ) | Print `text` starting from the actual cursor position onto the display. |
| PrintLn( text ) | Print `text` starting from the actual cursor position onto the display and<br>set the cursor to the beginning of the next line. |
| | |
| Clear() | Clear the hole display and sets the cursor to the top left position of the display. |
| ClearLine() | Clear just the current line and sets the cursor to the beginning of the line. |
| ClearLine( line ) | Clear the given `line` and set the cursor to the beginning of that line. |
| | |
| Home() | Set the cursor to the top left position of the display. |
| SetCursor( line, column ) | Set the cursor to the given `line` and `column` of the display. |
| | |
| SetInverseFont( inverse ) | Set the print mode to white background with black characters (`inverse`=true)<br>or black background with white characters (`inverse`=false). |
| | |
| SetInverse( inverse ) | Inverse the entire display (`inverse`=true).<br>Each white pixel will get black and each black pixel will get white. |
| Flip( on ) | turns the output to the display by 180 degree (`on`=true) |
| | |
| SetPrintMode( mode ) | There are three print modes:<br>- PM_SCROLL_LINE: fill the lines of the display and scroll them out to the top<br>- PM_OVERWRITE_NEXT_LINE: continuously overwrite the lines of the display from the beginning to the end<br>- PM_OVERWRITE_SAME_LINE: overwrite the actual line were the cursor is in. |
| | |

### How to use the class **_SimpleDisplayClass_**

Have a look into the examples and see how to use the functions.<br>
All examples assume to control an OLED display with an sh1106 chip.<br>
If your display has an ssd1306 chip you have to adapt the 'Init()' function.

| Example | Description |
| --- | --- |
| PrintText | This example shows how the cursor is placed and<br>prints some text lines in normal and inverse mode. |
| PrintMode | This example shows the different print modes.<br>The print modes are described at the top of the file 'PintMode.ino'. |
| FlashFlipDisplay | Here you can see how to use SetInverse() to flash the display.<br>And you will see how to 'flip' the display (turn the output by 180 degree) |
