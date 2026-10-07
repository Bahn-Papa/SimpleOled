## DisplayNumberColonClass
With this class it is possible to write only numbers/digits and the colon onto the display.<br>
The style of the font used must be defined in a derived class and must have a height of 64 pixels.

### Features:
The class has the following function set:

| Function | Description |
| --- | --- |
| DisplayNumberColonClass() | Constructor: create a new instance of the class. |
| DisplayNumberColonClass( chipType ) | Constructor: creates a new instance of the class with the given `chipType`.<br>The chip type can be **_CHIP_TYPE_SSD1306_** or **_CHIP_TYPE_SH1106_**. |
| | |
| DrawDigit( column, digit ) | Draw the given `digit` onto the display starting at the given `column`. |
| ClearDigit( column ) | Clear a digit starting at the given `column`. |
| | |
| DrawDigit0( column ) | Draw the digit '0' onto the display starting at the given `column`. |
| DrawDigit1( column ) | Draw the digit '1' onto the display starting at the given `column`. |
| ... | ... |
| DrawDigit9( column ) | Draw the digit '9' onto the display starting at the given `column`. |
| | |
| DrawColon( column ) | Draw the colon onto the display starting at the given `column`. |
| ClearColon( column ) | Clear the colon from the display starting at the given `column`. |
| | |
