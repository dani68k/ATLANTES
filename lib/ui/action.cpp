#include "actions.h"
#include <Arduino.h>

void action_button_main(lv_event_t * e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_PRESSED) {
    Serial.println("Button pressed!");
  }
  if (code == LV_EVENT_RELEASED) {
    Serial.println("Button released!");
  } 
}