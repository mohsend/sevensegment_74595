# Shift-register seven segment driver

Library and example sketches to drive multiple seven segments using daisy-chained shift-registers (74595).

## Use

### Exposed functions

- `void setupDisplay(LatchPin, ClockPin, DataPin, NumberOfDigits = 4, SegmentType = COMMON_ANODE)`
- `void displayString(String)`
- `void displayTicker(String, Delay = 200, Step = 1)`
- `void displayASCII(Character, Digit, Dot = 0)`
- `clearDisplay()`

## Example

```arduino
#include <ss_sr.h>

void setup() {
  // setup Display with shift register pins: 
  // latch: 10
  // clock: 12
  // data: 11
  // we have 4-digits of seven segment
  // and the seven segment is COMMON_ANODE (/CAMMON_CATHODE)
  setupDisplay(10, 12, 11, 4, COMMON_ANODE);
  Serial.begin(9600);
}

void loop() {
  static char buf[5];
  displayTicker("     Will display the ascii table from 0x20 to 0x60. Format:  Character Hex Value    ");
  for (byte i = 0x20; i < 0x60; i++) {
    static char buf[5];
    sprintf(buf, "%c %02x", i, i);
    displayString(buf);
    Serial.println(buf);
    delay(700);
  }
}
```
