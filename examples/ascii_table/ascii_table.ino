#include <ss_sr.h> // https://github.com/mohsend/sevensegment_74595

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
  displayTicker("     Wwill display the ascii table fromm 0x20 to 0x60. Formmat:  Character  Hex Value    ");
  for (byte i = 0x20; i < 0x60; i++) {
    static char buf[5];
    sprintf(buf, "%c %02x", i, i);
    displayString(buf);
    Serial.println(buf);
    delay(700);
  }
}
