#include "nvs_manager.h"
#include <Preferences.h>

extern bool ledAuto;

// Instancia privada del gestor de preferencias
static Preferences preferences;

// Definición física de la variable global compartida
volatile ConfiguracionSetup configActual;

// Valores por defecto de fábrica (por si la memoria está vacía)
const uint8_t  DEFAULT_WHITE = 1;      // 25mA (50%)
const uint8_t  DEFAULT_IR    = 1;      // 25mA
const uint8_t  DEFAULT_UV    = 1;      // 25mA
const uint8_t  DEFAULT_INTEG = 10;     // 10 ciclos (~56ms)
const uint8_t  DEFAULT_GAIN  = 0;      // Equivalente a AS7265X_GAIN_1X
const uint32_t DEFAULT_TIME  = 1000;    // 1 segundo entre tomas
const bool     DEFAULT_AUTO  = false;

void nvs_inicializar(void) {
  // Carga inicial al arrancar el microcontrolador
  nvs_cargar_configuracion();
}

void nvs_cargar_configuracion(void) {
  // Abre el namespace "setup_sensor" en modo SÓLO LECTURA (true)
  preferences.begin("setup_sensor", true);

  // Recupera los valores de la memoria NVS, usando los valores por defecto si no existen
  configActual.currentWhite       = preferences.getUChar("led_w", DEFAULT_WHITE);
  configActual.currentIR          = preferences.getUChar("led_ir", DEFAULT_IR);
  configActual.currentUV          = preferences.getUChar("led_uv", DEFAULT_UV);
  configActual.integracionCiclos  = preferences.getUChar("integ",  DEFAULT_INTEG);
  configActual.ganancia           = preferences.getUChar("gain",   DEFAULT_GAIN);
  configActual.tiempoEntreTomasMS = preferences.getUInt("t_medida", DEFAULT_TIME);
  configActual.autoLEDs           = preferences.getBool("auto", DEFAULT_AUTO);
  if (configActual.autoLEDs) ledAuto = true;
  else ledAuto = false;

  //Valores por terminal
  Serial.println("[NVS] Configuración cargada desde NVS:");
  Serial.print("   Led White: "); Serial.println(configActual.currentWhite);
  Serial.print("   Led IR: "); Serial.println(configActual.currentIR);
  Serial.print("   Led UV: "); Serial.println(configActual.currentUV);
  Serial.print("   Integ: "); Serial.println(configActual.integracionCiclos);
  Serial.print("   Gain: "); Serial.println(configActual.ganancia);
  Serial.print("   Time: "); Serial.println(configActual.tiempoEntreTomasMS);
  Serial.print("   Auto LEDs: "); Serial.println(configActual.autoLEDs ? "ON" : "OFF");
  preferences.end();
}

void nvs_guardar_configuracion(void) {
  // Abre el namespace en modo LECTURA/ESCRITURA (false)
  preferences.begin("setup_sensor", false);

  // Guarda las variables en la memoria Flash
  preferences.putUChar("led_w", configActual.currentWhite);
  preferences.putUChar("led_ir", configActual.currentIR);
  preferences.putUChar("led_uv", configActual.currentUV);
  preferences.putUChar("integ",  configActual.integracionCiclos);
  preferences.putUChar("gain",   configActual.ganancia);
  preferences.putUInt("t_medida", configActual.tiempoEntreTomasMS);
  preferences.putBool("auto", configActual.autoLEDs);

  preferences.end();
  Serial.println("[NVS] Configuración guardada correctamente.");
}
