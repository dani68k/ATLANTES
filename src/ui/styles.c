#include "styles.h"
#include "images.h"
#include "fonts.h"

#include "ui.h"
#include "screens.h"

//
// Style: bar_histo
//

void init_style_bar_histo_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_radius(style, 1);
    lv_style_set_bg_color(style, lv_color_darken(lv_color_hex(0xe5e6e8), 64));
    lv_style_set_bg_opa(style, 0);
};

lv_style_t *get_style_bar_histo_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_bar_histo_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_bar_histo_INDICATOR_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x4a00e0));
    lv_style_set_radius(style, 1);
    lv_style_set_bg_grad_dir(style, LV_GRAD_DIR_HOR);
    lv_style_set_bg_grad_color(style, lv_color_hex(0x3b00ed));
};

lv_style_t *get_style_bar_histo_INDICATOR_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_bar_histo_INDICATOR_DEFAULT(style);
    }
    return style;
};

void add_style_bar_histo(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_bar_histo_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_bar_histo_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
};

void remove_style_bar_histo(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_bar_histo_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_bar_histo_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
};

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
//
//

void add_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*AddStyleFunc)(lv_obj_t *obj);
    static const AddStyleFunc add_style_funcs[] = {
        add_style_bar_histo,
        add_style_label_y,
    };
    add_style_funcs[styleIndex](obj);
}

void remove_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*RemoveStyleFunc)(lv_obj_t *obj);
    static const RemoveStyleFunc remove_style_funcs[] = {
        remove_style_bar_histo,
        remove_style_label_y,
    };
    remove_style_funcs[styleIndex](obj);
}