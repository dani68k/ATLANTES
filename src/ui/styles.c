#include "styles.h"
#include "images.h"
#include "fonts.h"

#include "ui.h"
#include "screens.h"

//
// Style: Label_Y
//

void init_style_label_y_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_font(style, &lv_font_montserrat_10);
    lv_style_set_text_color(style, lv_color_hex(0xffffff));
};

lv_style_t *get_style_label_y_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_label_y_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_label_y(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_label_y_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_label_y(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_label_y_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: button_main
//

void init_style_button_main_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x108cf0));
    lv_style_set_shadow_width(style, 0);
};

lv_style_t *get_style_button_main_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_button_main_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_button_main_MAIN_PRESSED(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x00ff5e));
    lv_style_set_text_color(style, lv_color_hex(0x0d141b));
};

lv_style_t *get_style_button_main_MAIN_PRESSED() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_button_main_MAIN_PRESSED(style);
    }
    return style;
};

void add_style_button_main(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_button_main_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_button_main_MAIN_PRESSED(), LV_PART_MAIN | LV_STATE_PRESSED);
};

void remove_style_button_main(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_button_main_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_button_main_MAIN_PRESSED(), LV_PART_MAIN | LV_STATE_PRESSED);
};

//
// Style: bar_histogram
//

void init_style_bar_histogram_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_radius(style, 1);
    lv_style_set_bg_color(style, lv_color_darken(lv_color_hex(0xe5e6e8), 64));
    lv_style_set_bg_opa(style, 0);
};

lv_style_t *get_style_bar_histogram_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_bar_histogram_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_bar_histogram_INDICATOR_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x004cff));
    lv_style_set_radius(style, 1);
    lv_style_set_border_color(style, lv_color_hex(0xffffff));
    lv_style_set_border_width(style, 2);
    lv_style_set_border_opa(style, 0);
    lv_style_set_bg_grad_dir(style, LV_GRAD_DIR_NONE);
    lv_style_set_bg_grad_color(style, lv_color_hex(0x000000));
};

lv_style_t *get_style_bar_histogram_INDICATOR_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_bar_histogram_INDICATOR_DEFAULT(style);
    }
    return style;
};

void add_style_bar_histogram(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_bar_histogram_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_bar_histogram_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
};

void remove_style_bar_histogram(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_bar_histogram_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_bar_histogram_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
};

//
// Style: Background
//

void init_style_background_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x0d141b));
};

lv_style_t *get_style_background_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_background_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_background(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_background_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_background(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_background_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: Panel
//

void init_style_panel_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_border_color(style, lv_color_hex(0x79a3c2));
    lv_style_set_bg_color(style, lv_color_hex(0x2b4055));
};

lv_style_t *get_style_panel_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_panel_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_panel_MAIN_PRESSED(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x1d2c3b));
};

lv_style_t *get_style_panel_MAIN_PRESSED() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_panel_MAIN_PRESSED(style);
    }
    return style;
};

void add_style_panel(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_panel_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_panel_MAIN_PRESSED(), LV_PART_MAIN | LV_STATE_PRESSED);
};

void remove_style_panel(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_panel_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_panel_MAIN_PRESSED(), LV_PART_MAIN | LV_STATE_PRESSED);
};

//
// Style: slide
//

void init_style_slide_KNOB_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x108cf0));
    lv_style_set_radius(style, 1);
};

lv_style_t *get_style_slide_KNOB_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_slide_KNOB_DEFAULT(style);
    }
    return style;
};

void init_style_slide_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0xffffff));
    lv_style_set_bg_opa(style, 255);
};

lv_style_t *get_style_slide_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_slide_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_slide(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_slide_KNOB_DEFAULT(), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_slide_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_slide(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_slide_KNOB_DEFAULT(), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_slide_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: roller
//

void init_style_roller_SELECTED_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x108cf0));
    lv_style_set_text_font(style, &ui_font_montserrat_extra_bold_12);
    lv_style_set_radius(style, 5);
};

lv_style_t *get_style_roller_SELECTED_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_roller_SELECTED_DEFAULT(style);
    }
    return style;
};

void init_style_roller_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_font(style, &ui_font_montserrat_bold_10);
    lv_style_set_radius(style, 7);
};

lv_style_t *get_style_roller_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_roller_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_roller(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_roller_SELECTED_DEFAULT(), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_roller_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_roller(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_roller_SELECTED_DEFAULT(), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_roller_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
//
//

void add_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*AddStyleFunc)(lv_obj_t *obj);
    static const AddStyleFunc add_style_funcs[] = {
        add_style_label_y,
        add_style_button_main,
        add_style_bar_histogram,
        add_style_background,
        add_style_panel,
        add_style_slide,
        add_style_roller,
    };
    add_style_funcs[styleIndex](obj);
}

void remove_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*RemoveStyleFunc)(lv_obj_t *obj);
    static const RemoveStyleFunc remove_style_funcs[] = {
        remove_style_label_y,
        remove_style_button_main,
        remove_style_bar_histogram,
        remove_style_background,
        remove_style_panel,
        remove_style_slide,
        remove_style_roller,
    };
    remove_style_funcs[styleIndex](obj);
}