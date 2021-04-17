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

#include "TextOLED.h"

TextOLED::TextOLED(PinName rs, PinName en, PinName d4, PinName d5, PinName d6, PinName d7, PinName rw, lcd_size_t type):
    TextLCD{rs, en, d4, d5, d6, d7, rw, type} {
    MBED_ASSERT(type != SIZE_20x4);
}

void TextOLED::init(uint8_t dots, uint8_t chars) {
    // Synchronization function for an 4-bit bus
    for (auto i = 0; i < 5; i++) {
        writeBits(0b0000);
    }

    DisplayBase::init(dots, chars);
}
