#include <lvgl.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include "ui/ui.h"
#include "ui/screens.h"
#include "ui/vars.h"

// Touchscreen pins
#define XPT2046_IRQ 36   // T_IRQ
#define XPT2046_MOSI 32  // T_DIN
#define XPT2046_MISO 39  // T_OUT
#define XPT2046_CLK 25   // T_CLK
#define XPT2046_CS 33    // T_CS

SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

// Touchscreen coordinates: (x, y) and pressure (z)
int x, y, z;

#define DRAW_BUF_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

// If logging is enabled, it will inform the user about what is happening in the library
void log_print(lv_log_level_t level, const char * buf) {
  LV_UNUSED(level);
  Serial.println(buf);
  Serial.flush();
}

// Get the Touchscreen data
void touchscreen_read(lv_indev_t * indev, lv_indev_data_t * data) {
  // Checks if Touchscreen was touched, and prints X, Y and Pressure (Z)
  if(touchscreen.tirqTouched() && touchscreen.touched()) {
    // Get Touchscreen points
    TS_Point p = touchscreen.getPoint();
    // Calibrate Touchscreen points with map function to the correct width and height
    x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
    z = p.z;

    data->state = LV_INDEV_STATE_PRESSED;

    // Set the coordinates
    data->point.x = x;
    data->point.y = y;

    // Print Touchscreen info about X, Y and Pressure (Z) on the Serial Monitor
    /* Serial.print("X = ");
    Serial.print(x);
    Serial.print(" | Y = ");
    Serial.print(y);
    Serial.print(" | Pressure = ");
    Serial.print(z);
    Serial.println();*/
  }
  else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

void Runtime() {
  static unsigned long lastTime = 0;
  static uint32_t count = 0;
  unsigned long currentTime = millis();
  if (currentTime - lastTime >= 1000) { // Check if 1 second has passed
    lastTime = currentTime;
    byte array[18];
    array[0] = random(0, 100); // Generate a random value between 0 and 100
    lv_bar_set_value(objects.bar_410, array[0], LV_ANIM_ON); // Update the progress bar value

    array[1] = random(0, 100);
    lv_bar_set_value(objects.bar_435, array[1], LV_ANIM_ON); // Update the progress bar value
        
    array[2] = random(0, 100);
    lv_bar_set_value(objects.bar_460, array[2], LV_ANIM_ON); // Update the progress bar value

    array[3] = random(0, 100);
    lv_bar_set_value(objects.bar_485, array[3], LV_ANIM_ON); // Update the progress bar value

    array[4] = random(0, 100);
    lv_bar_set_value(objects.bar_510, array[4], LV_ANIM_ON); // Update the progress bar value

    array[5] = random(0, 100);
    lv_bar_set_value(objects.bar_535, array[5], LV_ANIM_ON); // Update the progress bar value

    array[6] = random(0, 100);
    lv_bar_set_value(objects.bar_560, array[6], LV_ANIM_ON); // Update the progress bar value

    array[7] = random(0, 100);
    lv_bar_set_value(objects.bar_585, array[7], LV_ANIM_ON); // Update the progress bar value

    array[8] = random(0, 100);
    lv_bar_set_value(objects.bar_610, array[8], LV_ANIM_ON); // Update the progress bar value

    array[9] = random(0, 100);
    lv_bar_set_value(objects.bar_645, array[9], LV_ANIM_ON); // Update the progress bar value

    array[10] = random(0, 100);
    lv_bar_set_value(objects.bar_680, array[10], LV_ANIM_ON); // Update the progress bar value

    array[11] = random(0, 100);
    lv_bar_set_value(objects.bar_705, array[11], LV_ANIM_ON); // Update the progress bar value
    
    array[12] = random(0, 100);
    lv_bar_set_value(objects.bar_730, array[12], LV_ANIM_ON);
    
    array[13] = random(0, 100);
    lv_bar_set_value(objects.bar_760, array[13], LV_ANIM_ON);

    array[14] = random(0, 100);
    lv_bar_set_value(objects.bar_810, array[14], LV_ANIM_ON);

    array[15] = random(0, 100);
    lv_bar_set_value(objects.bar_860, array[15], LV_ANIM_ON);

    array[16] = random(0, 100);
    lv_bar_set_value(objects.bar_900, array[16], LV_ANIM_ON);
    
    array[17] = random(0, 100);
    lv_bar_set_value(objects.bar_940, array[17], LV_ANIM_ON);

    byte max_val = 0;
    for(int i = 0; i < 18; i++) {        
        if(array[i] > max_val) {
            max_val = array[i];
        }
    }

    char buffer[30];
    float intensity = 0;
    lv_label_set_text(objects.label_max_indicator, "NA");
    if (array[0] == max_val) {
        lv_obj_set_style_border_opa(objects.bar_410, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        intensity = (float)array[0] / 100;
        sprintf(buffer, "CH 1 - 410nm - I = %.2f", intensity);
        lv_label_set_text(objects.label_max_indicator, buffer);
    } else {
        lv_obj_set_style_border_opa(objects.bar_410, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    }

    if (array[1] == max_val) {
        lv_obj_set_style_border_opa(objects.bar_435, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        intensity = (float)array[1] / 100;
        sprintf(buffer, "CH 2 - 435nm - I = %.2f", intensity);
        lv_label_set_text(objects.label_max_indicator, buffer);
    } else {
        lv_obj_set_style_border_opa(objects.bar_435, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    }

    if (array[2] == max_val) {
        lv_obj_set_style_border_opa(objects.bar_460, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        intensity = (float)array[2] / 100;
        sprintf(buffer, "CH 3 - 460nm - I = %.2f", intensity);
        lv_label_set_text(objects.label_max_indicator, buffer);
    } else {
        lv_obj_set_style_border_opa(objects.bar_460, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    }

    if (array[3] == max_val) {
        lv_obj_set_style_border_opa(objects.bar_485, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        intensity = (float)array[3] / 100;
        sprintf(buffer, "CH 4 - 485nm - I = %.2f", intensity);
        lv_label_set_text(objects.label_max_indicator, buffer);
    } else {
        lv_obj_set_style_border_opa(objects.bar_485, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    }

    if (array[4] == max_val) {
        lv_obj_set_style_border_opa(objects.bar_510, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        intensity = (float)array[4] / 100;
        sprintf(buffer, "CH 5 - 510nm - I = %.2f", intensity);
        lv_label_set_text(objects.label_max_indicator, buffer);
    } else {
        lv_obj_set_style_border_opa(objects.bar_510, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    }

    if (array[5] == max_val) {
        lv_obj_set_style_border_opa( objects.bar_535, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        intensity = (float)array[5] / 100;
        sprintf(buffer, "CH 6 - 535nm - I = %.2f", intensity);
        lv_label_set_text(objects.label_max_indicator, buffer);
    } else {
        lv_obj_set_style_border_opa( objects.bar_535, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    }

    if (array[6] == max_val) {
        lv_obj_set_style_border_opa( objects.bar_560, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        intensity = (float)array[6] / 100;
        sprintf(buffer, "CH 7 - 560nm - I = %.2f", intensity);
        lv_label_set_text(objects.label_max_indicator, buffer);
    } else {
        lv_obj_set_style_border_opa( objects.bar_560, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    }

    if (array[7] == max_val) {
        lv_obj_set_style_border_opa( objects.bar_585, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        intensity = (float)array[7] / 100;
        sprintf(buffer, "CH 8 - 585nm - I = %.2f", intensity);
        lv_label_set_text(objects.label_max_indicator, buffer);
    } else {
        lv_obj_set_style_border_opa( objects.bar_585, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    }

    if (array[8] == max_val) {
        lv_obj_set_style_border_opa( objects.bar_610, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        intensity = (float)array[8] / 100;
        sprintf(buffer, "CH 9 - 610nm - I = %.2f", intensity);
        lv_label_set_text(objects.label_max_indicator, buffer);
    } else {
        lv_obj_set_style_border_opa( objects.bar_610, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    }

    if (array[9] == max_val) {
        lv_obj_set_style_border_opa( objects.bar_645, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        intensity = (float)array[9] / 100;
        sprintf(buffer, "CH 10 - 645nm - I = %.2f", intensity);
        lv_label_set_text(objects.label_max_indicator, buffer);
    } else {
        lv_obj_set_style_border_opa( objects.bar_645, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    }

    if (array[10] == max_val) {
        lv_obj_set_style_border_opa( objects.bar_680, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        intensity = (float)array[10] / 100;
        sprintf(buffer, "CH 11 - 680nm - I = %.2f", intensity);
        lv_label_set_text(objects.label_max_indicator, buffer);
    } else {
        lv_obj_set_style_border_opa( objects.bar_680, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    }

    if (array[11] == max_val) {
        lv_obj_set_style_border_opa( objects.bar_705, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        intensity = (float)array[11] / 100;
        sprintf(buffer, "CH 12 - 705nm - I = %.2f", intensity);
        lv_label_set_text(objects.label_max_indicator, buffer);
    } else {
        lv_obj_set_style_border_opa( objects.bar_705, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    }

    if (array[12] == max_val) {
        lv_obj_set_style_border_opa( objects.bar_730, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        intensity = (float)array[12] / 100;
        sprintf(buffer, "CH 13 - 730nm - I = %.2f", intensity);
        lv_label_set_text(objects.label_max_indicator, buffer);
    } else {
        lv_obj_set_style_border_opa( objects.bar_730, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    }

    if (array[13] == max_val) {
        lv_obj_set_style_border_opa( objects.bar_760, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        intensity = (float)array[13] / 100;
        sprintf(buffer, "CH 14 - 760nm - I = %.2f", intensity);
        lv_label_set_text(objects.label_max_indicator, buffer);
    } else {
        lv_obj_set_style_border_opa( objects.bar_760, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    }

    if (array[14] == max_val) {
        lv_obj_set_style_border_opa( objects.bar_810, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        intensity = (float)array[14] / 100;
        sprintf(buffer, "CH 15 - 810nm - I = %.2f", intensity);
        lv_label_set_text(objects.label_max_indicator, buffer);
    } else {
        lv_obj_set_style_border_opa( objects.bar_810, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    }

    if (array[15] == max_val) {
        lv_obj_set_style_border_opa( objects.bar_860, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        intensity = (float)array[15] / 100;
        sprintf(buffer, "CH 16 - 860nm - I = %.2f", intensity);
        lv_label_set_text(objects.label_max_indicator, buffer);
    } else {
        lv_obj_set_style_border_opa( objects.bar_860, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    }

    if (array[16] == max_val) {
        lv_obj_set_style_border_opa( objects.bar_900, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        intensity = (float)array[16] / 100;
        sprintf(buffer, "CH 17 - 900nm - I = %.2f", intensity);
        lv_label_set_text(objects.label_max_indicator, buffer);
    } else {
        lv_obj_set_style_border_opa( objects.bar_900, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    }

    if (array[17] == max_val) {
        lv_obj_set_style_border_opa( objects.bar_940, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        intensity = (float)array[17] / 100;
        sprintf(buffer, "CH 18 - 940nm - I = %.2f", intensity);
        lv_label_set_text(objects.label_max_indicator, buffer);
    } else {
        lv_obj_set_style_border_opa( objects.bar_940, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    }
  }
}

void setup() {
  String LVGL_Arduino = String("LVGL Library Version: ") + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  Serial.begin(115200);
  Serial.println(LVGL_Arduino);
  
  // Start LVGL
  lv_init();
  // Register print function for debugging
  lv_log_register_print_cb(log_print);

  // Start the SPI for the touchscreen and init the touchscreen
  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  // Set the Touchscreen rotation in landscape mode
  // Note: in some displays, the touchscreen might be upside down, so you might need to set the rotation to 0: touchscreen.setRotation(0);
  touchscreen.setRotation(2);

  // Create a display object
  lv_display_t * disp;
  // Initialize the TFT display using the TFT_eSPI library
  disp = lv_tft_espi_create(SCREEN_WIDTH, SCREEN_HEIGHT, draw_buf, sizeof(draw_buf));
  lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_270);
    
  // Initialize an LVGL input device object (Touchscreen)
  lv_indev_t * indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  // Set the callback function to read Touchscreen input
  lv_indev_set_read_cb(indev, touchscreen_read);

  ui_init();  // Initialize the user interface
}

void loop() {
  Runtime();
  ui_tick();
  lv_task_handler();  // let the GUI do its work
  lv_tick_inc(5);     // tell LVGL how much time has passed
  delay(5);           // let this time pass
}