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

#include "DisplayBase.h"

DisplayBase::DisplayBase(lcd_size_t type, bool bf):
    _type(type), _bf(bf) {
}

void DisplayBase::init(uint8_t dots, uint8_t chars) {
    // Function Set
    for (auto i = 0; i < 2; i++) {
        writeBits(0b0010); // 4-bit mode
    }

    writeBits((rows() == 2 ? FN_2LINE : FN_1LINE) | dots | chars);
    waitReady();

    // Display ON/OFF Control
    writeCommand(CMD_DISPLAY_CONTROL | CTRL_DISPLAY_OFF | CTRL_CURSOR_OFF | CTRL_BLINK_OFF);

    // Display Clear
    cls();

    // Return Home
    home();

    // Entry Mode Set
    writeCommand(CMD_ENTRY_MODE_SET | ENTRY_MODE_DECREMENT | ENTRY_MODE_SHIFT_RIGHT);
}

void DisplayBase::character(uint8_t column, uint8_t row, uint8_t c) {
    uint8_t addr = getAddress(column, row);
    writeCommand(addr);
    writeData(c);
}

void DisplayBase::cls() {
    locate(0, 0);
    writeByte(CMD_CLEAR_DISPLAY);

    if (_bf) {
        ThisThread::sleep_for(6ms);
        waitReady();

    } else {
        ThisThread::sleep_for(7ms); // minimum 6.2ms
    }
}

void DisplayBase::locate(uint8_t column, uint8_t row) {
    _column = column;
    _row = row;
}

void DisplayBase::home() {
    writeByte(CMD_RETURN_HOME);

    if (_bf) {
        ThisThread::sleep_for(1ms);
        waitReady();

    } else {
        ThisThread::sleep_for(2ms); // minimum 1.52ms
    }
}

void DisplayBase::display(lcd_mode_t mode) {
    switch (mode) {
        case DISPLAY_ON :
            _control |= CTRL_DISPLAY_ON;
            writeCommand(CMD_DISPLAY_CONTROL | _control);
            break;

        case DISPLAY_OFF:
            _control &= ~CTRL_DISPLAY_ON;
            writeCommand(CMD_DISPLAY_CONTROL | _control);
            break;

        case CURSOR_ON:
            _control |= CTRL_CURSOR_ON;
            writeCommand(CMD_DISPLAY_CONTROL | _control);
            break;

        case CURSOR_OFF:
            _control &= ~CTRL_CURSOR_ON;
            writeCommand(CMD_DISPLAY_CONTROL | _control);
            break;

        case BLINK_ON:
            _control |= CTRL_BLINK_ON;
            writeCommand(CMD_DISPLAY_CONTROL | _control);
            break;

        case BLINK_OFF:
            _control &= ~CTRL_BLINK_ON;
            writeCommand(CMD_DISPLAY_CONTROL | _control);
            break;

        case SCROLL_LEFT:
            writeCommand(CMD_CURSOR_SHIFT | DISPLAY_MOVE | MOVE_LEFT);
            break;

        case SCROLL_RIGHT:
            writeCommand(CMD_CURSOR_SHIFT | DISPLAY_MOVE | MOVE_RIGHT);
            break;

        case LEFT_TO_RIGHT:
            _entry_mode |= ENTRY_MODE_SHIFT_LEFT;
            writeCommand(CMD_ENTRY_MODE_SET | _entry_mode);
            break;

        case RIGHT_TO_LEFT:
            _entry_mode &= ~ENTRY_MODE_SHIFT_LEFT;
            writeCommand(CMD_ENTRY_MODE_SET | _entry_mode);
            break;

        case SCROLL_ON:
            _entry_mode |= ENTRY_MODE_INCREMENT;
            writeCommand(CMD_ENTRY_MODE_SET | _entry_mode);
            break;

        case SCROLL_OFF:
            _entry_mode &= ~ENTRY_MODE_INCREMENT;
            writeCommand(CMD_ENTRY_MODE_SET | _entry_mode);
            break;
    }
}

void DisplayBase::create(uint8_t location, uint8_t charmap[]) {
    if (location > 7) { // only 8 locations available
        return;
    }

    writeCommand(CMD_SET_CGRAM_ADDR | (location << 3));

    for (auto i = 0; i < 8; i++) {
        writeData(charmap[i]);
    }
}

int DisplayBase::_putc(int value) {
    if (value == '\n') {
        _column = 0;
        _row++;

        if (_row >= rows()) {
            _row = 0;
        }

    } else {
        character(_column, _row, value);
        _column++;

        if (_column >= columns()) {
            _column = 0;
            _row++;

            if (_row >= rows()) {
                _row = 0;
            }
        }
    }

    return value;
}

int DisplayBase::_getc() {
    return -1;
}

uint8_t DisplayBase::getAddress(uint8_t column, uint8_t row) {
    switch (_type) {
        case SIZE_20x4:
            switch (row) {
                case 0:
                    return 0x80 + column;

                case 1:
                    return 0xc0 + column;

                case 2:
                    return 0x94 + column;

                case 3:
                    return 0xd4 + column;

                default:
                    return -1;
            }

        default:
            return 0x80 + (row * 0x40) + column;
    }
}

uint8_t DisplayBase::columns() {
    switch (_type) {
        case SIZE_20x4:
        case SIZE_20x2:
        case SIZE_40x2:
            return 20;

        case SIZE_8x2:
            return 8;

        default:
            return 16;
    }
}

uint8_t DisplayBase::rows() {
    switch (_type) {
        case SIZE_20x4:
            return 4;

        default:
            return 2;
    }
}

void DisplayBase::writeCommand(uint8_t command) {
    rs(0);
    writeByte(command);

    waitReady();
}

void DisplayBase::writeData(uint8_t data) {
    rs(1);
    writeByte(data);

    waitReady();
}

void DisplayBase::writeBits(uint8_t value) {
    rs(0);

    rw(0);

    dataWrite(value & 0b1111);
    pulseEnable();
}

void DisplayBase::writeByte(uint8_t value) {
    rw(0);

    dataWrite(value >> 4); // send upper part first
    pulseEnable();
    dataWrite(value & 0b1111);
    pulseEnable();
}

void DisplayBase::pulseEnable() {
    en(0);
    wait_us(2);
    en(1);
    wait_us(2);
    en(0);
    wait_us(40);
}

void DisplayBase::waitReady() {
    if (_bf) {
        dataInput();
        rs(0);
        rw(1);

        while (1) {
            en(0);
            wait_us(1);
            en(1);

            wait_us(10);

            bool busy = dataRead() & 0b1000;

            en(0);

            pulseEnable();

            if (!busy) {
                break;
            }
        }

        dataOutput();
        rw(0);

    } else {
        wait_us(40); // minimum 37us
    }
}
