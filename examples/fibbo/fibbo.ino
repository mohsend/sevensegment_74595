#include <ss_sr.h> // // https://github.com/mohsend/sevensegment_74595

int fibbo(int a) {
  if (a <= 2) {
    return a;
  } else {
    return fibbo(a-2) + fibbo(a-1);
  }
}

void setup() {
  // setup Display with shift register pins: 
  // latch: 10
  // clock: 12
  // data: 11
  // and seven the seven segment is common anode (true)
  setupDisplay(10, 12, 11, 4, COMMON_ANODE);
}

void loop() {
  for (int i = 0; i < 20; i++) {
    static char buf[5];
    int f = fibbo(i);
    sprintf(buf, "%4d", f);
    displayString(buf);
    delay(700);
  }
}
