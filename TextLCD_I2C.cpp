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

#include "TextLCD_I2C.h"

TextLCD_I2C::TextLCD_I2C(lcd_size_t size, int8_t address):
    DisplayBase{size, true} {

    dataWrite(0);
    en(0);
    rw(1);
}

TextLCD_I2C::TextLCD_I2C(PinName sda, PinName scl, lcd_size_t size, int8_t address,
                         uint32_t frequency):
    DisplayBase{size, true} {

    dataWrite(0);
    en(0);
    rw(1);

    _i2c = new (_i2c_obj) I2C(sda, scl);
    _i2c->frequency(frequency);
}

TextLCD_I2C::~TextLCD_I2C() {
    if (_i2c == reinterpret_cast<I2C *>(_i2c_obj)) {
        _i2c->~I2C();
    }
}

void TextLCD_I2C::initI2C(I2C *i2c_obj) {
    if (i2c_obj != nullptr) {
        _i2c = i2c_obj;
    }

    MBED_ASSERT(_i2c);
}

void TextLCD_I2C::init(I2C *i2c_obj, lcd_char_t chars) {
    initI2C(i2c_obj);

    // Function Set
    writeBits(0b11); // 8-bit mode
    ThisThread::sleep_for(5ms); // minimum 4.1ms

    writeBits(0b0011); // 8-bit mode
    wait_us(120); // minimum 100us

    // Function Set
    writeBits(0b0011); // 8-bit mode

    DisplayBase::init(FONT_JAPANESE, chars);
}

uint8_t TextLCD_I2C::dataRead() {
    int32_t ack;
    char buf[1];

    _i2c->lock();
    ack = _i2c->read(_i2c_addr, buf, 1);
    _i2c->unlock();

    if (ack != 0) {
        return 0;
    }

    return buf[0] >> 4;
}

void TextLCD_I2C::dataWrite(uint8_t pins) {
    _pins &= ~0b11110000;
    _pins |= (pins & 0b1111) << 4;

    i2cWrite();
}

void TextLCD_I2C::en(bool state) {
    _pins &= ~0b100;
    _pins |= state << 2;

    i2cWrite();
}

void TextLCD_I2C::rs(bool state) {
    _pins &= ~0b1;
    _pins |= state;

    i2cWrite();
}

void TextLCD_I2C::rw(bool state) {
    _pins &= ~0b10;
    _pins |= state << 1;

    i2cWrite();
}

void TextLCD_I2C::setBacklight(bool on) {
    _pins &= ~0b1000;
    _pins |= on << 3;

    i2cWrite();
}

bool TextLCD_I2C::i2cWrite() {
    int32_t ack;

    _i2c->lock();
    ack = _i2c->write(_i2c_addr, &_pins, 1);
    _i2c->unlock();

    if (ack != 0) {
        return false;
    }

    return true;
}