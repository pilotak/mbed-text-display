# Mbed LCD & OLED text display library

[![Framework Badge mbed](https://img.shields.io/badge/framework-mbed-008fbe.svg)](https://os.mbed.com/)

Based on work by [sstaub](https://github.com/sstaub/mbedLCD) which is based on [Simon's work](https://os.mbed.com/users/simon/code/TextLCD/)

Supports both HD44780 and RS0010 interfaces commonly found in text LCD/OLED displays.

## Example

```cpp
#include "TextLCD.h"

uint8_t upArrow[8] = {
    0b00100,
    0b01010,
    0b10001,
    0b00100,
    0b00100,
    0b00100,
    0b00000,
};

uint8_t downArrow[8] = {
    0b00000,
    0b00100,
    0b00100,
    0b00100,
    0b10001,
    0b01010,
    0b00100,
};

uint8_t rightArrow[8] = {
    0b00000,
    0b00100,
    0b00010,
    0b11001,
    0b00010,
    0b00100,
    0b00000,
};

uint8_t leftArrow[8] = {
    0b00000,
    0b00100,
    0b01000,
    0b10011,
    0b01000,
    0b00100,
    0b00000,
};

TextLCD lcd(DISPLAY_RS_pin, DISPLAY_E_pin, DISPLAY_D4_pin, DISPLAY_D5_pin,
             DISPLAY_D6_pin, DISPLAY_D7_pin, DISPLAY_RW_pin);

int main() {
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
    lcd.printf("Really long hello world\nwith short text");
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

```cpp
#include "TextOLED.h"

TextOLED lcd(DISPLAY_RS_pin, DISPLAY_E_pin, DISPLAY_D4_pin, DISPLAY_D5_pin,
             DISPLAY_D6_pin, DISPLAY_D7_pin, DISPLAY_RW_pin);

const char spinner[] = {0x7C, 0x2F, 0x2D, 0xDA};

int main() {
    lcd.init(TextOLED::FONT_EUROPEAN_I);
    lcd.display(TextOLED::DISPLAY_ON);

    // animate spinner
    while(1){
        for (size_t i = 0; i < sizeof(spinner); i++) {
            lcd.character(0, 0, spinner[i]);
            ThisThread::sleep_for(150ms);
        }
    }
}
```
