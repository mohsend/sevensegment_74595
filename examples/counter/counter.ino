#include <ss_sr.h> // https://github.com/mohsend/sevensegment_74595

unsigned long sp = 500;
int8_t st = 1;

void setup() {
  // setup Display with shift register pins: 
  // latch: 10
  // clock: 12
  // data: 11
  // there are 4 sevensegments being driven
  // and seven the seven segment is common anode
  setupDisplay(10, 12, 11, 4, COMMON_ANODE);
  // Connect to a potentiometer, sets delay between counts
  pinMode(A0, INPUT);
  // Connect to a push button, loops through diffrent settings 
  pinMode(A2, INPUT_PULLUP);
}

void loop() {
  char buf[5];
  static int16_t i = 0;
  if (digitalRead(A2)) {
    i += st;
    sprintf(buf, "%4d", i);
    displayString(buf);
    delay(sp);  // accepts unsigned long
  } else {
    static uint8_t setting = 1;
    sp = analogRead(A0);
    //sp = map(sp, 1, 10000, 0, 1023); // map delay between 1ms and 10s
    
    switch(setting) {
      case 0:
        st = 1;
        strcpy(buf, " +1 ");
        break;
      case 1:
        st = -1;
        strcpy(buf, " -1 ");
        break;
      case 2:
        st = 2;
        i = (i % 2)? i-1 : i;
        strcpy(buf, " +2n");
        break;
      case 3:
        st = -2;
        i = (i % 2)? i : i-1;
        strcpy(buf, "2n+1");
        break;
      case 4:
        st = -2;
        i = (i % 2)? i+1 : i;
        strcpy(buf, " -2n");
        break;
      case 5:
        st = -2;
        i = (i % 2)? i : i+1;
        strcpy(buf, "-2n+1");
        break;
      default:
        break;
    }
    
    if (++setting == 6) setting = 0;
    
    while(!digitalRead(A2)) displayTicker("^-v-^-v", 100);
    displayString("step");
    delay(700);
    displayString(buf);
    delay(1000);
    displayString("dely");
    delay(700);
    sprintf(buf, "%04d", sp);
    displayString(buf);
    delay(1000);
    displayTicker("   <><><><     ", 100);
  }
}

