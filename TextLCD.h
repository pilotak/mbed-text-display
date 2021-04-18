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

#ifndef TEXT_LCD_H
#define TEXT_LCD_H

#include "DisplayBase.h"

class TextLCD: public DisplayBase {
  public:
    /**
     * @brief Create an LCD interface
     *
     * @param rs    Instruction/data control pin
     * @param e     Chip enable signal pin
     * @param d4-d7 Data line pins
     * @param rw    Read/write pin
     * @param size  Panel size
     */
    TextLCD(PinName rs, PinName en, PinName d4, PinName d5, PinName d6, PinName d7, PinName rw = NC,
            lcd_size_t size = SIZE_16x2);

    /**
     * @brief Destructor
     *
     */
    ~TextLCD();

    /**
     * @brief Initialize display
     *
     * @param chars Size of 1 character, can be 5x8 or 5x10 on some displays
     */
    void init(lcd_char_t chars = CHAR_5X8);

  protected:
    uint8_t dataRead() override;
    void dataWrite(uint8_t pins) override;
    void dataInput() override;
    void dataOutput() override;
    void en(bool state) override;
    void rs(bool state) override;
    void rw(bool state) override;

  private:
    DigitalOut _rs;
    DigitalOut _en;
    DigitalOut *_rw;
    BusInOut _data;
};

#endif
