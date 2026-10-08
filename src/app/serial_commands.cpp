#include "app.h"
#include <string.h>
#include "sensor/sensor.h"

namespace {
char command[32];
size_t commandLength = 0;
bool invalidCommand = false;

void executeCommand() {
    command[commandLength] = '\0';
    char* text = command;
    while (*text == ' ' || *text == '\t') ++text;
    char* end = command + commandLength;
    while (end > text && (end[-1] == ' ' || end[-1] == '\t')) --end;
    *end = '\0';
    if (!*text) return;

    // if (strcmp(text, "csv") != 0) {
    //     Serial.println("[SERIAL] Unknown command. Use: csv");
    //     return;
    // }
    if (liveState || measureamentInProgress) {
        Serial.println("[CSV] Busy: stop LIVE and wait for the current measurement.");
        return;
    }

    if (strcmp(text, "csv") == 0) {
    
        Serial.println("[CSV BEGIN /data.csv]");
        const csv::Result result = csv::exportTo(Serial);
        if (result == csv::Result::Ok) {
            Serial.println("[CSV END]");
        } else {
            Serial.printf("\n[CSV] Export failed: %s\n", csv::resultMessage(result));
        }
    } else if (strcmp(text, "temp") == 0) {
        float temperature = getTemperatureAverage();
        Serial.printf("[SENSOR] Average Temperature: %.2f ºC\n", temperature);
    } else {
        Serial.println("[SERIAL] Unknown command. List of commands:");
        Serial.println("[SERIAL] 1. csv"); 
        Serial.println("[SERIAL] 2. temp");
    }
}
} // namespace


/**
 * @brief Esta función se ejecuta en la interrupcion del puerto serie.
 *        Permite leer comandos desde el puerto serie y ejecutarlos.
 *        Está limitada a 32 bytes para no bloquear el puerto serie
 *        con comandos largos.  
 *        Filtra los caracteres no imprimibles y el backspace.
 * @param void
 * @return void
 */
void processSerialCommands() {
    // Bounded, non-blocking input; accept LF, CR or CRLF without duplicate execution.
    for (uint8_t budget = 0; budget < 32 && Serial.available() > 0; ++budget) {
        const int value = Serial.read();
        if (value < 0) return;
        if (value == '\r' || value == '\n') {
            if (invalidCommand) {
                Serial.println("[SERIAL] Command too long or invalid. Use: csv");
            } else if (commandLength) {
                executeCommand();
            }
            commandLength = 0;
            invalidCommand = false;
        } else if (value == '\b' || value == 127) {
            if (!invalidCommand && commandLength) --commandLength;
        } else if (!invalidCommand) {
            if ((value < 32 && value != '\t') || value > 126 ||
                commandLength + 1 >= sizeof(command)) {
                invalidCommand = true;
            } else {
                command[commandLength++] = static_cast<char>(value);
            }
        }
    }
}
