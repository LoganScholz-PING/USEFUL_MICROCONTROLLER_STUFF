#include <avr/wdt.h>

// when WDT does not get reset this interrupt happens then the arduino restarts
ISR(WDT_vect) {
  wdt_disable();
  WDTCSR = bit (WDE); // this should reset the Arduino
}

void setWDT(byte sWDT) {
  WDTCSR |= 0b00011000; // get register ready for writing
                        // (we have 4 CPU cycles to change the register)
  WDTCSR = sWDT | WDTO_2S; // WDT reset arduino after 4 seconds of inactivity
  wdt_reset(); // confirm the settings
}

void setup() {
  wdt_disable(); // data sheet recommends disabling wdt immediately while uC starts up
  
  setWDT(0b01000000); // 00001000 = just reset if WDT not handled within timeframe
                      // 01001000 = set to trigger interrupt then reset
                      // 01000000 = just interrupt
}

void loop() {
  wdt_reset(); // if we don't reset the WDT within 2 seconds the arduino will restart
}
