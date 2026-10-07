## DisplayBaseClass
This class provides the basic functions for communicating with the display via I2C and to draw data onto the display. It is intended only for OLED displays with sh1106/ssd1306 driver chip and a resolution of 128 x 64 pixel.

### Features:
The class has the following function set:

#### Public Functions:
| Function | Description |
| --- | --- |
| DisplayBaseClass() | Constructor: creates a new instance of the class
| DisplayBaseClass( chipType ) | Constructor: creates a new instance of the class with the given `chipType`.<br>The chip type can be **_CHIP_TYPE_SSD1306_** or **_CHIP_TYPE_SH1106_**. |
| | |
| Init( address ) | Initializes communication with the display using the specified `address`.<br>You should use this function only if you created the instance using the constructor with the chip type parameter.
| Init( chipType, address ) | Initializes communication with the display using the specified `chipType` and `address`. |
| | |
| SetDisplayColumnOffset( offset ) | Sets the column start position of the display to `offset`.<br>Only useful for chip type **_SH1106_** |
| SetDisplayLineOffset( offset ) | Sets the line start position of the display to `offset`. |
| | |
| ClearDisplay() | Clear all pixels on the display so you get a blank display screen. |
| ClearPage( page ) | Clear all pixel of the given `page` of the display.<br>The display is arranged in 8 rows with 8 pixels height each.<br>One such row is called **page**. |
| | |
| SetDrawPosition( page, column ) | Position the drawing cursor to the given `page` and `column`. |
| Draw( buffer, size ) | Sends `size` units of data from the specified `buffer` to the display. |
| | |
| SetInverse( inverse ) | Invers the entire display (`inverse`=true).<br>Each white pixel will get black and each black pixel will get white. |
| Flip( flip ) | This function will turn the output on the display by 180 degree and clears the display (`flip`=true). |
| | |

#### Protected Functions:
| Function | Description |
| --- | --- |
| Initsh1106() | Send initialization sequence to display with SH1106 driver chip. |
| Initssd1306() | Send initialization sequence to display with SSD1306 driver chip. |
| | |
| SendCommand( opCode ) | Send the command `opCode` without a parameter to the display. |
| SendCommand( opCode, parameter ) | Send the command `opCode` with the `parameter` to the display. |
| | |
