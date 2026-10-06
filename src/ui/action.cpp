#include "actions.h"
#include <Arduino.h>
#include "screens.h"
#include "ui.h"
#include "sensor/sensor.h"
#include "nvs_manager/nvs_manager.h"
#include "app/app.h"

static csv::Record pendingLogRecord;
static bool pendingLog = false;

static void showLogMessage(const char* message) {
    lv_obj_t* box = lv_msgbox_create(nullptr);
    lv_obj_set_width(box, 280);
    lv_msgbox_add_title(box, "LOG");
    lv_msgbox_add_text(box, message);
    lv_msgbox_add_close_button(box);
    lv_obj_center(box);
}

///////// BUTTONS ACTIONS /////////
extern void action_button_live(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_RELEASED && liveState) {
        liveState = false; // Toggle the state
        lv_obj_set_style_bg_color(objects.button_live_main, lv_color_hex(0x108CF0), LV_PART_MAIN);
        lv_obj_set_style_text_color(objects.button_live_main, lv_color_hex(0xffffff), LV_PART_MAIN);
    }
    else if (code == LV_EVENT_RELEASED && !liveState) {
        liveState = true; // Toggle the state
        firstMeasureament = true;
        lv_obj_set_style_bg_color(objects.button_live_main, lv_color_hex(0x00FF5E), LV_PART_MAIN);
        lv_obj_set_style_text_color(objects.button_live_main, lv_color_hex(0x000000), LV_PART_MAIN);
    }
    else {}
}






extern void action_setup_icon(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_LONG_PRESSED) {
        loadScreen(SCREEN_ID_SETUP);
        float integrationTime = ((configActual.integracionCiclos + 1) * 2.78) * 2; 
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
        // Obtener el texto literal de la opción seleccionada (si lo necesitas)
        char buf[32];
        lv_roller_get_selected_str(objects.roller_white, buf, sizeof(buf));
        
        // Ejemplo de depuración por monitor serie
        printf("Índice seleccionado: %d, Texto: %s\n", sel_id, buf);
    }
}

extern void action_button_led_main(lv_event_t * e) {
    static bool LEDsState = false;
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_RELEASED){
        if (!LEDsState) {
            Serial.println("LEDs ON");
            Serial.println("Current White: " + String(configActual.currentWhite));
            Serial.println("Current IR: " + String(configActual.currentIR));
            Serial.println("Current UV: " + String(configActual.currentUV));
            LEDsState = true;
            lv_obj_set_style_bg_color(objects.button_led_main, lv_color_hex(0x00FF5E), LV_PART_MAIN);
            lv_obj_set_style_text_color(objects.button_led_main, lv_color_hex(0x000000), LV_PART_MAIN);
            if (configActual.currentWhite > 0) setWhiteLEDCurrent(configActual.currentWhite);
            else resetWhiteLEDCurrent();
            if (configActual.currentIR > 0) setIRLEDCurrent(configActual.currentIR);
            else resetIRLEDCurrent();
            if (configActual.currentUV > 0) setUVLEDCurrent(configActual.currentUV);
            else resetUVLEDCurrent();
        } else {
            Serial.println("LEDs OFF");
            resetWhiteLEDCurrent(); 
            resetIRLEDCurrent();
            resetUVLEDCurrent();
            LEDsState = false;
            lv_obj_set_style_bg_color(objects.button_led_main, lv_color_hex(0x108CF0), LV_PART_MAIN);
            lv_obj_set_style_text_color(objects.button_led_main, lv_color_hex(0xffffff), LV_PART_MAIN);
        }
    }
}

extern void action_slide_inegration_change(lv_event_t * e){
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_VALUE_CHANGED){
        float integrationTime = ((lv_slider_get_value(objects.slide_integration) + 1) * 2.78) * 2; // For reading 6 channels of every sensor, the integration time is doubled. 1 a 54 --> 11.2ms a 308.0ms
        char buffer[25];
        snprintf(buffer, sizeof(buffer), "INTEGRATION TIME: %.1fms", integrationTime);
        lv_label_set_text(objects.value_integration, buffer);
    }
}

extern void action_slide_measure_change(lv_event_t * e){
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_VALUE_CHANGED){
        float valueMeasuring = (float)lv_slider_get_value(objects.slide_measure);
        char buffer[25];
        snprintf(buffer, sizeof(buffer), "MEASURE PERIOD: %.1fs", valueMeasuring);
        lv_label_set_text(objects.value_measuring, buffer);
    }
}

extern void action_button_save_setup(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_RELEASED) {
        configActual.integracionCiclos = lv_slider_get_value(objects.slide_integration);
        setMaxADCvalue(constrain((configActual.integracionCiclos + 1) * 1024 - 1, 0, 65535)); // Update the max ADC value based on the new integration time
        Serial.println("New max ADC value: " + String(getMaxADCvalue()));
        configActual.tiempoEntreTomasMS = (lv_slider_get_value(objects.slide_measure)) * 1000;
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
    action_button_log(e);
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

extern void action_button_log(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_RELEASED) {
        if (pendingLog) return;
        uint16_t count = 0;
        const csv::Result result = csv::getRecordCount(count);
        if (result != csv::Result::Ok) {
            Serial.printf("[CSV] Cannot open LOG: %s\n", csv::resultMessage(result));
            showLogMessage("Storage unavailable. Check the serial terminal.");
            return;
        }
        if (count >= csv::MAX_RECORDS) {
            showLogMessage(csv::resultMessage(csv::Result::Full));
            return;
        }
        if (!copyLatestCsvRecord(pendingLogRecord)) {
            showLogMessage("Take a measurement before saving a log.");
            return;
        }
        pendingLog = true;
        lv_textarea_set_text(objects.textarea_keyboard, "");
        loadScreen(SCREEN_ID_KEYBOARD);
    }
}

extern void action_keyboard_ready(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CANCEL) {
        pendingLog = false;
        loadScreen(SCREEN_ID_MAIN);
        return;
    }
    if (code == LV_EVENT_READY && pendingLog) {
        const char* comment = lv_textarea_get_text(objects.textarea_keyboard);
        const csv::Result result = csv::append(pendingLogRecord, comment);
        if (result != csv::Result::Ok) {
            Serial.printf("[CSV] Save failed: %s\n", csv::resultMessage(result));
            showLogMessage(csv::resultMessage(result));
            return; // Keep the comment and snapshot for correction or cancellation.
        }
        pendingLog = false;
        uint16_t count = 0;
        csv::getRecordCount(count);
        Serial.printf("[CSV] Saved sample %lu (%u/%u records).\n",
                      static_cast<unsigned long>(pendingLogRecord.sampleId),
                      static_cast<unsigned>(count),
                      static_cast<unsigned>(csv::MAX_RECORDS));
        loadScreen(SCREEN_ID_MAIN);
        char message[48];
        snprintf(message, sizeof(message), "Saved: %u/%u records",
                 static_cast<unsigned>(count),
                 static_cast<unsigned>(csv::MAX_RECORDS));
        showLogMessage(message);
    }
}
