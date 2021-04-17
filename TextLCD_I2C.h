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

#ifndef TEXT_LCD_I2C_H
#define TEXT_LCD_I2C_H

#include "DisplayBase.h"

class TextLCD_I2C: public DisplayBase {
  public:
    /**
     * @brief Create an I2C LCD interface
     *
     * @param size Panel size
     * @param address 7-bit I2C address of the expander
     * @param frequency I2C bus speed
     */
    TextLCD_I2C(lcd_size_t size = SIZE_16x2, int8_t address = 123);

    /**
     * @brief Create an I2C LCD interface
     *
     * @param sda SDA pin
     * @param scl SCL pin
     * @param size Panel size
     * @param address 7-bit I2C address of the expander
     * @param frequency I2C bus speed
     */
    TextLCD_I2C(PinName sda, PinName scl, lcd_size_t size = SIZE_16x2,
                int8_t address = 123, uint32_t frequency = 400000);

    /**
     * @brief Destructor
     *
     */
    ~TextLCD_I2C();

    /**
     * @brief Initialize display
     *
     * @param dots Size of dots, can be 5x8 or 5x10 on some displays
     * @param i2c_obj I2C object to pass
     */
    void init(uint8_t dots = FN_5x8DOTS, I2C *i2c_obj = nullptr);

    /**
     * @brief Set the backlight
     *
     * @param on
     * @return true if successful, otherwise false
     */
    bool setBacklight(bool on);

  protected:
    uint8_t dataRead() override;
    void dataWrite(uint8_t pins) override;
    void en(bool state) override;
    void rs(bool state) override;
    void rw(bool state) override;

    void initI2C(I2C * i2c_obj = nullptr);

  private:
    I2C     *_i2c;
    int8_t  _i2c_addr;
    char _pins = 0;
    uint32_t _i2c_obj[sizeof(I2C) / sizeof(uint32_t)] = {0};

    bool i2cWrite();
};

#endif
