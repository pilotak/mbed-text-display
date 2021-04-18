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

#include "TextOLED_I2C.h"

TextOLED_I2C::TextOLED_I2C(lcd_size_t size, int8_t address):
    TextLCD_I2C{size, address} {
    MBED_ASSERT(size != SIZE_20x4);
}

TextOLED_I2C::TextOLED_I2C(PinName sda, PinName scl, lcd_size_t size, int8_t address,
                           uint32_t frequency):
    TextLCD_I2C{sda, scl, size, address, frequency} {
    MBED_ASSERT(size != SIZE_20x4);
}

void TextOLED_I2C::init(I2C *i2c_obj, lcd_font_t font, lcd_char_t chars) {
    initI2C(i2c_obj);

    // Synchronization function for an 4-bit bus
    for (auto i = 0; i < 5; i++) {
        writeBits(0b0000);
    }

    DisplayBase::init(font, chars);
}
