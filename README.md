# I2C_LCD_Controller

Code I wrote for the arduino Uno to interface with a nameless LCD display I have. This code uses I2C and is not a library, I recommend using LiquidDisplay if you are looking for a library.
This software is an example of how I2C commands are sent from the arduino to the display.

<h4 style="color:red">This code has only been tested with Arduino Uno</h4>

## Wire connections

| Arduino | LCD |
| ------- | --- |
| 5V      | Vcc |
| GND     | GND |
| SDA     | A4  |
| SCL     | A5  |

### Running Code

1. Open 12C_LCD_Controller.ino in Arduino IDE
2. Select your Arduino com port
3. Open serial monitor
4. Run code. You should see "Welcome" display on the LCD display

#### Serial Commands

**All commands start with the "!" character in the serial monitor**
_User serial monitor to test serial commands_

**!clr**
Clear the entire display

**!nl**
Move cursor to new line

**!cl**
Shift cursor left

**!cr**
Shift cursor right

**!dl**
Scroll display left

**!dr**
Scroll display right

**!hm**
Move both the cursor and the display to the home position

**!coff**
Turn off the cursor

**!con**
Turn on the cursor AND turn blinking off

**!cblink**
Turn on the cursor AND turn blinking on
