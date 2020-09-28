/* *******************************************************************
  cherry7seg Display Board Library
  
  copyright (c) Transtak Pte Ltd, Cherry Plaza Online Shop
  
  Version 1.0.0    06/10/2020
  
 * ******************************************************************/

#pragma once
#include <Wire.h>
#include <Print.h>
#include <Arduino.h>

#if defined(__AVR__)
#include <avr/pgmspace.h>
#elif defined(ESP8266)
#include <pgmspace.h>
#endif

/* *******************************************************************
        Settings
 * ******************************************************************/

#define HT16K33_DEBUG 0                // Library debug messages: 0 off    1 important/error     2 warning    3 info/debug   (mode 1 costs about 230 byte)

/* *******************************************************************
         no need to change something below here
 * ******************************************************************/
#define HT16K33_OSCILATOR_ON    0x21   // System Setup Register 0x20 + 0x01
#define HT16K33_CMD_BRIGHTNESS  0xE0   // Dimming set (Datasheet says EF, but this is just with the set bits D12-D8 for full brightness
#define HT16K33_BLINK_CMD       0x80   // Display Setup - blink and on/off
#define HT16K33_BLINK_DISPLAYON 0x01
#define HT16K33_BLINK_OFF       0
#define HT16K33_BLINK_2HZ       1
#define HT16K33_BLINK_1HZ       2
#define HT16K33_BLINK_HALFHZ    3

// Error Codes
#define HT16K33_ERR_ADDR _BV(2)        // chip not responding at adress 0b00000100 = 4

/* *******************************************************************
         HW Definitions of a bitmask for each LED segment:
 * ******************************************************************/
// 7 Segment
// This is the most common segment definition
// match LED segment of display to hardware
//                       .GFEDCBA
const uint8_t SEG_A  = 0b00000001;
const uint8_t SEG_B  = 0b00000010;
const uint8_t SEG_C  = 0b00000100;
const uint8_t SEG_D  = 0b00001000;
const uint8_t SEG_E  = 0b00010000;
const uint8_t SEG_F  = 0b00100000;
const uint8_t SEG_DP = 0b10000000;
const uint8_t SEG_G  = 0b01000000;



/* *******************************************************************
         Characterset for 7 segment
 * ******************************************************************/

const static uint8_t charTable [] PROGMEM  = {
  0,                                                       //     32   space
  SEG_B | SEG_C | SEG_DP,                                  // !   33
  SEG_B | SEG_F,                                           // "   34
  0,                                                       // #   35
  SEG_A | SEG_C | SEG_D | SEG_F | SEG_G,                   // $   36
  SEG_A | SEG_B | SEG_F | SEG_G,                           // %   37
  0,                                                       // &   38
  SEG_B,                                                   // '   39
  SEG_A | SEG_D | SEG_E | SEG_F,                           // (   40
  SEG_A | SEG_B | SEG_C | SEG_D,                           // )   41
  0,                                                       // *   42   no character on 7segment
  0,                                                       // +   43   no character on 7segment
  0,                                                       // ,   44   will be handled in the write methode
  SEG_G,                                                   // -   45
  0,                                                       // .   46   will be handled in the write methode
  SEG_B | SEG_E | SEG_G ,                                  // /   47
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,           // 0   48
  SEG_B | SEG_C,                                           // 1   49
  SEG_A | SEG_B | SEG_D | SEG_E | SEG_G,                   // 2   50
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_G,                   // 3   51
  SEG_B | SEG_C | SEG_F | SEG_G,                           // 4   52
  SEG_A | SEG_C | SEG_D | SEG_F | SEG_G,                   // 5   53
  SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,           // 6   54
  SEG_A | SEG_B | SEG_C,                                   // 7   55
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,   // 8   56
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G,           // 9   57
  0,                                                       // :   58   will be handled in the write methode
  0,                                                       // ;   59   will be handled in the write methode
  SEG_D | SEG_E | SEG_G,                                   // <   60
  SEG_G,                                                   // =   61
  SEG_C | SEG_D | SEG_G,                                   // >   62
  SEG_A | SEG_B | SEG_E | SEG_G,                           // ?   63
  0,                                                       // @   64
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,           // A   65
  SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,                   // B
  SEG_A | SEG_D | SEG_E | SEG_F,                           // C
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,                   // D
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,                   // E
  SEG_A | SEG_E | SEG_F | SEG_G,                           // F
  SEG_A | SEG_C | SEG_D | SEG_E | SEG_F,                   // G
  SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,                   // H
  SEG_B | SEG_C,                                           // I
  SEG_B | SEG_C | SEG_D | SEG_E,                           // J
  SEG_A | SEG_C | SEG_E | SEG_F | SEG_G,                   // K
  SEG_D | SEG_E | SEG_F,                                   // L
  SEG_A | SEG_C | SEG_E,                                   // M
  SEG_C | SEG_E | SEG_G,                                   // N
  SEG_C | SEG_D | SEG_E | SEG_G,                           // O
  SEG_A | SEG_B | SEG_E | SEG_F | SEG_G,                   // P
  SEG_A | SEG_B | SEG_C | SEG_F | SEG_G,                   // Q
  SEG_E | SEG_G,                                           // R
  SEG_A | SEG_C | SEG_D | SEG_F | SEG_G,                   // S
  SEG_D | SEG_E | SEG_F | SEG_G,                           // T
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,                   // U
  SEG_C | SEG_D | SEG_E,                                   // V
  SEG_B | SEG_D | SEG_F,                                   // W
  SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,                   // X
  SEG_B | SEG_C | SEG_D | SEG_F | SEG_G,                   // Y
  SEG_A | SEG_B | SEG_D | SEG_E | SEG_G,                   // Z   90
  SEG_A | SEG_D | SEG_E | SEG_F,                           // [   91
  SEG_C | SEG_F | SEG_G,                                   /* \   92 backslash*/
  SEG_A | SEG_B | SEG_C | SEG_D,                           // ]   93
  SEG_A,                                                   // ^   94
  SEG_D,                                                   // _   95 underscore
  SEG_B,                                                   // `   96
  SEG_C | SEG_D | SEG_E | SEG_G | SEG_DP,                  // a   97
  SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,                   // b   98
  SEG_D | SEG_E | SEG_G,                                   // c   99
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,                   // d   100
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,                   // e   101
  SEG_A | SEG_E | SEG_F | SEG_G,                           // f   102
  SEG_A | SEG_C | SEG_D | SEG_E | SEG_F,                   // g G 103 capital letter will be used
  SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,                   // h   104
  SEG_C,                                                   // i   105
  SEG_C | SEG_D,                                           // j   106
  SEG_A | SEG_C | SEG_E | SEG_F | SEG_G,                   // k   107
  SEG_E | SEG_F,                                           // l   108
  SEG_A | SEG_C | SEG_E,                                   // m n 109 n will be used
  SEG_C | SEG_E | SEG_G,                                   // n   110
  SEG_C | SEG_D | SEG_E | SEG_G,                           // o   111
  SEG_A | SEG_B | SEG_E | SEG_F | SEG_G,                   // p P 112
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_DP,          // q Q 113
  SEG_E | SEG_G,                                           // r   114
  SEG_A | SEG_C | SEG_D | SEG_F | SEG_G,                   // s S 115
  SEG_D | SEG_E | SEG_F | SEG_G,                           // t   116
  SEG_C | SEG_D | SEG_E,                                   // u   117
  SEG_C | SEG_D | SEG_E,                                   // v u 118 u will be used
  SEG_C | SEG_D | SEG_E,                                   // w u 119 u will be used
  SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,                   // x   120
  SEG_B | SEG_C | SEG_D | SEG_F | SEG_G,                   // y Y 121
  SEG_A | SEG_B | SEG_D | SEG_E | SEG_G,                   // z Z 122
  SEG_A | SEG_D | SEG_E | SEG_F,                           // {   123
  SEG_B | SEG_C,                                           // |   124
  SEG_A | SEG_B | SEG_C | SEG_D,                           // }   125
  SEG_G,                                                   // ~   126
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G | SEG_DP  //   127 all segments
};


/* *******************************************************************
   Classes
 * ******************************************************************/

 class cherry7seg_ht16k33 : public Print{                               // Base class for all HT16K33 displays
  public:
    cherry7seg_ht16k33(void);                                 
    uint8_t begin(uint8_t i2c_addr, uint8_t numDevices = 1);         // define the (start) I2C adress of the display and the number of used devices
    void blinkRate(uint8_t b);                                       // set the blink rate of the display
    void clear(void);                                                // clear display (I2C) and displaybuffer
    void off(void);                                                  // turn the display off
    void on(void);                                                   // turn the display on
    bool isConnected(void);                                          // check if all ICs are responding
    void setBrightness(uint8_t b);                                   // set the brightness of the display 
    void setCursor(uint8_t newPosition);                             // set the cursor for the next writing position
    void setDigits(uint8_t newDigits);                               // set the number of digits per device
    void writeLowLevel(uint8_t position, uint16_t bitmask);          // I2C lowLevel write of a bitmask to the IC to a specific position
    //size_t write(uint8_t);                                         // write method is depending on the HW and therefore in the first HW child classes
    //using Print::write;                                            
#ifdef HT16K33_BUFFERSIZE                                            
    void writeChar(uint8_t device, uint16_t digit);                  // I2C Ausgabe eines Zeichen aus Displaybuffer
#endif

	// not supported LCD API 1.0 functions
	// ===================================
	// these function are defined in the LCD API 1.0 API, 
  // but as this is not a LCD library, these functions are marked as deprecated
	// note: only very new versions of gcc support setting warning message
	// it breaks on on older versions that shipped with older 1.x IDEs
	// so test for gcc 4.5 or greater for better deprecated messages

#if ( __GNUC__ >= 4) && (__GNUC_MINOR__ >= 5)
	inline void __attribute__((deprecated("Use clear() instead")))
	 home() {clear();}
  inline void __attribute__((deprecated("Use begin() instead"))) 
   init(uint8_t i2c_addr, uint8_t numDevices = 1) {begin(i2c_addr, numDevices);}
#else
	inline void __attribute__((deprecated))
	 home() {clear();}
  inline void __attribute__((deprecated))
	 init(uint8_t i2c_addr, uint8_t numDevices = 1) {begin(i2c_addr, numDevices);}
#endif

  protected:
    uint8_t _hasColonDigit;                                // is set to 1 if display has a separate colon digit at digit 2 (the third column)
    uint8_t _numDigits = 8;                                // digits per device
    uint8_t _i2c_addr;                                     // base adress of first device (0)
    uint8_t _numDevices;                                   // total number of devices
#ifdef HT16K33_BUFFERSIZE
    uint16_t displaybuffer[HT16K33_BUFFERSIZE];            // stores printed bitmaps
    uint8_t _currentBufferPosition;                        // current read position in buffer
#endif
    uint8_t _currentPosition;                              // current position of cursor
    uint8_t _lastPosition;                                 // last position of cursor
};


class cherry7seg_ht16k33_hw_7 : public cherry7seg_ht16k33 {      // 7 segment display
  public:
    cherry7seg_ht16k33_hw_7();
    size_t write(uint8_t);                                 // write using a byte char table
    using Print::write;                                    // 
  protected:
    uint8_t _lastBitmap;                                   // last written bitmap (if we have to reprint for the decimal point)
};


class cherry7seg_ht16k33_hw_7_4_c : public cherry7seg_ht16k33_hw_7 {       // 7 Segment Display with 4 visible Digits and Colon Sign. In fact this display has 5 colons
  public:
    cherry7seg_ht16k33_hw_7_4_c();
};


