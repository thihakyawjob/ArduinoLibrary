/*******************************************************************************
  Strandtest 7 Segment
  cherry7seg HT16K33

  This is a strandtest to show if wiring of your HT16K33 display is correct

  by cherry7seg
  Sketch Version 2020-06-05
  
  On startup you should see some text on your display
  In the loop all printable characters will be shown.

 *******************************************************************************/
#include <Wire.h>                                                 // HT16K33 uses I2C, include the Wire Library
#include "cherry7seg.h"                                          // include the cherry7seg HT16K33 library - download from //http://werner.rothschopf.net/

cherry7seg_ht16k33_hw_7 displayL0 = cherry7seg_ht16k33_hw_7();             // object for 7 segments, 8 digits
cherry7seg_ht16k33_hw_7 displayL1 = cherry7seg_ht16k33_hw_7();             // object for 7 segments, 8 digits
cherry7seg_ht16k33_hw_7 displayL2 = cherry7seg_ht16k33_hw_7();             // object for 7 segments, 8 digits
cherry7seg_ht16k33_hw_7 displayL3 = cherry7seg_ht16k33_hw_7();             // object for 7 segments, 8 digits


const uint8_t line0 = 0x70;                                     // the I2C address of the first module
const uint8_t line1 = 0x71;                                     // the I2C address of the first module
const uint8_t line2 = 0x72;                                     // the I2C address of the first module
const uint8_t line3 = 0x73;                                     // the I2C address of the first module
const uint16_t wait = 500;                                      // wait milliseconds between demos


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(F("\n01 strandtest 7segment"));
  Wire.begin();                                                      // start the I2C interface
  displayL0.begin(line0, 1);                           // I2C address of first display, total number of devices
  displayL1.begin(line1, 1);                           // I2C address of first display, total number of devices
  displayL2.begin(line2, 1);                           // I2C address of first display, total number of devices
  displayL3.begin(line3, 1);                           // I2C address of first display, total number of devices

  // display.setColonDigit();                                        // activates a separate digit for a colon manually. But better to use the cherry7seg_ht16k33_hw_7_4_c class if necessary.
  Serial.println(F("ON Test"));
  displayL0.clear();                     // clear display
  displayL0.print(F("CHERRY0"));    // if the text is to long or your display (chain) you will only see the last letters
  displayL1.clear();                     // clear display
  displayL1.print(F("CHERRY1"));    // if the text is to long or your display (chain) you will only see the last letters
  displayL2.clear();                     // clear display
  displayL2.print(F("CHERRY2"));    // if the text is to long or your display (chain) you will only see the last letters
  displayL3.clear();                     // clear display
  displayL3.print(F("CHERRY3"));    // if the text is to long or your display (chain) you will only see the last letters
  delay(wait);
}


void loop() {


}

