#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_button_live(lv_event_t * e);
extern void action_button_back(lv_event_t * e);
extern void action_setup_icon(lv_event_t * e);
extern void action_slide_inegration_change(lv_event_t * e);
extern void action_slide_measure_change(lv_event_t * e);
extern void action_button_save_setup(lv_event_t * e);
extern void action_white_roller_change(lv_event_t * e);
extern void action_ir_roller_change(lv_event_t * e);
extern void action_uv_roller_change(lv_event_t * e);
extern void action_button_log_main(lv_event_t * e);
extern void action_button_led_main(lv_event_t * e);
extern void action_slide_gain_change(lv_event_t * e);
extern void action_keyboard_ready(lv_event_t * e);
extern void action_button_log(lv_event_t * e);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/