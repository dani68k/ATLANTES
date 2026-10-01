#include <Arduino.h>
#include "app.h"
#include "sensor/sensor.h"
#include <lvgl.h>
#include "ui/screens.h"
#include "nvs_manager/nvs_manager.h"

bool liveState = false;

#define MAX_INTENSITY 65535.00

void Runtime() {
  static unsigned long lastTime = 0;
  unsigned long currentTime = millis();

  // 1. CONTROL DE DISPARO DINÁMICO (Usa el tiempo real del SETUP)
  if (!liveState) {
    lv_obj_remove_flag(objects.setup_icon, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(objects.label_info_test, LV_OBJ_FLAG_HIDDEN);
    return; // Si no está activo el modo live, salimos de inmediato
  }
  
  lv_obj_add_flag(objects.setup_icon, LV_OBJ_FLAG_HIDDEN);
  lv_obj_remove_flag(objects.label_info_test, LV_OBJ_FLAG_HIDDEN);
  // Compara contra el parámetro guardado en tu estructura modular NVS
  if (currentTime - lastTime >= configActual.tiempoEntreTomasMS) { 
    lastTime = currentTime; // Reiniciamos el cronómetro en el instante del disparo
    if (!isDataReady()) {
      // Disparamos la lectura física. La función startMeasurements() aplica
      // configActual.ganancia y configActual.integracionCiclos internamente antes de ordenar ONE_SHOT
      startMeasurements();
    } 
  }  

  // 2. PROCESAMIENTO CUANDO EL POLLING DE DATOS ESTÁ LISTO
  if (isDataReady()) {
    
    // Arrays locales para almacenar los datos de esta toma atómica
    float rawData[18];
    byte mappedValues[18];

    // --- FASE A: Absorción limpia de datos I2C (Una sola lectura por canal) ---
    rawData[0]  = getDataChannel1();  mappedValues[0]  = mapFloatToByte(rawData[0],  MAX_INTENSITY);
    rawData[1]  = getDataChannel2();  mappedValues[1]  = mapFloatToByte(rawData[1],  MAX_INTENSITY);
    rawData[2]  = getDataChannel3();  mappedValues[2]  = mapFloatToByte(rawData[2],  MAX_INTENSITY);
    rawData[3]  = getDataChannel4();  mappedValues[3]  = mapFloatToByte(rawData[3],  MAX_INTENSITY);
    rawData[4]  = getDataChannel5();  mappedValues[4]  = mapFloatToByte(rawData[4],  MAX_INTENSITY);
    rawData[5]  = getDataChannel6();  mappedValues[5]  = mapFloatToByte(rawData[5],  MAX_INTENSITY);
    rawData[6]  = getDataChannel7();  mappedValues[6]  = mapFloatToByte(rawData[6],  MAX_INTENSITY);
    rawData[7]  = getDataChannel8();  mappedValues[7]  = mapFloatToByte(rawData[7],  MAX_INTENSITY);
    rawData[8]  = getDataChannel9();  mappedValues[8]  = mapFloatToByte(rawData[8],  MAX_INTENSITY);
    rawData[9]  = getDataChannel10(); mappedValues[9]  = mapFloatToByte(rawData[9],  MAX_INTENSITY);
    rawData[10] = getDataChannel11(); mappedValues[10] = mapFloatToByte(rawData[10], MAX_INTENSITY);
    rawData[11] = getDataChannel12(); mappedValues[11] = mapFloatToByte(rawData[11], MAX_INTENSITY);
    rawData[12] = getDataChannel13(); mappedValues[12] = mapFloatToByte(rawData[12], MAX_INTENSITY);
    rawData[13] = getDataChannel14(); mappedValues[13] = mapFloatToByte(rawData[13], MAX_INTENSITY);
    rawData[14] = getDataChannel15(); mappedValues[14] = mapFloatToByte(rawData[14], MAX_INTENSITY);
    rawData[15] = getDataChannel16(); mappedValues[15] = mapFloatToByte(rawData[15], MAX_INTENSITY);
    rawData[16] = getDataChannel17(); mappedValues[16] = mapFloatToByte(rawData[16], MAX_INTENSITY);
    rawData[17] = getDataChannel18(); mappedValues[17] = mapFloatToByte(rawData[17], MAX_INTENSITY);

    // --- FASE B: Actualización inmediata de los gráficos del Histograma ---
    lv_bar_set_value(objects.bar_410, mappedValues[0],  LV_ANIM_ON);
    lv_bar_set_value(objects.bar_435, mappedValues[1],  LV_ANIM_ON);
    lv_bar_set_value(objects.bar_460, mappedValues[2],  LV_ANIM_ON);
    lv_bar_set_value(objects.bar_485, mappedValues[3],  LV_ANIM_ON);
    lv_bar_set_value(objects.bar_510, mappedValues[4],  LV_ANIM_ON);
    lv_bar_set_value(objects.bar_535, mappedValues[5],  LV_ANIM_ON);
    lv_bar_set_value(objects.bar_560, mappedValues[6],  LV_ANIM_ON);
    lv_bar_set_value(objects.bar_585, mappedValues[7],  LV_ANIM_ON);
    lv_bar_set_value(objects.bar_610, mappedValues[8],  LV_ANIM_ON);
    lv_bar_set_value(objects.bar_645, mappedValues[9],  LV_ANIM_ON);
    lv_bar_set_value(objects.bar_680, mappedValues[10], LV_ANIM_ON);
    lv_bar_set_value(objects.bar_705, mappedValues[11], LV_ANIM_ON);
    lv_bar_set_value(objects.bar_730, mappedValues[12], LV_ANIM_ON);
    lv_bar_set_value(objects.bar_760, mappedValues[13], LV_ANIM_ON);
    lv_bar_set_value(objects.bar_810, mappedValues[14], LV_ANIM_ON);
    lv_bar_set_value(objects.bar_860, mappedValues[15], LV_ANIM_ON);
    lv_bar_set_value(objects.bar_900, mappedValues[16], LV_ANIM_ON);
    lv_bar_set_value(objects.bar_940, mappedValues[17], LV_ANIM_ON);

    // --- FASE C: Determinar el Pico Máximo Espectral ---
    byte max_val = 0;
    for(int i = 0; i < 18; i++) {        
        if(mappedValues[i] > max_val) {
            max_val = mappedValues[i];
        }
    }

    // --- FASE D: Evaluación lógica simétrica de alertas y colores por canal ---
    char buffer[100];
    lv_label_set_text(objects.label_max_indicator, "NA");
    
    // Definimos punteros para simplificar la evaluación en bloques repetitivos de tus 18 canales
    lv_obj_t* labels_x[18] = {objects.label_x_1, objects.label_x_2, objects.label_x_3, objects.label_x_4,
                              objects.label_x_5, objects.label_x_6, objects.label_x_7, objects.label_x_8,
                              objects.label_x_9, objects.label_x_10, objects.label_x_11, objects.label_x_12,
                              objects.label_x_13, objects.label_x_14, objects.label_x_15, objects.label_x_16,
                              objects.label_x_17, objects.label_x_18};
    int longitudesOnda[18] = {410, 435, 460, 490, 520, 550, 580, 610, 640, 670, 700, 730, 760, 790, 820, 850, 880, 910};

    // Procesamos los primeros 4 canales que pasaste en tu ejemplo
    for(int i = 0; i < 18; i++) {
        float intensity = (float)mappedValues[i] / 100.0f;

        // Condición 1: SATURACIÓN CRÍTICA (Física o porcentual > 100%)
        if (intensity > 1.0f) {
            lv_obj_set_style_text_color(labels_x[i], lv_color_hex(0xFF0000), LV_PART_MAIN); // ROJO
        }
        // Condición 2: ES EL PICO MÁXIMO VÁLIDO
        else if (mappedValues[i] == max_val && intensity >= 0.01f) {
            snprintf(buffer, sizeof(buffer), "CH%d - λ = %dnm - I = %.2f\r\nRaw value = %.1f", i+1, longitudesOnda[i], intensity, rawData[i]);
            lv_label_set_text(objects.label_max_indicator, buffer);
            lv_obj_set_style_text_color(labels_x[i], lv_color_hex(0xFF8000), LV_PART_MAIN); // NARANJA CORPORATIVO DE 6 DÍGITOS
        }
        // Condición 3: LECTURA NORMAL O SEÑAL DÉBIL
        else {
            lv_obj_set_style_text_color(labels_x[i], lv_color_hex(0xFFFFFF), LV_PART_MAIN); // BLANCO
        }
    }
  }
}
