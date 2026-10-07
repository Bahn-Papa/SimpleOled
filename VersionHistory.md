## Simple OLED library

| Version | Date | Description |
| --- | --- | --- |
| 2.01.00 | 06.10.2026 | Two new classes created to simple show numbers on a display.<br>- 'DisplayNumberColonClass' is the base class with all virtual functions to draw the numbers and the colon.<br>- 'Display7SegmentClass' implements the numbers as seven segment font.
| 2.00.00 | 01.10.2026 | In preparation to new display classes the existing class was split into two new one:<br>- 'DisplayBaseClass' (common functions)<br> - 'SimpleDisplayClass' (specific functions for simple text output)<br>The streamlined 'SimpleDisplayClass' has the same functionality as before. Only the common functions were moved to the new base class 'DisplayBaseClass'.
| 1.02.01 | 07.11.2023 | Bug Fix: did not start with chip type sh1106 |
| 1.02.00 | 06.11.2023 | Add support for ESP32 and rework of function Init() |
| 1.01.00 | 21.09.2023 | Set default chip type to ssd1306<br>Don't send anything over I2C if no display is connected. |
| 1.00.01 | 21.09.2023 | First working version that supports two chips: sh1106 and ssd1306. |
| 0.09.01 | 19.09.2023 | First implementation of the class 'SimpleDisplayClass'. |
