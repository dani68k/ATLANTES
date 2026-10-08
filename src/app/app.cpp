#include <Arduino.h>
#include "app.h"
#include "sensor/sensor.h"
#include <lvgl.h>
#include "ui/screens.h"
#include "ui/actions.h"
#include "nvs_manager/nvs_manager.h"

bool liveState = false;
bool measureamentInProgress = false;
bool firstMeasureament = false;
extern bool LEDsState;

static constexpr unsigned long MEASUREMENT_TIMEOUT_MS = 5000;

#define MAX_INTENSITY 65535.00
#define TOLERANCE 1.0 

uint16_t rawData[18];
float calibratedData[18];
uint16_t previousRawData[18] = {0};

static csv::Record latestCsvRecord;
static bool latestCsvRecordAvailable = false;

bool copyLatestCsvRecord(csv::Record& record) {
    if (!latestCsvRecordAvailable) return false;
    record = latestCsvRecord;
    return true;
}

lv_obj_t *array_bars[18] ;
lv_obj_t *array_labels[18];
int longitudesOnda[18] = {410, 435, 460, 485, 510, 535, 560, 585, 610, 645, 680, 705, 730, 760, 810, 860, 900, 940};

void initUIObjectArrays() {
    array_bars[0] = objects.bar_410;
    array_bars[1] = objects.bar_435;
    array_bars[2] = objects.bar_460;
    array_bars[3] = objects.bar_485;
    array_bars[4] = objects.bar_510;
    array_bars[5] = objects.bar_535;
    array_bars[6] = objects.bar_560;
    array_bars[7] = objects.bar_585;
    array_bars[8] = objects.bar_610;
    array_bars[9] = objects.bar_645;
    array_bars[10] = objects.bar_680;
    array_bars[11] = objects.bar_705;
    array_bars[12] = objects.bar_730;
    array_bars[13] = objects.bar_760;
    array_bars[14] = objects.bar_810;
    array_bars[15] = objects.bar_860;
    array_bars[16] = objects.bar_900;
    array_bars[17] = objects.bar_940;

    array_labels[0] = objects.label_x_1;
    array_labels[1] = objects.label_x_2;
    array_labels[2] = objects.label_x_3;
    array_labels[3] = objects.label_x_4;
    array_labels[4] = objects.label_x_5;
    array_labels[5] = objects.label_x_6;
    array_labels[6] = objects.label_x_7;
    array_labels[7] = objects.label_x_8;
    array_labels[8] = objects.label_x_9;
    array_labels[9] = objects.label_x_10;
    array_labels[10] = objects.label_x_11;
    array_labels[11] = objects.label_x_12;
    array_labels[12] = objects.label_x_13;
    array_labels[13] = objects.label_x_14;
    array_labels[14] = objects.label_x_15;
    array_labels[15] = objects.label_x_16;
    array_labels[16] = objects.label_x_17;
    array_labels[17] = objects.label_x_18;

    // Keep runtime keyboard settings outside the EEZ-generated screen files.
    lv_textarea_set_one_line(objects.textarea_keyboard, true);
    lv_textarea_set_max_length(objects.textarea_keyboard, csv::MAX_LABEL_BYTES);
    lv_textarea_set_placeholder_text(objects.textarea_keyboard, "Sample comment");
    lv_obj_add_event_cb(objects.keyboard_keyboard, action_keyboard_ready,
                        LV_EVENT_CANCEL, nullptr);

}

struct SpectrumAnalysis {
    uint16_t maxRaw = 0;
    float maxCalibrated = 0.0f;
    int calibratedMaxIndex = -1; // No positive calibrated maximum found.
};

static SpectrumAnalysis analyzeSpectrum() {
    SpectrumAnalysis analysis;
    for (int i = 0; i < 18; i++) {
        if (rawData[i] > analysis.maxRaw) {
            analysis.maxRaw = rawData[i];
        }
        if (calibratedData[i] > analysis.maxCalibrated) {
            analysis.maxCalibrated = calibratedData[i];
            analysis.calibratedMaxIndex = i;
        }
    }
    return analysis;
}

static void updateSpectrumUI(const SpectrumAnalysis& analysis) {
    uint16_t maxADCvalue = getMaxADCvalue();
    // Serial.println("[SENSOR] Max ADC value: " + String(maxADCvalue));
    // Update the bars if any value has changed
    for(int i = 0; i < 18; i++) {
        uint32_t barValue = map(rawData[i], 0, maxADCvalue, 0, 100);
        if (rawData[i] > previousRawData[i] * (1.0 + TOLERANCE / 100.0) || rawData[i] < previousRawData[i] * (1.0 - TOLERANCE / 100.0)) {
            lv_bar_set_value(array_bars[i], barValue, LV_ANIM_ON);
            previousRawData[i] = rawData[i]; // Update the previous value for the next comparison
        }
        if (barValue >= 99) { // SATURATION
            lv_obj_set_style_text_color(array_labels[i], lv_color_hex(0xFF0000), LV_PART_MAIN); // ROJO
        } else if (barValue >= 90 && barValue < 99) { // WARNING
            lv_obj_set_style_text_color(array_labels[i], lv_color_hex(0xFF8000), LV_PART_MAIN); // NARANJA
        } else if (rawData[i] == analysis.maxRaw) {
            lv_obj_set_style_text_color(array_labels[i], lv_color_hex(0x00FF5E), LV_PART_MAIN); // VERD
        } else { // NORMAL READING
            lv_obj_set_style_text_color(array_labels[i], lv_color_hex(0xFFFFFF), LV_PART_MAIN); // BLANCO
        }
    }

    char buffer_label_indicator[60] = "Not data read yet\r";
    if (analysis.calibratedMaxIndex >= 0) {
        const int i = analysis.calibratedMaxIndex;
        uint16_t gainValue = configActual.ganancia;
        if (configActual.ganancia == 0) gainValue = 1;
        else if (configActual.ganancia == 1) gainValue = 3.7;
        else if (configActual.ganancia == 2) gainValue = 16;
        else if (configActual.ganancia == 3) gainValue = 64;

        float timeIntegrationValue = (configActual.integracionCiclos + 1) * 5.56; // in ms 2.78 * 2 reading 6 channels of 3 sensors

        if (analysis.maxCalibrated >= 65535.00) {
            snprintf(buffer_label_indicator, sizeof(buffer_label_indicator),
                     "CH%d - λ = %dnm - BLINDED\rx%d - %.1fms - %ds - %.2fºC", 
                     i + 1, longitudesOnda[i], 
                     gainValue, 
                     timeIntegrationValue, 
                     getTemperatureAverage());
        } else {
            snprintf(buffer_label_indicator, sizeof(buffer_label_indicator),
                     "CH%d - λ = %dnm - %.1f\rx%d - %.1fms - %ds - %.2fºC", 
                     i + 1, longitudesOnda[i],
                     analysis.maxCalibrated,
                     gainValue,
                     timeIntegrationValue,
                     configActual.tiempoEntreTomasMS / 1000,
                     getTemperatureAverage());
        }
    }
    lv_label_set_text(objects.label_max_indicator, buffer_label_indicator);
}

void Runtime() {
    static unsigned long lastTime = 0;
    unsigned long currentTime = millis();

    // 1. CONTROL DE DISPARO DINÁMICO (Usa el tiempo real del SETUP)
    if (!liveState && !measureamentInProgress && !LEDsState) {
        lv_obj_set_hidden(objects.setup_icon, false);
    } else {
        lv_obj_set_hidden(objects.setup_icon, true);
    }
    // Compara contra el parámetro guardado en tu estructura modular NVS
    if (liveState && !measureamentInProgress &&
    (firstMeasureament || currentTime - lastTime >= configActual.tiempoEntreTomasMS)) {
			if (firstMeasureament) {
                latestCsvRecordAvailable = false;
				for (int i = 0; i < 18; i++) {
					previousRawData[i] = 0;
					lv_bar_set_value(array_bars[i], 0, LV_ANIM_OFF);//claer values of all bars
			    }
			}
            startMeasurements();
			measureamentInProgress = true;
			firstMeasureament = false;
			lastTime = millis();
    }  

    if (measureamentInProgress &&
        millis() - lastTime >= MEASUREMENT_TIMEOUT_MS) {
        measureamentInProgress = false;
        liveState = false;
        firstMeasureament = false;
        lv_obj_set_style_bg_color(objects.button_live_main, lv_color_hex(0x108CF0), LV_PART_MAIN);
        lv_obj_set_style_text_color(objects.button_live_main, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
        lv_obj_remove_flag(objects.setup_icon, LV_OBJ_FLAG_HIDDEN);
        Serial.println("[SENSOR] Measurement timeout (5000 ms). LIVE stopped.");
        return;
    }

    if (measureamentInProgress && isDataReady()) {        
        // raw data 
        uint32_t timestamp = millis();
        readRawChannels(rawData);
        uint32_t elapsed = millis() - timestamp;
        // Serial.printf("[SENSOR] Data read in %lu ms\n", elapsed);

        // Read calibrated channels before calculating or displaying the sample.
        timestamp = millis();
        readCalibratedChannels(calibratedData);
        elapsed = millis() - timestamp;
        // Serial.printf("[SENSOR] Calibrated data read in %lu ms\n", elapsed);

        // Publish only after both sets of channels have been collected.
        ++latestCsvRecord.sampleId;
        //WHITE LED
        if (configActual.currentWhite == 0) snprintf(latestCsvRecord.whiteLed, sizeof(latestCsvRecord.whiteLed), "OFF"); 
        else if (configActual.currentWhite == 1) snprintf(latestCsvRecord.whiteLed, sizeof(latestCsvRecord.whiteLed), "%d%%", 25);
        else if (configActual.currentWhite == 2) snprintf(latestCsvRecord.whiteLed, sizeof(latestCsvRecord.whiteLed), "%d%%", 50);
        else if (configActual.currentWhite == 3) snprintf(latestCsvRecord.whiteLed, sizeof(latestCsvRecord.whiteLed), "%d%%", 75);
        else if (configActual.currentWhite == 4) snprintf(latestCsvRecord.whiteLed, sizeof(latestCsvRecord.whiteLed), "%d%%", 100);
        //UV LED
        if (configActual.currentUV == 0) snprintf(latestCsvRecord.uvLed, sizeof(latestCsvRecord.uvLed), "OFF"); 
        else if (configActual.currentUV == 1) snprintf(latestCsvRecord.uvLed, sizeof(latestCsvRecord.uvLed), "%d%%", 25);
        else if (configActual.currentUV == 2) snprintf(latestCsvRecord.uvLed, sizeof(latestCsvRecord.uvLed), "%d%%", 50);
        else if (configActual.currentUV == 3) snprintf(latestCsvRecord.uvLed, sizeof(latestCsvRecord.uvLed), "%d%%", 75);
        else if (configActual.currentUV == 4) snprintf(latestCsvRecord.uvLed, sizeof(latestCsvRecord.uvLed), "%d%%", 100);
        //IR LED
        if (configActual.currentIR == 0) snprintf(latestCsvRecord.irLed, sizeof(latestCsvRecord.irLed), "OFF"); 
        else if (configActual.currentIR == 1) snprintf(latestCsvRecord.irLed, sizeof(latestCsvRecord.irLed), "%d%%", 25);
        else if (configActual.currentIR == 2) snprintf(latestCsvRecord.irLed, sizeof(latestCsvRecord.irLed), "%d%%", 50);
        else if (configActual.currentIR == 3) snprintf(latestCsvRecord.irLed, sizeof(latestCsvRecord.irLed), "%d%%", 75);
        else if (configActual.currentIR == 4) snprintf(latestCsvRecord.irLed, sizeof(latestCsvRecord.irLed), "%d%%", 100);
        //GAIN
        if (configActual.ganancia == 0) snprintf(latestCsvRecord.gain, sizeof(latestCsvRecord.gain), "x1");
        else if (configActual.ganancia == 1) snprintf(latestCsvRecord.gain, sizeof(latestCsvRecord.gain), "x3.7");
        else if (configActual.ganancia == 2) snprintf(latestCsvRecord.gain, sizeof(latestCsvRecord.gain), "x16");
        else if (configActual.ganancia == 3) snprintf(latestCsvRecord.gain, sizeof(latestCsvRecord.gain), "x64");
        //INTEGRATION TIME
        latestCsvRecord.integrationTimeMs = (configActual.integracionCiclos + 1) * 2.78 * 2; // in ms
        //MEASURE PERIOD
        latestCsvRecord.measureTime = configActual.tiempoEntreTomasMS / 1000; // in seconds
        //SENSOR TEMPERATURE
        latestCsvRecord.temperature = getTemperatureAverage();
        //RAW AND CALIBRATED DATA
        for (uint8_t i = 0; i < csv::CHANNEL_COUNT; ++i) {
            latestCsvRecord.raw[i] = rawData[i];
            latestCsvRecord.calibrated[i] = calibratedData[i];
        }
        latestCsvRecordAvailable = true;

        const SpectrumAnalysis analysis = analyzeSpectrum();
        // Serial.println("[SENSOR] Max raw value: " + String(analysis.maxRaw));
        // Serial.println("[SENSOR] Max value: " + String(analysis.maxCalibrated, 2));
        updateSpectrumUI(analysis);

        measureamentInProgress = false; // Reset the flag after processing the data    
    }
}
