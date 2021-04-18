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

#ifndef DISPLAY_BASE_H
#define DISPLAY_BASE_H

#include "mbed.h"
#include "Stream.h"

class DisplayBase : public Stream {
  public:
    enum lcd_size_t {
        SIZE_8x2,  // 8x2 panel
        SIZE_16x2, // 16x2 panel
        SIZE_20x2, // 20x2 panel
        SIZE_20x4, // 20x4 panel
        SIZE_40x2  // 40x2 panel
    };

    enum lcd_mode_t {
        DISPLAY_ON,    // Turn the display ON
        DISPLAY_OFF,   // Turn the display OFF
        CURSOR_ON,     // Turns the underline cursor ON
        CURSOR_OFF,    // Turns the underline cursor OFF
        BLINK_ON,      // Turn the blinking cursor ON
        BLINK_OFF,     // Turn the blinking cursor OFF
        SCROLL_LEFT,   // These command scroll the display without changing the RAM
        SCROLL_RIGHT,  // These commands scroll the display without changing the RAM
        LEFT_TO_RIGHT, // This is for text that flows Left to right
        RIGHT_TO_LEFT, // This is for text that flows right to left
        SCROLL_ON,     // This will 'right justify' text from the cursor
        SCROLL_OFF     // This will 'left justify' text from the cursor
    };

    enum lcd_char_t {
        CHAR_5X10 = 0b100, // F
        CHAR_5X8  = 0 // F
    };

    enum lcd_font_t {
        FONT_JAPANESE    = 0b00, // (FT1 = 0, FT0 = 0)
        FONT_EUROPEAN_I  = 0b01, // (FT1 = 0, FT0 = 1)
        FONT_RUSSIAN     = 0b10, // (FT1 = 1, FT0 = 0)
        FONT_EUROPEAN_II = 0b11 // (FT1 = 1, FT0 = 1)
    };

    /**
     * @brief Create an interface

     * @param size  Panel size
     * @param bf  Set to true if busy flag should be used
     */
    DisplayBase(lcd_size_t size, bool bf);

    /**
     * @brief Clear the screen and locate to 0,0
     *
     */
    void cls();

    /**
     * @brief Locate to a screen column and row
     *
     * @param column  The horizontal position from the left, indexed from 0
     * @param row     The vertical position from the top, indexed from 0
     */
    void locate(uint8_t column, uint8_t row);

    /**
     * @brief Set cursor position to zero
     *
     */
    void home();

    /**
     * @brief Set display modes
     *
     * @param mode
     */
    void display(lcd_mode_t mode);

    /**
     * @brief Create a user defined char object
     * Allows us to fill the first 8 CGRAM locations with custom characters
     *
     * @param location index 0-7
     * @param charmap data with custom character
     */
    void create(uint8_t location, uint8_t charmap[]);

    /**
     * @brief Writes a single char to a given position, usefull for UDC
     *
     * @param column
     * @param row
     * @param c character
     */
    void character(uint8_t column, uint8_t row, uint8_t c);

    /**
     * @brief Get number of rows
     *
     * @return row count
     */
    uint8_t rows();

    /**
     * @brief Get number of columns
     *
     * @return column count
     */
    uint8_t columns();

  protected:
    enum lcd_command_t {
        CMD_CLEAR_DISPLAY   = 0b1,
        CMD_RETURN_HOME     = 0b10,
        CMD_ENTRY_MODE_SET  = 0b100,
        CMD_DISPLAY_CONTROL = 0b1000,
        CMD_CURSOR_SHIFT    = 0b10000,
        CMD_FUNCTION_SET    = 0b100000,
        CMD_SET_CGRAM_ADDR  = 0b1000000,
        CMD_SET_DDRAM_ADDR  = 0b10000000,
    };

    enum lcd_control_t {
        CTRL_DISPLAY_ON            = 0b100, // D
        CTRL_DISPLAY_OFF           = 0, // D
        CTRL_CURSOR_ON             = 0x02, // C
        CTRL_CURSOR_OFF            = 0, // C
        CTRL_BLINK_ON              = 0x01, // B
        CTRL_BLINK_OFF             = 0, // B
    };

    enum lcd_function_t {
        FN_8BIT_MODE        = 0b10000, // DL
        FN_4BIT_MODE        = 0, // DL
        FN_2LINE            = 0b1000, // N
        FN_1LINE            = 0, // N
    };

    enum lcd_entry_mode_t {
        ENTRY_MODE_SHIFT_LEFT  = 0b1, // S
        ENTRY_MODE_SHIFT_RIGHT = 0, // S
        ENTRY_MODE_INCREMENT   = 0b10, // I/D
        ENTRY_MODE_DECREMENT   = 0, // I/D
    };

    enum lcd_shift_t {
        DISPLAY_MOVE = 0b1000, // S/C
        CURSOR_MOVE  = 0,
        MOVE_RIGHT   = 0b100, // RL
        MOVE_LEFT    = 0,
    };

    /**
     * @brief Initialize the display
     * 
     * @param font Font table used
     * @param chars Size of 1 character, can be 5x8 or 5x10 on some displays
     */
    void init(lcd_font_t font, lcd_char_t chars);

    /**
     * @brief Write command (with wait or busy flag)
     *
     * @param command
     */
    void writeCommand(uint8_t command);

    /**
     * @brief Write data (with wait or busy flag)
     *
     * @param data
     */
    void writeData(uint8_t data);

    /**
     * @brief Write 4 bits
     *
     * @param value
     */
    void writeBits(uint8_t value);

    /**
     * @brief Write byte
     *
     * @param value
     */
    void writeByte(uint8_t value);

    /**
     * @brief Wait for instruction completion
     *
     */
    void waitReady();

  private:
    const lcd_size_t _type = SIZE_16x2;
    bool _bf = false;

    uint8_t _control = CTRL_DISPLAY_OFF | CTRL_CURSOR_OFF | CTRL_BLINK_OFF;
    uint8_t _entry_mode = ENTRY_MODE_INCREMENT | ENTRY_MODE_SHIFT_RIGHT;

    uint8_t _column = 0;
    uint8_t _row = 0;

    // Stream implementation functions
    int _putc(int value);
    int _getc();

    void pulseEnable();
    uint8_t getAddress(uint8_t column, uint8_t row);

    virtual uint8_t dataRead() = 0;
    virtual void dataWrite(uint8_t pins) = 0;
    virtual void dataInput() = 0;
    virtual void dataOutput() = 0;
    virtual void en(bool state) = 0;
    virtual void rs(bool state) = 0;
    virtual void rw(bool state) = 0;
};

#endif
