#ifndef NVS_MANAGER_H
#define NVS_MANAGER_H

#include <Arduino.h>

// Estructura que agrupa todos tus parámetros (todos de tipo entero)
struct ConfiguracionSetup {
  uint8_t currentWhite;        // 0 a 3 (12.5mA a 100mA)
  uint8_t currentIR;           // 0 a 2 (12.5mA a 50mA)
  uint8_t currentUV;           // 0 a 1 (12.5mA a 25mA)
  uint8_t integracionCiclos;   // 1 a 54 (Máx ~302.4ms)
  uint8_t ganancia;            // Constantes de la librería (1X, 3.7X, 16X, 64X)
  uint32_t tiempoEntreTomasMS; // 1000ms a 10000 ms
  bool autoLEDs;                // true o false
};

// Declaramos la variable global como 'extern' para que sea visible en otros archivos .cpp
extern volatile ConfiguracionSetup configActual;

// Declaración de funciones públicas
void nvs_inicializar(void);
void nvs_cargar_configuracion(void);
void nvs_guardar_configuracion(void);

#endif // NVS_MANAGER_H
