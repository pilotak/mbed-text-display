/*
MIT License
Copyright (c) 2021 Pavel Slama
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef TEXT_OLED_H
#define TEXT_OLED_H

#include "TextLCD_I2C.h"

class TextOLED_I2C: public TextLCD_I2C {
  public:
    /**
     * @brief Create an I2C OLED interface
     *
     * @param size Panel size
     * @param address 7-bit I2C address of the expander
     * @param frequency I2C bus speed
     */
    TextOLED_I2C(lcd_size_t size = SIZE_16x2, int8_t address = TEXT_DISPLAY_I2C_ADDRESS);

    /**
     * @brief Create an I2C OLED interface
     *
     * @param sda SDA pin
     * @param scl SCL pin
     * @param size Panel size
     * @param address 7-bit I2C address of the expander
     * @param frequency I2C bus speed
     */
    TextOLED_I2C(PinName sda, PinName scl, lcd_size_t size = SIZE_16x2,
                 int8_t address = TEXT_DISPLAY_I2C_ADDRESS, uint32_t frequency = 400000);

    /**
     * @brief Initialize display
     *
     * @param i2c_obj I2C object to pass
     * @param font Font table used
     * @param chars Size of 1 character, can be 5x8 or 5x10 on some displays
     *
     * @return true if success, false otherwise
     */
    bool init(I2C *i2c_obj = nullptr, lcd_font_t font = FONT_JAPANESE, lcd_char_t chars = CHAR_5X8);
};

#endif
