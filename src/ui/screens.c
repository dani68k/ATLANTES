#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;

//
// Event handlers
//

lv_obj_t *tick_value_change_obj;

//
// Screens
//

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x0d141b), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // label_max_indicator
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.label_max_indicator = obj;
            lv_obj_set_pos(obj, 168, 9);
            lv_obj_set_size(obj, 147, 15);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &ui_font_montserrat_bold_12, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "CH 9 - 610nm - I = 0.95");
        }
        {
            lv_obj_t *obj = lv_line_create(parent_obj);
            objects.obj0 = obj;
            lv_obj_set_pos(obj, 0, 30);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            static lv_point_precise_t line_points[] = {
                { 0, 0 },
                { 320, 0 }
            };
            lv_line_set_points(obj, line_points, 2);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_line_color(obj, lv_color_hex(0x79a3c2), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.obj1 = obj;
            lv_obj_set_pos(obj, 5, 35);
            lv_obj_set_size(obj, 310, 150);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_border_color(obj, lv_color_hex(0x79a3c2), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2b4055), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1d2c3b), LV_PART_MAIN | LV_STATE_PRESSED);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // Eje_Y
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    objects.eje_y = obj;
                    lv_obj_set_pos(obj, 10, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    static lv_point_precise_t line_points[] = {
                        { 0, 0 },
                        { 0, 101 }
                    };
                    lv_line_set_points(obj, line_points, 2);
                    lv_line_set_y_invert(obj, true);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // Mark_100
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    objects.mark_100 = obj;
                    lv_obj_set_pos(obj, 10, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    static lv_point_precise_t line_points[] = {
                        { 0, 0 },
                        { 0, 0 },
                        { 275, 0 }
                    };
                    lv_line_set_points(obj, line_points, 3);
                    lv_obj_set_style_line_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0x807c7c), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // label_100
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_100 = obj;
                    lv_obj_set_pos(obj, -8, -4);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "1,0");
                }
                {
                    // mark_80
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    objects.mark_80 = obj;
                    lv_obj_set_pos(obj, 10, 20);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    static lv_point_precise_t line_points[] = {
                        { 0, 0 },
                        { 0, 0 },
                        { 275, 0 }
                    };
                    lv_line_set_points(obj, line_points, 3);
                    lv_obj_set_style_line_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0x807c7c), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // label_2
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_2 = obj;
                    lv_obj_set_pos(obj, -10, 15);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "0.8");
                }
                {
                    // Mark_60
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    objects.mark_60 = obj;
                    lv_obj_set_pos(obj, 10, 40);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    static lv_point_precise_t line_points[] = {
                        { 0, 0 },
                        { 0, 0 },
                        { 275, 0 }
                    };
                    lv_line_set_points(obj, line_points, 3);
                    lv_obj_set_style_line_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0x807c7c), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // label_60
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_60 = obj;
                    lv_obj_set_pos(obj, -9, 35);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "0.6");
                }
                {
                    // Mark_40
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    objects.mark_40 = obj;
                    lv_obj_set_pos(obj, 10, 60);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    static lv_point_precise_t line_points[] = {
                        { 0, 0 },
                        { 0, 0 },
                        { 275, 0 }
                    };
                    lv_line_set_points(obj, line_points, 3);
                    lv_obj_set_style_line_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0x807c7c), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // label_40
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_40 = obj;
                    lv_obj_set_pos(obj, -9, 55);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "0.4");
                }
                {
                    // Mark_20
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    objects.mark_20 = obj;
                    lv_obj_set_pos(obj, 10, 80);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    static lv_point_precise_t line_points[] = {
                        { 0, 0 },
                        { 0, 0 },
                        { 275, 0 }
                    };
                    lv_line_set_points(obj, line_points, 3);
                    lv_obj_set_style_line_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0x807c7c), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // label_20
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_20 = obj;
                    lv_obj_set_pos(obj, -9, 75);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "0.2");
                }
                {
                    // label_21
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_21 = obj;
                    lv_obj_set_pos(obj, -9, 94);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "0.0");
                }
                {
                    // bar_410
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.bar_410 = obj;
                    lv_obj_set_pos(obj, 15, 0);
                    lv_obj_set_size(obj, 13, 100);
                    lv_bar_set_value(obj, 10, LV_ANIM_ON);
                    add_style_bar_histogram(obj);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x6a00ff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    // bar_435
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.bar_435 = obj;
                    lv_obj_set_pos(obj, 30, 0);
                    lv_obj_set_size(obj, 13, 100);
                    lv_bar_set_value(obj, 20, LV_ANIM_ON);
                    add_style_bar_histogram(obj);
                    lv_obj_set_style_radius(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_darken(lv_color_hex(0xe5e6e8), 64), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x4b00ff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 1, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    // bar_460
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.bar_460 = obj;
                    lv_obj_set_pos(obj, 45, 0);
                    lv_obj_set_size(obj, 13, 100);
                    lv_bar_set_value(obj, 30, LV_ANIM_ON);
                    add_style_bar_histogram(obj);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x0055ff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    // bar_485
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.bar_485 = obj;
                    lv_obj_set_pos(obj, 60, 0);
                    lv_obj_set_size(obj, 13, 100);
                    lv_bar_set_value(obj, 40, LV_ANIM_ON);
                    add_style_bar_histogram(obj);
                    lv_obj_set_style_radius(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_darken(lv_color_hex(0xe5e6e8), 64), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x009aff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 1, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    // bar_510
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.bar_510 = obj;
                    lv_obj_set_pos(obj, 75, 0);
                    lv_obj_set_size(obj, 13, 100);
                    lv_bar_set_value(obj, 50, LV_ANIM_ON);
                    add_style_bar_histogram(obj);
                    lv_obj_set_style_radius(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_darken(lv_color_hex(0xe5e6e8), 64), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x00d4b0), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 1, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    // bar_535
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.bar_535 = obj;
                    lv_obj_set_pos(obj, 90, 0);
                    lv_obj_set_size(obj, 13, 100);
                    lv_bar_set_value(obj, 60, LV_ANIM_ON);
                    add_style_bar_histogram(obj);
                    lv_obj_set_style_radius(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_darken(lv_color_hex(0xe5e6e8), 64), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x00c850), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 1, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    // bar_560
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.bar_560 = obj;
                    lv_obj_set_pos(obj, 105, 0);
                    lv_obj_set_size(obj, 13, 100);
                    lv_bar_set_value(obj, 70, LV_ANIM_ON);
                    add_style_bar_histogram(obj);
                    lv_obj_set_style_radius(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_darken(lv_color_hex(0xe5e6e8), 64), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x58d000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 1, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    // bar_585
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.bar_585 = obj;
                    lv_obj_set_pos(obj, 120, 0);
                    lv_obj_set_size(obj, 13, 100);
                    lv_bar_set_value(obj, 80, LV_ANIM_ON);
                    add_style_bar_histogram(obj);
                    lv_obj_set_style_radius(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_darken(lv_color_hex(0xe5e6e8), 64), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xb8d000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 1, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    // bar_610
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.bar_610 = obj;
                    lv_obj_set_pos(obj, 135, 0);
                    lv_obj_set_size(obj, 13, 100);
                    lv_bar_set_value(obj, 90, LV_ANIM_ON);
                    add_style_bar_histogram(obj);
                    lv_obj_set_style_radius(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_darken(lv_color_hex(0xe5e6e8), 64), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffb000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 1, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    // bar_645
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.bar_645 = obj;
                    lv_obj_set_pos(obj, 150, 0);
                    lv_obj_set_size(obj, 13, 100);
                    lv_bar_set_value(obj, 100, LV_ANIM_ON);
                    add_style_bar_histogram(obj);
                    lv_obj_set_style_radius(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_darken(lv_color_hex(0xe5e6e8), 64), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff6a00), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 1, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    // bar_680
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.bar_680 = obj;
                    lv_obj_set_pos(obj, 165, 0);
                    lv_obj_set_size(obj, 13, 100);
                    lv_bar_set_value(obj, 90, LV_ANIM_ON);
                    add_style_bar_histogram(obj);
                    lv_obj_set_style_radius(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_darken(lv_color_hex(0xe5e6e8), 64), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xe03000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 1, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    // bar_705
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.bar_705 = obj;
                    lv_obj_set_pos(obj, 180, 0);
                    lv_obj_set_size(obj, 13, 100);
                    lv_bar_set_value(obj, 80, LV_ANIM_ON);
                    add_style_bar_histogram(obj);
                    lv_obj_set_style_radius(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_darken(lv_color_hex(0xe5e6e8), 64), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xb02000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 1, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    // bar_730
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.bar_730 = obj;
                    lv_obj_set_pos(obj, 195, 0);
                    lv_obj_set_size(obj, 13, 100);
                    lv_bar_set_value(obj, 70, LV_ANIM_ON);
                    add_style_bar_histogram(obj);
                    lv_obj_set_style_radius(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_darken(lv_color_hex(0xe5e6e8), 64), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x8a1a00), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 1, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    // bar_760
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.bar_760 = obj;
                    lv_obj_set_pos(obj, 210, 0);
                    lv_obj_set_size(obj, 13, 100);
                    lv_bar_set_value(obj, 60, LV_ANIM_ON);
                    add_style_bar_histogram(obj);
                    lv_obj_set_style_radius(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_darken(lv_color_hex(0xe5e6e8), 64), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x6e1608), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 1, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    // bar_810
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.bar_810 = obj;
                    lv_obj_set_pos(obj, 225, 0);
                    lv_obj_set_size(obj, 13, 100);
                    lv_bar_set_value(obj, 50, LV_ANIM_ON);
                    add_style_bar_histogram(obj);
                    lv_obj_set_style_radius(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_darken(lv_color_hex(0xe5e6e8), 64), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x5a1018), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 1, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    // bar_860
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.bar_860 = obj;
                    lv_obj_set_pos(obj, 240, 0);
                    lv_obj_set_size(obj, 13, 100);
                    lv_bar_set_value(obj, 40, LV_ANIM_ON);
                    add_style_bar_histogram(obj);
                    lv_obj_set_style_radius(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_darken(lv_color_hex(0xe5e6e8), 64), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x4a1028), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 1, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    // bar_940
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.bar_940 = obj;
                    lv_obj_set_pos(obj, 270, 0);
                    lv_obj_set_size(obj, 13, 100);
                    lv_bar_set_value(obj, 20, LV_ANIM_ON);
                    add_style_bar_histogram(obj);
                    lv_obj_set_style_radius(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_darken(lv_color_hex(0xe5e6e8), 64), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2c1e38), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 1, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    // bar_900
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.bar_900 = obj;
                    lv_obj_set_pos(obj, 255, 0);
                    lv_obj_set_size(obj, 13, 100);
                    lv_bar_set_value(obj, 30, LV_ANIM_ON);
                    add_style_bar_histogram(obj);
                    lv_obj_set_style_radius(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_darken(lv_color_hex(0xe5e6e8), 64), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3a1634), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 1, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    // label_x_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_x_1 = obj;
                    lv_obj_set_pos(obj, 20, 105);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "1");
                }
                {
                    // label_x_2
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_x_2 = obj;
                    lv_obj_set_pos(obj, 34, 105);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "2");
                }
                {
                    // label_x_3
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_x_3 = obj;
                    lv_obj_set_pos(obj, 49, 105);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "3");
                }
                {
                    // label_x_4
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_x_4 = obj;
                    lv_obj_set_pos(obj, 63, 105);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "4");
                }
                {
                    // label_x_5
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_x_5 = obj;
                    lv_obj_set_pos(obj, 79, 105);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "5");
                }
                {
                    // label_x_6
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_x_6 = obj;
                    lv_obj_set_pos(obj, 94, 105);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "6");
                }
                {
                    // label_x_7
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_x_7 = obj;
                    lv_obj_set_pos(obj, 109, 105);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "7");
                }
                {
                    // label_x_8
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_x_8 = obj;
                    lv_obj_set_pos(obj, 124, 105);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "8");
                }
                {
                    // label_x_9
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_x_9 = obj;
                    lv_obj_set_pos(obj, 140, 105);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "9");
                }
                {
                    // label_x_10
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_x_10 = obj;
                    lv_obj_set_pos(obj, 152, 105);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "10");
                }
                {
                    // label_x_11
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_x_11 = obj;
                    lv_obj_set_pos(obj, 168, 105);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "11");
                }
                {
                    // label_x_12
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_x_12 = obj;
                    lv_obj_set_pos(obj, 182, 105);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "12");
                }
                {
                    // label_x_13
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_x_13 = obj;
                    lv_obj_set_pos(obj, 197, 105);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "13");
                }
                {
                    // label_x_14
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_x_14 = obj;
                    lv_obj_set_pos(obj, 211, 105);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "14");
                }
                {
                    // label_x_15
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_x_15 = obj;
                    lv_obj_set_pos(obj, 227, 105);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "15");
                }
                {
                    // label_x_16
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_x_16 = obj;
                    lv_obj_set_pos(obj, 242, 105);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "16");
                }
                {
                    // label_x_17
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_x_17 = obj;
                    lv_obj_set_pos(obj, 257, 105);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "17");
                }
                {
                    // label_x_18
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_x_18 = obj;
                    lv_obj_set_pos(obj, 272, 105);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "18");
                }
                {
                    // label_x_410
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_x_410 = obj;
                    lv_obj_set_pos(obj, 15, 120);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "410");
                }
                {
                    // label_x_510
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_x_510 = obj;
                    lv_obj_set_pos(obj, 73, 120);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "510");
                }
                {
                    // label_x_610
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_x_610 = obj;
                    lv_obj_set_pos(obj, 135, 120);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "610");
                }
                {
                    // label_x_730
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_x_730 = obj;
                    lv_obj_set_pos(obj, 193, 120);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "730");
                }
                {
                    // label_x_940
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_x_940 = obj;
                    lv_obj_set_pos(obj, 267, 120);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_y(obj);
                    lv_label_set_text_static(obj, "940");
                }
                {
                    // Eje_X
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    objects.eje_x = obj;
                    lv_obj_set_pos(obj, 10, 101);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    static lv_point_precise_t line_points[] = {
                        { 0, 0 },
                        { 275, 0 }
                    };
                    lv_line_set_points(obj, line_points, 2);
                    lv_line_set_y_invert(obj, true);
                    lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            lv_obj_t *obj = lv_line_create(parent_obj);
            objects.obj2 = obj;
            lv_obj_set_pos(obj, 0, 190);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            static lv_point_precise_t line_points[] = {
                { 0, 0 },
                { 320, 0 }
            };
            lv_line_set_points(obj, line_points, 2);
            lv_line_set_y_invert(obj, true);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_line_color(obj, lv_color_hex(0x79a3c2), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 237, 200);
            lv_obj_set_size(obj, 75, 30);
            add_style_button_main(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 9, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_montserrat_extra_bold_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "SAVE");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, -19, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_awsome, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "\uf016");
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj3 = obj;
            lv_obj_set_pos(obj, 44, 3);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &ui_font_montserrat_extra_bold_12, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "SPECTROMETER\nAS7265x | 18 bands");
        }
        {
            lv_obj_t *obj = lv_image_create(parent_obj);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 44, 35);
            lv_image_set_src(obj, &img_logo_signal);
            lv_image_set_scale(obj, 150);
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 153, 200);
            lv_obj_set_size(obj, 75, 30);
            add_style_button_main(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, -19, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_awsome, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "\uf144");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 9, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_montserrat_extra_bold_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "LIVE");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 68, 200);
            lv_obj_set_size(obj, 75, 30);
            add_style_button_main(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, -19, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_awsome, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "\uf0eb");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 9, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_montserrat_extra_bold_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "LED");
                }
            }
        }
        {
            lv_obj_t *obj = lv_image_create(parent_obj);
            lv_obj_set_pos(obj, 5, 200);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_image_set_src(obj, &img_setup);
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
}

typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
};
void tick_screen(int screen_index) {
    if (screen_index >= 0 && screen_index < 1) {
        tick_screen_funcs[screen_index]();
    }
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen(screenId - 1);
}

//
// Fonts
//

ext_font_desc_t fonts[] = {
    { "MONTSERRAT_BOLD_12", &ui_font_montserrat_bold_12 },
    { "MONTSERRAT_EXTRA_BOLD_12", &ui_font_montserrat_extra_bold_12 },
    { "awsome", &ui_font_awsome },
#if LV_FONT_MONTSERRAT_8
    { "MONTSERRAT_8", &lv_font_montserrat_8 },
#endif
#if LV_FONT_MONTSERRAT_10
    { "MONTSERRAT_10", &lv_font_montserrat_10 },
#endif
#if LV_FONT_MONTSERRAT_12
    { "MONTSERRAT_12", &lv_font_montserrat_12 },
#endif
#if LV_FONT_MONTSERRAT_14
    { "MONTSERRAT_14", &lv_font_montserrat_14 },
#endif
#if LV_FONT_MONTSERRAT_16
    { "MONTSERRAT_16", &lv_font_montserrat_16 },
#endif
#if LV_FONT_MONTSERRAT_18
    { "MONTSERRAT_18", &lv_font_montserrat_18 },
#endif
#if LV_FONT_MONTSERRAT_20
    { "MONTSERRAT_20", &lv_font_montserrat_20 },
#endif
#if LV_FONT_MONTSERRAT_22
    { "MONTSERRAT_22", &lv_font_montserrat_22 },
#endif
#if LV_FONT_MONTSERRAT_24
    { "MONTSERRAT_24", &lv_font_montserrat_24 },
#endif
#if LV_FONT_MONTSERRAT_26
    { "MONTSERRAT_26", &lv_font_montserrat_26 },
#endif
#if LV_FONT_MONTSERRAT_28
    { "MONTSERRAT_28", &lv_font_montserrat_28 },
#endif
#if LV_FONT_MONTSERRAT_30
    { "MONTSERRAT_30", &lv_font_montserrat_30 },
#endif
#if LV_FONT_MONTSERRAT_32
    { "MONTSERRAT_32", &lv_font_montserrat_32 },
#endif
#if LV_FONT_MONTSERRAT_34
    { "MONTSERRAT_34", &lv_font_montserrat_34 },
#endif
#if LV_FONT_MONTSERRAT_36
    { "MONTSERRAT_36", &lv_font_montserrat_36 },
#endif
#if LV_FONT_MONTSERRAT_38
    { "MONTSERRAT_38", &lv_font_montserrat_38 },
#endif
#if LV_FONT_MONTSERRAT_40
    { "MONTSERRAT_40", &lv_font_montserrat_40 },
#endif
#if LV_FONT_MONTSERRAT_42
    { "MONTSERRAT_42", &lv_font_montserrat_42 },
#endif
#if LV_FONT_MONTSERRAT_44
    { "MONTSERRAT_44", &lv_font_montserrat_44 },
#endif
#if LV_FONT_MONTSERRAT_46
    { "MONTSERRAT_46", &lv_font_montserrat_46 },
#endif
#if LV_FONT_MONTSERRAT_48
    { "MONTSERRAT_48", &lv_font_montserrat_48 },
#endif
};

//
// Color themes
//

uint32_t active_theme_index = 0;

//
//
//

void create_screens() {

// Set default LVGL theme
    lv_display_t *dispp = lv_display_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_display_set_theme(dispp, theme);
    
    // Initialize screens
    // Create screens
    create_screen_main();
}