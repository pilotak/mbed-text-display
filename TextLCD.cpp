/*
MIT License
Copyright (c) 2007-2010, sford, http://mbed.org
Copyright (c) 2020, sstaub
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

#include "mbed.h"
#include "TextLCD.h"

TextLCD::TextLCD(PinName rs, PinName en, PinName d4, PinName d5, PinName d6, PinName d7, PinName rw, lcd_size_t type):
    DisplayBase{type, (rw != NC)}, _rs(rs), _en(en), _data(d4, d5, d6, d7) {

    _data.output();
    _data.write(0);
    _en.write(0);

    if (rw != NC) {
        _rw = new DigitalOut(rw);
        _rw->write(1);
    }
}

TextLCD::~TextLCD() {
    if (_rw) {
        delete _rw;
    }
}

void TextLCD::init(uint8_t dots) {
    // Function Set
    writeBits(0b11); // 8-bit mode
    ThisThread::sleep_for(5ms); // minimum 4.1ms

    writeBits(0b0011); // 8-bit mode
    wait_us(120); // minimum 100us

    // Function Set
    writeBits(0b0011); // 8-bit mode

    DisplayBase::init(dots, FN_FONT_JAPANESE);
}

uint8_t TextLCD::dataRead() {
    return _data.read();
}

void TextLCD::dataWrite(uint8_t pins) {
    _data.write(pins);
}

void TextLCD::dataInput() {
    _data.input();
}

void TextLCD::dataOutput() {
    _data.output();
}

void TextLCD::en(bool state) {
    _en.write(state);
}

void TextLCD::rs(bool state) {
    _rs.write(state);
}

void TextLCD::rw(bool state) {
    if (_rw) {
        _rw->write(state);
    }
}