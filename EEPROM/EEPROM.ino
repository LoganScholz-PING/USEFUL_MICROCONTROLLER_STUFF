/*
SOME VARIABLES ARE NOT DEFINED, YADA YADA THIS IS AN EXAMPLE
*/

#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_CONTENTS_START_ADDRESS 0 // 4 bytes wide

enum CLUB_TYPE { GRAPHITE, IRON, GENERIC };

struct EEPROM_CONTENTS 
{
  // set default values initially
  unsigned long EEPROM_total_shaft_count = 0;
  CLUB_TYPE club_type = CLUB_TYPE::GENERIC;
  unsigned long saved_on_time = 4000;
};

EEPROM_CONTENTS ec;

void updateEEPROMContents() 
{
  ec.EEPROM_total_shaft_count += (total_shaft_count - EEPROM_last_pwr_cycle_shaft_count);
  ec.saved_on_time = led_on_time;
  EEPROM.put(EEPROM_CONTENTS_START_ADDRESS, ec);
  EEPROM_last_pwr_cycle_shaft_count = total_shaft_count;
}

void loadEEPROMContents() 
{
  EEPROM.get(EEPROM_CONTENTS_START_ADDRESS, ec);
  
  // check for uninitialized EEPROM (will be all 1's)
  if(ec.saved_on_time > 30000) 
  {
    ec.saved_on_time = 7000;
    ec.club_type = CLUB_TYPE::GRAPHITE;
    ec.EEPROM_total_shaft_count = 0;
    updateEEPROMContents(); // write the defaults to EEPROM
  }

  led_on_time = ec.saved_on_time;
}

void clearEEPROMContents() 
{
  // set first 4 bytes to 0 (ADDR0,1,2,3)
  ec.EEPROM_total_shaft_count = 0x0;
  ec.saved_on_time = led_on_time;
  EEPROM.put(EEPROM_CONTENTS_START_ADDRESS, ec); // reset total shaft count EEPROM to 0
}

void setup() {
  Serial.begin(9600); // for testing EEPROM

  loadEEPROMContents();
}

void loop() {
  if(nexbtn_sub_1_second) {
    led_on_time -= 1000;
    if(led_on_time <= 0) led_on_time = 1000; // clamp to a min time
    updateEEPROMContents();
  }

  if(nexbtn_add_1_second) {
    led_on_time += 1000;
    if(led_on_time > 15000) led_on_time = 15000; // clamp to a max time
    updateEEPROMContents();
  }

  if(nexbtn_reset_eeprom) {
    clearEEPROMContents();
  }

  // !! we are no longer tracking club type for now !!
  // if(nexbtn_switch_club_type) {
  //   if(ec.club_type == CLUB_TYPE::IRON) ec.club_type = CLUB_TYPE::GRAPHITE;
  //   else ec.club_type = CLUB_TYPE::IRON;
  //   updateEEPROMContents();
  // }

  // update EEPROM every EEPROM_save_period milliseconds
  if((unsigned long)millis() - EEPROM_last_save_time >= EEPROM_save_period) {
    updateEEPROMContents();
    EEPROM_last_save_time = millis();
  }
}