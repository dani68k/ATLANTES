#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_SETUP = 2,
    SCREEN_ID_KEYBOARD = 3,
    _SCREEN_ID_LAST = 3
};

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *setup;
    lv_obj_t *keyboard;
    lv_obj_t *label_max_indicator;
    lv_obj_t *obj0;
    lv_obj_t *eje_y;
    lv_obj_t *mark_100;
    lv_obj_t *label_100;
    lv_obj_t *mark_80;
    lv_obj_t *label_2;
    lv_obj_t *mark_60;
    lv_obj_t *label_60;
    lv_obj_t *mark_40;
    lv_obj_t *label_40;
    lv_obj_t *mark_20;
    lv_obj_t *label_20;
    lv_obj_t *label_21;
    lv_obj_t *bar_410;
    lv_obj_t *bar_435;
    lv_obj_t *bar_460;
    lv_obj_t *bar_485;
    lv_obj_t *bar_510;
    lv_obj_t *bar_535;
    lv_obj_t *bar_560;
    lv_obj_t *bar_585;
    lv_obj_t *bar_610;
    lv_obj_t *bar_645;
    lv_obj_t *bar_680;
    lv_obj_t *bar_705;
    lv_obj_t *bar_730;
    lv_obj_t *bar_760;
    lv_obj_t *bar_810;
    lv_obj_t *bar_860;
    lv_obj_t *bar_900;
    lv_obj_t *bar_940;
    lv_obj_t *label_x_1;
    lv_obj_t *label_x_2;
    lv_obj_t *label_x_3;
    lv_obj_t *label_x_4;
    lv_obj_t *label_x_5;
    lv_obj_t *label_x_6;
    lv_obj_t *label_x_7;
    lv_obj_t *label_x_8;
    lv_obj_t *label_x_9;
    lv_obj_t *label_x_10;
    lv_obj_t *label_x_11;
    lv_obj_t *label_x_12;
    lv_obj_t *label_x_13;
    lv_obj_t *label_x_14;
    lv_obj_t *label_x_15;
    lv_obj_t *label_x_16;
    lv_obj_t *label_x_17;
    lv_obj_t *label_x_18;
    lv_obj_t *label_x_410;
    lv_obj_t *label_x_510;
    lv_obj_t *label_x_610;
    lv_obj_t *label_x_730;
    lv_obj_t *label_x_940;
    lv_obj_t *eje_x;
    lv_obj_t *obj1;
    lv_obj_t *button_log_main;
    lv_obj_t *obj2;
    lv_obj_t *button_live_main;
    lv_obj_t *button_led_main;
    lv_obj_t *setup_icon;
    lv_obj_t *label_leds_main;
    lv_obj_t *obj3;
    lv_obj_t *panel_leds;
    lv_obj_t *obj4;
    lv_obj_t *roller_ir;
    lv_obj_t *obj5;
    lv_obj_t *roller_uv;
    lv_obj_t *roller_white;
    lv_obj_t *obj6;
    lv_obj_t *value_gain;
    lv_obj_t *panel_gain;
    lv_obj_t *slide_gain;
    lv_obj_t *value_integration;
    lv_obj_t *panel_integration;
    lv_obj_t *slide_integration;
    lv_obj_t *value_measuring;
    lv_obj_t *panel_measuring;
    lv_obj_t *slide_measure;
    lv_obj_t *button_save_setup;
    lv_obj_t *button_back_setup;
    lv_obj_t *switch_auto_setup;
    lv_obj_t *label_switch_setup;
    lv_obj_t *keyboard_keyboard;
    lv_obj_t *textarea_keyboard;
} objects_t;

extern objects_t objects;

void create_screen_main();
void tick_screen_main();

void create_screen_setup();
void tick_screen_setup();

void create_screen_keyboard();
void tick_screen_keyboard();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/