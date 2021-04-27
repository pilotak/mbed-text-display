# Mbed LCD & OLED text display library

[![Framework Badge mbed](https://img.shields.io/badge/framework-mbed-008fbe.svg)](https://os.mbed.com/)

Based on work by [sstaub](https://github.com/sstaub/mbedLCD) which is based on [Simon's work](https://os.mbed.com/users/simon/code/TextLCD/).
So what is the difference, why another fork?

- optional R/W pin which enables reading busy flag to avoid fixed waiting time (once the display finishes the task it will let you know)
- rewritten initialization - not done in constructor in case you are using voltage level translator with OE pin & avoids delays
- enums are within class context - in case you have a same variable defined elsewhere in the code, it will not collide
- all display types share the same codebase, they only rewrite pin handling & initialization
- you can specify char size 5x8 or 5x10 pixels
- I2C packpack (PCF8574) supported, there are two pinouts on the market - both are supported

Supports HD44780 _(tested)_, RS0010 _(tested)_ and WS0010 _(untested)_ interfaces commonly found in text LCD/OLED displays.

## Example

### LCD

```cpp
#include "TextLCD.h"

#define DISPLAY_RS_pin PB_1
#define DISPLAY_E_pin  PB_2
#define DISPLAY_D4_pin PB_3
#define DISPLAY_D5_pin PB_4
#define DISPLAY_D6_pin PB_5
#define DISPLAY_D7_pin PB_6

const uint8_t upArrow[8] = {
    0b00100,
    0b01010,
    0b10001,
    0b00100,
    0b00100,
    0b00100,
    0b00000,
};

const uint8_t downArrow[8] = {
    0b00000,
    0b00100,
    0b00100,
    0b00100,
    0b10001,
    0b01010,
    0b00100,
};

const uint8_t rightArrow[8] = {
    0b00000,
    0b00100,
    0b00010,
    0b11001,
    0b00010,
    0b00100,
    0b00000,
};

const uint8_t leftArrow[8] = {
    0b00000,
    0b00100,
    0b01000,
    0b10011,
    0b01000,
    0b00100,
    0b00000,
};

TextLCD lcd(DISPLAY_RS_pin, DISPLAY_E_pin, DISPLAY_D4_pin, DISPLAY_D5_pin,
             DISPLAY_D6_pin, DISPLAY_D7_pin);

int main() {
    ThisThread::sleep_for(50ms); // give a time to wakeup the controller

    lcd.init();
    lcd.display(TextLCD::DISPLAY_ON);

    lcd.printf("Hello world\n");

    // try display functions
    ThisThread::sleep_for(2s);
    lcd.display(TextLCD::CURSOR_ON);
    lcd.display(TextLCD::BLINK_ON);
    ThisThread::sleep_for(4s);
    lcd.display(TextLCD::BLINK_OFF);
    ThisThread::sleep_for(2s);
    lcd.display(TextLCD::CURSOR_OFF);
    ThisThread::sleep_for(2s);

    // clear
    lcd.cls();

    // create custom characters
    lcd.create(0, downArrow);
    lcd.create(1, upArrow);
    lcd.create(2, rightArrow);
    lcd.create(3, leftArrow);

    // show custom characters
    lcd.locate(0, 0);
    lcd.character(0, 0, 0);
    lcd.character(2, 0, 1);
    lcd.character(4, 0, 2);
    lcd.character(6, 0, 3);

    // try long text
    lcd.locate(0, 0);
    lcd.printf("Really long hello world\nwith scrolling");
    ThisThread::sleep_for(2s);

    // scroll
    while (1) {
        for (auto pos = 0; pos < (23 - lcd.columns()); pos++) {
            lcd.display(TextLCD::SCROLL_LEFT);
            ThisThread::sleep_for(150ms);
        }

        ThisThread::sleep_for(1s);

        for (auto pos = 0; pos < (23 - lcd.columns()); pos++) {
            lcd.display(TextLCD::SCROLL_RIGHT);
            ThisThread::sleep_for(150ms);
        }

        ThisThread::sleep_for(2s);
    }
}
```

### OLED

```cpp
#include "TextOLED.h"

#define DISPLAY_RS_pin PB_1
#define DISPLAY_E_pin  PB_2
#define DISPLAY_D4_pin PB_3
#define DISPLAY_D5_pin PB_4
#define DISPLAY_D6_pin PB_5
#define DISPLAY_D7_pin PB_6
#define DISPLAY_RW_pin PB_7

TextOLED oled(DISPLAY_RS_pin, DISPLAY_E_pin, DISPLAY_D4_pin, DISPLAY_D5_pin,
             DISPLAY_D6_pin, DISPLAY_D7_pin, DISPLAY_RW_pin);

const char spinner[] = {0x7C, 0x2F, 0x2D, 0xDA};

int main() {
    ThisThread::sleep_for(500ms); // give a time to wakeup the controller

    oled.init(TextOLED::FONT_EUROPEAN_I);
    oled.display(TextOLED::DISPLAY_ON);

    // animate spinner
    while(1){
        for (size_t i = 0; i < sizeof(spinner); i++) {
            oled.character(0, 0, spinner[i]);
            ThisThread::sleep_for(150ms);
        }
    }
}
```

### LCD I2C
```cpp
#include "TextLCD_I2C.h"

#define SDA PB_9
#define SCL PB_8

TextLCD_I2C lcd(SDA, SCL, false); // set to true if LCD backpack has different pinout

int main() {
    ThisThread::sleep_for(500ms); // give a time to wakeup the controller

    if (lcd.init()) {
        printf("init OK\n");
        lcd.display(TextLCD_I2C::DISPLAY_ON);
        lcd.setBacklight(true);
        lcd.printf("Hello world\n");

    } else {
        printf("error\n");
    }
}
```

### OLED I2C
You can use the constructor same as above or you can pass existing I2C object

```cpp
#include "TextOLED_I2C.h"

#define SDA PB_9
#define SCL PB_8

I2C i2c(SDA, SCL)
TextOLED_I2C oled;

int main() {
    ThisThread::sleep_for(500ms); // give a time to wakeup the controller

    if (oled.init(&i2c)) {
        printf("init OK\n");
        oled.display(TextOLED_I2C::DISPLAY_ON);
        oled.printf("Hello world\n");

    } else {
        printf("error\n");
    }
}
```