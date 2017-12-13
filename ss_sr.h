/*
 * These functions help you use a 4-digit sevensegment 
 * with a couple of 74595 serial-to-prallel daisy-chained together
 * to display ASCII strings
 * Author: mehsen.com
 * License: CC 0
 * https://creativecommons.org/publicdomain/zero/1.0/
 * Github Repo:
 * https://github.com/mohsend/sevensegment_74595
 */

#ifndef SEVENSEGMENT_DISPLAY_H
#define SEVENSEGMENT_DISPLAY_H 1

#define COMMON_CATHODE 0x00
#define COMMON_ANODE 0xff

#include <Arduino.h>
#include <TimerOne.h>
// #include <avr/interrupt.h>

// Globals
// Pin connected to latch pin (ST_CP) of 74HC595
byte ss_latchPin;
// Pin connected to clock pin (SH_CP) of 74HC595
byte ss_clockPin;
// Pin connected to Data in (DS) of 74HC595
byte ss_dataPin;
// number of sevensegments in the display
byte ss_count;
// type of LED seven segment display
byte ss_anode;
// bits that control common pins of seven segment
byte commonBits;
// assign the byte pattern to each digit on 7segment to be displayed
byte ss_pattern[8];

void setupDisplay(byte l, byte c, byte d, byte count = 4, byte a = 0);
inline void shiftPattern(byte pattern, byte place);
inline void writePattern(byte place, byte ch);
inline void clearDisplay();
inline void displayASCII(char ch, byte place, byte dot = 0);
void updateDisplay();
inline void displayString(char* str);
void displayTicker(char* str, unsigned long sp = 200, byte st = 1);

// each byte is a seven segment digit pattern
// this array is a segment of ASCII encoding.
// from 0x20 to 0x60
const PROGMEM byte ss_font[] = { 
  0x00, //  
  0x92, // !
  0x44, // "
  0x92, // #
  0x92, // $
  0x4a, // %
  0x92, // &
  0x04, // '
  0x9c, // (
  0xf0, // )
  0xc6, // *
  0x62, // +
  0x30, // ,
  0x02, // -
  0x01, // .
  0x4a, // /
  0xfc, // 0
  0x60, // 1
  0xda, // 2
  0xf2, // 3
  0x66, // 4
  0xb6, // 5
  0xbe, // 6
  0xe0, // 7
  0xfe, // 8
  0xf6, // 9
  0x12, // :
  0x12, // ;
  0x86, // <
  0x12, // =
  0xc2, // >
  0xca, // ?
  0x92, // @
  0xee, // A
  0x3e, // B
  0x1a, // C
  0x7a, // D
  0x9e, // E
  0x8e, // F
  0xf6, // G
  0x6e, // H
  0x0c, // I
  0x78, // J
  0x0e, // K
  0x1c, // L
  0x2a, // M
  0x2a, // N
  0xfc, // O
  0xce, // P
  0xe6, // Q
  0x0a, // R
  0xb6, // S
  0x1e, // T
  0x38, // U
  0x38, // V
  0x38, // W
  0x6e, // X
  0x76, // Y
  0xda, // Z
  0x9c, // [
  0x26, // '\'
  0xf0, // ]
  0xc4, // ^
  0x10, // _ 
  0x04 // `
  };

void setupDisplay(byte l, byte c, byte d, byte count, byte a) {
  // set global variables
  // Pin connected to latch pin (ST_CP) of 74HC595
  ss_latchPin = l;
  // Pin connected to clock pin (SH_CP) of 74HC595
  ss_clockPin = c;
  // Pin connected to Data in (DS) of 74HC595
  ss_dataPin = d;
  // number of sevensegments in the display
  ss_count = count - 1;
  
  commonBits = ~(0xff << (ss_count -1));
  // type of LED display
  ss_anode = a;
  
  // set pin modes to output
  pinMode(ss_latchPin, OUTPUT);
  pinMode(ss_dataPin, OUTPUT);  
  pinMode(ss_clockPin, OUTPUT);
  
  // attach updateDisplay function to timer1 interrupt
  Timer1.initialize(6000);
  Timer1.attachInterrupt(updateDisplay);

  // clear display if there's leftover data in shift registers
  clearDisplay();
}

inline void shiftPattern(byte pattern, byte place) {
  // shift the first byte that determines the digit should light up
  shiftOut(ss_dataPin, ss_clockPin, LSBFIRST, (ss_anode ^ 0x7f) >> place);
  // shift the next byte with the pattern
  shiftOut(ss_dataPin, ss_clockPin, LSBFIRST, pattern);
}

inline void writePattern(byte place, byte pattern) {
  // set latch to low, data won't show while shifting
  digitalWrite(ss_latchPin, LOW);
  // shift the font pattern to 'place' digit
  shiftPattern(ss_anode ^ pattern, place);
  // set latch to high, show new data
  digitalWrite(ss_latchPin, HIGH);
}


inline void clearDisplay() {
  for (byte i = 0; i < 4; i++)
    ss_pattern[i] = 0x00;
}

inline void displayASCII(char ch, byte place, byte dot = 0) {
  ch = ch - 0x20;  // I trusted the compiler to optimize this with a left-shift
  if (ch <= 0x40) {
    // map ASCII charachters to the font array
    ss_pattern[place] = pgm_read_byte_near(ss_font + ch) | dot;
  } else if (ch <= 0x5A) {
    // reuse lowercase characters for uppercase
    ss_pattern[place] = pgm_read_byte_near(ss_font + ch - 0x20) | dot;
  } else {
    // if the charecter is not in the array display nothing
    ss_pattern[place] = 0x00;
  }
}

// this functions gets called by time interrupt
void updateDisplay() {
  // show pattern of a digit each time the function runs
  static byte placeNow = 0;
  writePattern(placeNow, ss_pattern[placeNow]);
  // add 1 to placeNow and keep the lower 2 bits
  // this assigns 0, 1, 2, 3 to placeNow each time
  placeNow = (placeNow + 1) & commonBits;
}

inline void displayString(char* str) {
  int8_t end = 0;
  for (int8_t i = ss_count; i >= end; i--) {
    if (str[-(i-ss_count)+1] == '.') {
      displayASCII(str[-(i-ss_count)], i - end, 1);
      i--;
      end--;
    } else {
      displayASCII(str[-(i-ss_count)], i - end);
    }
  }
}

void displayTicker(char* str, unsigned long sp, byte st) {
  for (int8_t index = 0; str[index + ss_count]; index = index + st) {
    displayString(str + index);
    // wait before showing the next 4 characters
    delay(sp);
  }
}

#endif
