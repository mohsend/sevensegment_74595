#include <ss_sr.h> // https://github.com/mohsend/sevensegment_74595

void setup() {
  // setup Display with shift register pins: 
  // latch: 10
  // clock: 12
  // data: 11
  // and seven the seven segment is common anode (true)
  setupDisplay(10, 12, 11, 4, COMMON_ANODE);
  // set up serial port
  Serial.begin(9600);
}

void loop() {
  static char buf[50] = "   Enter string wwith serial mmonitor.   ";
  if (Serial.available() > 0) {
    Serial.readString().toCharArray(buf, 50);
    Serial.println(buf);
  }
  displayTicker(buf);
}
