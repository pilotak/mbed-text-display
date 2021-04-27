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

#define TEXT_DISPLAY_I2C_ADDRESS (0x27 << 1)

#include "DisplayBase.h"

class TextLCD_I2C: public DisplayBase {
  public:
    /**
     * @brief Create an I2C LCD interface
     *
     * @param alt_pinmap PCF8574 altternative pin maping
     * @param size Panel size
     * @param address 7-bit I2C address of the expander
     * @param frequency I2C bus speed
     */
    TextLCD_I2C(bool alt_pinmap = false, lcd_size_t size = SIZE_16x2,
                int8_t address = TEXT_DISPLAY_I2C_ADDRESS);

    /**
     * @brief Create an I2C LCD interface
     *
     * @param sda SDA pin
     * @param scl SCL pin
     * @param alt_pinmap PCF8574 altternative pin maping
     * @param size Panel size
     * @param address 7-bit I2C address of the expander
     * @param frequency I2C bus speed
     */
    TextLCD_I2C(PinName sda, PinName scl, bool alt_pinmap = false, lcd_size_t size = SIZE_16x2,
                int8_t address = TEXT_DISPLAY_I2C_ADDRESS, uint32_t frequency = 100000);

    /**
     * @brief Destructor
     *
     */
    ~TextLCD_I2C();

    /**
     * @brief Initialize display
     *
     * @param i2c_obj I2C object to pass
     * @param chars Size of 1 character, can be 5x8 or 5x10 on some displays
     *
     * @return true if success, false otherwise
     */
    bool init(I2C *i2c_obj = nullptr, lcd_char_t chars = CHAR_5X8);

    /**
     * @brief Set the backlight
     *
     * @param on
     */
    void setBacklight(bool on);

  protected:
    uint8_t dataRead() override;
    void dataWrite(uint8_t pins) override;
    void en(bool state) override;
    void rs(bool state) override;
    void rw(bool state) override;

    void initI2C(I2C *i2c_obj = nullptr);

  private:
    I2C *_i2c;
    const int8_t _i2c_addr;
    const bool _alt_pinmap = false;
    char _pins = 0;
    uint32_t _i2c_obj[sizeof(I2C) / sizeof(uint32_t)] = {0};

    bool i2cWrite();

    void dataInput() {};
    void dataOutput() {};
};

#endif
