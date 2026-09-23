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
    char buffer[20];
    int32_t val = random(0, 100); // Generate a random value between 0 and 100
    lv_bar_set_value(objects.bar_410, val, LV_ANIM_ON); // Update the progress bar value
    val = random(0, 100);
    lv_bar_set_value(objects.bar_435, val, LV_ANIM_ON); // Update the progress bar value
    val = random(0, 100);
    lv_bar_set_value(objects.bar_460, val, LV_ANIM_ON); // Update the progress bar value
    val = random(0, 100);
    lv_bar_set_value(objects.bar_485, val, LV_ANIM_ON); // Update the progress bar value
    val = random(0, 100);
    lv_bar_set_value(objects.bar_510, val, LV_ANIM_ON); // Update the progress bar value
    val = random(0, 100);
    lv_bar_set_value(objects.bar_535, val, LV_ANIM_ON); // Update the progress bar value
    val = random(0, 100);
    lv_bar_set_value(objects.bar_560, val, LV_ANIM_ON); // Update the progress bar value
    val = random(0, 100);
    lv_bar_set_value(objects.bar_585, val, LV_ANIM_ON); // Update the progress bar value
    val = random(0, 100);
    lv_bar_set_value(objects.bar_610, val, LV_ANIM_ON); // Update the progress bar value
    val = random(0, 100);
    lv_bar_set_value(objects.bar_645, val, LV_ANIM_ON); // Update the progress bar value
    val = random(0, 100);
    lv_bar_set_value(objects.bar_680, val, LV_ANIM_ON); // Update the progress bar value
    val = random(0, 100);
    lv_bar_set_value(objects.bar_730, val, LV_ANIM_ON); // Update the progress bar value
    val = random(0, 100);
    lv_bar_set_value(objects.bar_760, val, LV_ANIM_ON); // Update the progress bar value
    val = random(0, 100);
    lv_bar_set_value(objects.bar_810, val, LV_ANIM_ON); // Update the progress bar value
    val = random(0, 100);
    lv_bar_set_value(objects.bar_860, val, LV_ANIM_ON); // Update the progress bar value
    val = random(0, 100);
    lv_bar_set_value(objects.bar_900, val, LV_ANIM_ON); // Update the progress bar value
    val = random(0, 100);
    lv_bar_set_value(objects.bar_940, val, LV_ANIM_ON); // Update the progress bar value  
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