/********************************************************************
  cherry7seg Display Board Library
  
  copyright (c) Transtak Pte Ltd, Cherry Plaza Online Shop
  
  Version 1.0.0    06/10/2020
 *******************************************************************/
#include "cherry7seg.h"

cherry7seg_ht16k33::cherry7seg_ht16k33()
{
  
}


uint8_t cherry7seg_ht16k33::begin(uint8_t i2c_addr,  uint8_t numDevices  /* = 1 */) {
  uint8_t result = 0;    // 0 = success
  _i2c_addr = i2c_addr;
  _numDevices = numDevices;
  if (_numDevices > 8) _numDevices = 1;
#if HT16K33_DEBUG > 2
  Serial.println(F("D012 begin base"));
  //Serial.print(F("D013 numDevices=")); Serial.println(numDevices);
#endif
#if defined(__AVR__)
  if (TWCR == 0) {
    Wire.begin();                                // only call when not started before
#if HT16K33_DEBUG >= 2
  Serial.println(F("W019 add Wire.begin() to your setup"));
#endif
  }
#endif
  for (uint8_t i = _i2c_addr; i < _i2c_addr + _numDevices; i++)
  {
    Wire.beginTransmission(i);
    Wire.write(HT16K33_OSCILATOR_ON);            // turn on oscillator
    if (Wire.endTransmission())
    {
      result = HT16K33_ERR_ADDR;
#if HT16K33_DEBUG > 0
      Serial.println(F("E032 I2C error for adress 0x"));
      Serial.println(i, HEX);
#endif
    }
    blinkRate(HT16K33_BLINK_OFF);
    setBrightness(15);                           // max brightness
  }
  return result;
}


void cherry7seg_ht16k33::blinkRate(uint8_t b) {
  for (uint8_t i = 0; i < _numDevices; i++)
  {
    Wire.beginTransmission(_i2c_addr + i);
    if (b > 3) b = 0;                            // turn blink off if not sure
    Wire.write(HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON | (b << 1));
    Wire.endTransmission();
  }
}

void cherry7seg_ht16k33::clear(void) {                                 // Clears displaybuffer and Display (I2C)
#ifdef HT16K33_BUFFERSIZE
  for (uint8_t i = 0; i < HT16K33_BUFFERSIZE; i++) {
    displaybuffer[i] = 0;
  }
#endif
  for (uint8_t i = 0; i < _numDevices; i++)
  {
    Wire.beginTransmission(_i2c_addr + i);
    for (uint8_t j = 0; j < 16; j++) {                               // for all 8 positions per device (16 adresses)
      Wire.write(0);
    }
    Wire.endTransmission();
  }
  _currentPosition = 0;
}

/*
   check if all ICs are responding
   return true on success
*/
bool cherry7seg_ht16k33::isConnected()                                 
{
  for (uint8_t i = _i2c_addr; i < _i2c_addr + _numDevices; i++) {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() != 0) // 0 if something ack's at this address
    {
#if HT16K33_DEBUG >= 1
      Serial.print(F("E081 IC 0x"));
      Serial.print(i, HEX);
      Serial.println(F(" not responding"));
#endif
      return false; // exit on first not responding display
    }
  }
  return true;
}


/*
   turn the display off
*/
void cherry7seg_ht16k33::off(void)
{
  for (uint8_t i = 0; i < _numDevices; i++)
  {
    Wire.beginTransmission(_i2c_addr + i);
    Wire.write(HT16K33_BLINK_CMD);
    Wire.endTransmission();
  } 
}


/*
   turn the display on
*/
void cherry7seg_ht16k33::on(void)
{
   blinkRate(HT16K33_BLINK_OFF);
}


/*
   set the brightness of the display 
*/
void cherry7seg_ht16k33::setBrightness(uint8_t b) {
  if (b > 15) b = 15;
  for (uint8_t i = 0; i < _numDevices; i++) {
    Wire.beginTransmission(_i2c_addr + i);
    Wire.write(HT16K33_CMD_BRIGHTNESS | b);
    Wire.endTransmission();
  }
}


/*
  set the cursor for the next writing position
*/
void cherry7seg_ht16k33::setCursor(uint8_t newPosition) {               // Set the cursor for the next writing position
  _currentPosition = newPosition;
}


/*
  set the number of digits per device
*/
void cherry7seg_ht16k33::setDigits(uint8_t newDigits)
{
  _numDigits = newDigits;
}


#ifdef HT16K33_BUFFERSIZE
void cherry7seg_ht16k33::writeChar(uint8_t device, uint16_t digit) {     // I2C Ausgabe eines Zeichen aus Displaybuffer
  device = digit / _numDigits;
  uint8_t reg = (digit % _numDigits) * 2;
  uint8_t i2c_addr = _i2c_addr + device;
  Wire.beginTransmission(i2c_addr);
  Wire.write(reg);                         // Register  (uint8_t)digit * 2
  Wire.write(displaybuffer[digit] & 0xFF); // nibble
  Wire.write(displaybuffer[digit] >> 8);   // nibble
  Wire.endTransmission();
#if HT16K33_DEBUG > 2
  Serial.print(F("   digit=")); Serial.println(digit);
  Serial.print(F("  device=")); Serial.println(device);
  Serial.print(F("i2c_addr=")); Serial.println(i2c_addr, HEX);
  Serial.print(F("     reg=")); Serial.println(reg);
  Serial.print(F("    char=")); Serial.println(displaybuffer[digit], BIN);
#endif
}
#endif

/*
  I2C lowLevel write of a bitmask to the IC to a specific position
*/
void cherry7seg_ht16k33::writeLowLevel(uint8_t position, uint16_t bitmask) {
  uint8_t device = position / _numDigits;
  uint8_t reg = (position % _numDigits) * 2;
  uint8_t i2c_addr = _i2c_addr + device;
  Wire.beginTransmission(i2c_addr);
  Wire.write(reg);                         // Register  (uint8_t)digit * 2
  Wire.write(bitmask & 0xFF); // nibble
  Wire.write(bitmask >> 8);   // nibble
  Wire.endTransmission();
#if HT16K33_DEBUG > 2
  Serial.print(F("position=")); Serial.println(position);
  Serial.print(F("  device=")); Serial.println(device);
  Serial.print(F("i2c_addr=")); Serial.println(i2c_addr, HEX);
  Serial.print(F("     reg=")); Serial.println(reg);
  Serial.print(F(" bitmask=")); Serial.println(bitmask, BIN);
#endif
}


/* *******************************************************************
   generic module 7 SEGMENT OBJECT
 * ******************************************************************/

cherry7seg_ht16k33_hw_7::cherry7seg_ht16k33_hw_7 ()
{
  //public:
  _numDigits = 8;            // In fact the display has 5 digits but only 0 1 3 4 are visible. 2 is used for the colon Digit
  _hasColonDigit = 0;
}

// general write method for 7 segment
size_t cherry7seg_ht16k33_hw_7::write(uint8_t value) {
  if (value == '.' || value == ',' || value == ':' || value == ';')  // dots need a special handling
  {
#ifdef HT16K33_BUFFERSIZE
    displaybuffer[_lastPosition] = displaybuffer[_lastPosition] | SEG_DP;
#endif
    _lastBitmap = _lastBitmap | SEG_DP;
    writeLowLevel(_lastPosition, _lastBitmap);
  }
  else if (value > 31 && value < 128)                                // write printable ASCII characters to display
  {
    _lastBitmap = pgm_read_byte_near(charTable + value - 32);          // the table starts with the first printable character at 32
#ifdef HT16K33_BUFFERSIZE    
    displaybuffer[_currentPosition] = _lastBitmap;
#endif
    _lastPosition = _currentPosition;
    writeLowLevel(_currentPosition, _lastBitmap);
    _currentPosition++;
    if (_currentPosition >= _numDigits * _numDevices) _currentPosition = 0;
  }
  return 1; // assume sucess
}



/* *******************************************************************
   7 SEGMENT 4 Digits no digitalpoint, and colon digit
 * ******************************************************************/


cherry7seg_ht16k33_hw_7_4_c::cherry7seg_ht16k33_hw_7_4_c()
{
  _numDigits = 4;            // In fact the display has 5 digits but only 0 1 3 4 are visible. 2 is used for the colon Digit
}

