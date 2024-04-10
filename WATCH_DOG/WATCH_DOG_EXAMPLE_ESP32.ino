#include <esp_task_wdt.h>

int count = 0;
unsigned long wait = 2000;
unsigned long last_check = millis();

void setup() 
{
  Serial.begin(115200);

  esp_task_wdt_config_t wdt_config = 
  {
      .timeout_ms = 3000,
      .idle_core_mask = 0, // Subscribe to the idle task on the APP CPU
      .trigger_panic = true,
  };
  esp_err_t err = esp_task_wdt_reconfigure(&wdt_config);
  esp_task_wdt_add(NULL);
}

void loop() 
{
  if(millis() - last_check >= wait) 
  {
    Serial.print("Count: ");
    Serial.println(count);
    
    esp_task_wdt_reset();

    count++;
    last_check = millis();
  }
}
