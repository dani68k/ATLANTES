#ifndef EEZ_LVGL_UI_STYLES_H
#define EEZ_LVGL_UI_STYLES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Style: Label_Y
lv_style_t *get_style_label_y_MAIN_DEFAULT();
void add_style_label_y(lv_obj_t *obj);
void remove_style_label_y(lv_obj_t *obj);

// Style: button_main
lv_style_t *get_style_button_main_MAIN_DEFAULT();
lv_style_t *get_style_button_main_MAIN_PRESSED();
void add_style_button_main(lv_obj_t *obj);
void remove_style_button_main(lv_obj_t *obj);

// Style: bar_histogram
lv_style_t *get_style_bar_histogram_MAIN_DEFAULT();
lv_style_t *get_style_bar_histogram_INDICATOR_DEFAULT();
void add_style_bar_histogram(lv_obj_t *obj);
void remove_style_bar_histogram(lv_obj_t *obj);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/