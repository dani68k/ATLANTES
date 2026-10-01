#include "actions.h"
#include <Arduino.h>
#include "screens.h"
#include "ui.h"
#include "sensor/sensor.h"
#include "nvs_manager/nvs_manager.h"

extern bool liveState;


extern void action_button_live(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_RELEASED && liveState) {
        liveState = false; // Toggle the state
        lv_obj_set_style_bg_color(objects.button_live_main, lv_color_hex(0x108CF0), LV_PART_MAIN);
        lv_obj_set_style_text_color(objects.button_live_main, lv_color_hex(0xffffff), LV_PART_MAIN);
    }
    else if (code == LV_EVENT_RELEASED && !liveState) {
        liveState = true; // Toggle the state
        lv_obj_set_style_bg_color(objects.button_live_main, lv_color_hex(0x00FF5E), LV_PART_MAIN);
        lv_obj_set_style_text_color(objects.button_live_main, lv_color_hex(0x000000), LV_PART_MAIN);
    }
    else {}
}

extern void action_setup_icon(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_LONG_PRESSED) {
        loadScreen(SCREEN_ID_SETUP);
        float valueIntegration = (float)configActual.integracionCiclos;
        float integrationTime = (valueIntegration + 1) * 2.80 * 2; //1 a 54 --> 11.2ms a 308.0ms
        char buffer[25];
        snprintf(buffer, sizeof(buffer), "INTEGRATION TIME: %.1fms", integrationTime);
        lv_label_set_text(objects.value_integration, buffer);
        lv_slider_set_value(objects.slide_integration, configActual.integracionCiclos, LV_ANIM_OFF);

        float valueMeasuring = (float)configActual.tiempoEntreTomasMS / 1000.0;
        snprintf(buffer, sizeof(buffer), "MEASURE PERIOD: %.1fs", valueMeasuring);
        lv_label_set_text(objects.value_measuring, buffer);
        lv_slider_set_value(objects.slide_measure, configActual.tiempoEntreTomasMS / 1000, LV_ANIM_OFF);

        int valueGain = configActual.ganancia;
        if (valueGain == 0) snprintf(buffer, sizeof(buffer), "GAIN: x1");
        if (valueGain == 1) snprintf(buffer, sizeof(buffer), "GAIN: x3.7");
        if (valueGain == 2) snprintf(buffer, sizeof(buffer), "GAIN: x16");
        if (valueGain == 3) snprintf(buffer, sizeof(buffer), "GAIN: x64");
        lv_label_set_text(objects.value_gain, buffer);
        lv_slider_set_value(objects.slide_gain, configActual.ganancia, LV_ANIM_OFF);

        lv_roller_set_selected(objects.roller_white, configActual.currentWhite, LV_ANIM_OFF);
        lv_roller_set_selected(objects.roller_ir, configActual.currentIR, LV_ANIM_OFF);
        lv_roller_set_selected(objects.roller_uv, configActual.currentUV, LV_ANIM_OFF);

    }
}

extern void action_button_back(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_RELEASED) {
        loadScreen(SCREEN_ID_MAIN);
    }
}

extern void action_white_roller_change(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_VALUE_CHANGED) {
        // Obtener el índice seleccionado (0 para la primera opción, 1 para la segunda...)
        uint16_t sel_id = lv_roller_get_selected(objects.roller_white);
        if (sel_id == 1) configActual.currentWhite = 1;
        else if (sel_id == 2) configActual.currentWhite = 2;
        else if (sel_id == 3) configActual.currentWhite = 3;
        else if (sel_id == 4) configActual.currentWhite = 4;
        // Obtener el texto literal de la opción seleccionada (si lo necesitas)
        char buf[32];
        lv_roller_get_selected_str(objects.roller_white, buf, sizeof(buf));
        
        // Ejemplo de depuración por monitor serie
        printf("Índice seleccionado: %d, Texto: %s\n", sel_id, buf);
    }
}

extern void action_button_led_main(lv_event_t * e) {
    static bool whiteLEDState = false;
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_RELEASED){
        if (!whiteLEDState) {
            Serial.println("White LED ON");
            whiteLEDState = true;
            lv_obj_set_style_bg_color(objects.button_led_main, lv_color_hex(0x00FF5E), LV_PART_MAIN);
            lv_obj_set_style_text_color(objects.button_led_main, lv_color_hex(0x000000), LV_PART_MAIN);
            setWhiteLEDCurrent(configActual.currentWhite);
        } else {
            Serial.println("White LED OFF");
            resetWhiteLEDCurrent(); 
            whiteLEDState = false;
            lv_obj_set_style_bg_color(objects.button_led_main, lv_color_hex(0x108CF0), LV_PART_MAIN);
            lv_obj_set_style_text_color(objects.button_led_main, lv_color_hex(0xffffff), LV_PART_MAIN);
        }
    }
}

extern void action_slide_inegration_change(lv_event_t * e){
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_VALUE_CHANGED){
        float valueIntegration = (float)lv_slider_get_value(objects.slide_integration);
        float integrationTime = (valueIntegration + 1) * 2.80 * 2;
        char buffer[25];
        snprintf(buffer, sizeof(buffer), "INTEGRATION TIME: %.1fms", integrationTime);
        lv_label_set_text(objects.value_integration, buffer);
    }
}

extern void action_slide_measure_change(lv_event_t * e){
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_VALUE_CHANGED){
        float valueMeasuring = (float)lv_slider_get_value(objects.slide_measure) * 0.5;
        char buffer[25];
        snprintf(buffer, sizeof(buffer), "MEASURE PERIOD: %.1fs", valueMeasuring);
        lv_label_set_text(objects.value_measuring, buffer);
    }
}

extern void action_button_save_setup(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_RELEASED) {
        configActual.integracionCiclos = lv_slider_get_value(objects.slide_integration);
        configActual.tiempoEntreTomasMS = (lv_slider_get_value(objects.slide_measure) * 0.5 ) * 1000;
        configActual.currentWhite = lv_roller_get_selected(objects.roller_white);
        configActual.currentIR = lv_roller_get_selected(objects.roller_ir);
        configActual.currentUV = lv_roller_get_selected(objects.roller_uv);
        configActual.ganancia = lv_slider_get_value(objects.slide_gain);
        nvs_guardar_configuracion();
    }
}

extern void action_ir_roller_change(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_VALUE_CHANGED) {
        // Obtener el índice seleccionado (0 para la primera opcion, 1 para la segunda...)
        uint16_t sel_id = lv_roller_get_selected(objects.roller_ir);
        
        // Obtener el texto literal de la opcion seleccionada (si lo necesitas)
        char buf[32];
        lv_roller_get_selected_str(objects.roller_ir, buf, sizeof(buf));
        
        // Ejemplo de depuracion por monitor serie
        printf("Indice seleccionado: %d, Texto: %s\n", sel_id, buf);
    }
}

extern void action_uv_roller_change(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_VALUE_CHANGED) {
        // Obtener el índice seleccionado (0 para la primera opcion, 1 para la segunda...)
        uint16_t sel_id = lv_roller_get_selected(objects.roller_uv);
        
        // Obtener el texto literal de la opcion seleccionada (si lo necesitas)
        char buf[32];
        lv_roller_get_selected_str(objects.roller_uv, buf, sizeof(buf));
        
        // Ejemplo de depuracion por monitor serie
        printf("Indice seleccionado: %d, Texto: %s\n", sel_id, buf);
    }
}

extern void action_button_log_main(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_RELEASED) {

    }
}

extern void action_slide_gain_change(lv_event_t * e){
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_VALUE_CHANGED){
        int valueGain = lv_slider_get_value(objects.slide_gain);
        char buffer[25];
        if (valueGain == 0) snprintf(buffer, sizeof(buffer), "GAIN: x1");
        if (valueGain == 1) snprintf(buffer, sizeof(buffer), "GAIN: x3.7");
        if (valueGain == 2) snprintf(buffer, sizeof(buffer), "GAIN: x16");
        if (valueGain == 3) snprintf(buffer, sizeof(buffer), "GAIN: x64");
        lv_label_set_text(objects.value_gain, buffer);
    }
}