# Manual del módulo CSV

Este módulo guarda espectros de 18 bandas calibradas en la flash del ESP32,
utilizando LittleFS. La API está declarada en [csv.h](csv.h) y su implementación
está en [csv.cpp](csv.cpp).

Los ejemplos de este manual son para la futura integración. Actualmente el
módulo no está conectado al botón LOG, al teclado ni a `Runtime()`.

## 1. Estado actual y comportamiento previsto

| Aspecto | Implementado actualmente | Propuesta pendiente de implementar |
| --- | --- | --- |
| Capacidad | 100 medidas más la cabecera | Mantener las últimas 100 medidas |
| Orden | Cada medida se añade al final | Medida más reciente justo debajo de la cabecera |
| Medida número 101 | Devuelve `csv::Result::Full` y conserva el archivo | Guarda la nueva y descarta la más antigua |
| Operación de guardado | `csv::append()` | Adaptar la operación y valorar el nombre `save()` |

**El guardado con rotación todavía no existe.** Los ejemplos siguientes usan
la API disponible y no eliminan automáticamente medidas anteriores.

## 2. Ubicación del archivo

| Ruta | Función |
| --- | --- |
| `data/data.csv`, en el proyecto del ordenador | Archivo inicial opcional que se incluye al crear una imagen del sistema de archivos |
| `/data.csv`, en LittleFS del ESP32 | Archivo donde el firmware guarda las medidas |
| `/data.csv.tmp`, en LittleFS | Archivo temporal utilizado al crear o restablecer la cabecera |

Guardar en el ESP32 no actualiza el archivo del ordenador. Para recuperar las
medidas hay que exportar el contenido, por ejemplo mediante `csv::exportTo()`.

`/data.csv` está en la raíz de LittleFS; el nombre de la carpeta `data/` del
proyecto no crea una carpeta `/data/` dentro de la flash.

## 3. Preparar LittleFS

La configuración del proyecto ya incluye:

```ini
board_build.filesystem = littlefs
```

`csv::begin()` monta LittleFS con `LittleFS.begin(false)`: **no formatea si el
montaje falla**. Si el dispositivo aún no tiene un sistema LittleFS válido,
primero hay que prepararlo. Una opción es crear y subir la imagen de `data/`
desde la raíz del proyecto:

```powershell
& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32dev -t buildfs
& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32dev -t uploadfs
```

`buildfs` crea la imagen local. `uploadfs` la escribe en el dispositivo y
**sustituye el contenido anterior de esa partición**, incluido el registro CSV.
Es un paso de preparación, no una operación que deba repetirse para guardar
cada medida. Exportar primero cualquier registro que se quiera conservar.

El archivo vacío `data/data.csv` del proyecto es válido como punto de partida:
al inicializarse, el módulo escribe su cabecera. También crea el CSV si no existe.
Compilar el firmware no equivale a subir la imagen LittleFS.

## 4. Formato de los registros

Cada medida ocupa una línea. El archivo tiene 45 columnas: los datos de
configuración, el nombre introducido en LOG, la temperatura y los 18 valores
raw y calibrados:

```csv
Sample_id,Label,White LED,UV LED,IR LED,Gain,Integration time,Measurement time,Temperature,raw410,raw435,raw460,raw485,raw510,raw535,raw560,raw585,raw610,raw645,raw680,raw705,raw730,raw760,raw810,raw860,raw900,raw940,cal410,cal435,cal460,cal485,cal510,cal535,cal560,cal585,cal610,cal645,cal680,cal705,cal730,cal760,cal810,cal860,cal900,cal940
```

| Campo | Significado |
| --- | --- |
| `Sample_id` | Identificador suministrado por la adquisición; el módulo no lo genera ni exige que sea único |
| `Label` | Nombre introducido en LOG, entre comillas dobles |
| `White LED`, `UV LED`, `IR LED` | Configuración de intensidad de los LEDs |
| `Gain` | Ganancia usada por el sensor |
| `Integration time` | Tiempo de integración en milisegundos |
| `Measurement time` | Período de medida en segundos |
| `Temperature` | Temperatura media del sensor en grados Celsius |
| `raw410` … `raw940` | Las 18 lecturas raw (`uint16_t`) en orden de longitud de onda |
| `cal410` … `cal940` | Los 18 valores calibrados en el mismo orden |

Reglas del formato:

- Separador de columnas: coma. Separador decimal: punto.
- Integración, temperatura y valores calibrados se escriben con hasta 9 cifras
  significativas (`%.9g`); pueden usar notación científica. Se rechazan
  `NaN` e infinito.
- Las cadenas de configuración no pueden contener comas, comillas ni
  caracteres de control y deben caber en sus campos de `Record`.
- Nombre de hasta 64 bytes; con texto UTF-8 no necesariamente son 64 caracteres.
  Se permite un nombre vacío, pero no un puntero nulo.
- Las comas y comillas en el nombre se escapan automáticamente. Por ejemplo,
  `Muestra, "A"` se escribe como `"Muestra, ""A"""`.
- No se aceptan saltos de línea, tabuladores ni otros caracteres de control en
  el nombre; cada medida ocupa una única línea física.
- El escritor termina las líneas con `\n`. El lector acepta también `\r\n`.
  Una última línea sin terminador se considera incompleta.

La cabecera no cuenta como medida: con 100 registros hay 101 líneas.

`sampleId` y `measureTime` son `uint32_t`. `measureTime` expresa el período en
segundos; no representa fecha y hora. El contador de filas persistidas no
sustituye a un identificador de adquisición.

## 5. API disponible

Todas las funciones están en el espacio de nombres `csv`:

| Función | Uso |
| --- | --- |
| `begin()` | Montar LittleFS, preparar o validar el CSV y recuperar el contador |
| `append(record, label)` | Añadir una medida al final; devuelve `Full` si ya hay 100 |
| `getRecordCount(count)` | Consultar el contador; usar `count` solo si devuelve `Ok` |
| `exportTo(output)` | Enviar el archivo completo, incluida la cabecera, a un objeto `Print` |
| `clear()` | Borrar las medidas y dejar únicamente la cabecera |
| `resultMessage(result)` | Obtener un texto de diagnóstico para un resultado |

Las operaciones son síncronas. Deben ejecutarse desde la tarea principal,
no desde una interrupción ni concurrentemente desde varias tareas. `append()`
abre, escribe y cierra el archivo en cada guardado. `exportTo()` puede tardar
según el tamaño del CSV y la velocidad de su destino.

El módulo administra `/data.csv`; no modificar ese archivo por otra vía
mientras se utiliza el contador en memoria. Si se reemplaza externamente,
volver a ejecutar `begin()` antes de guardar.

## 6. Inicialización

Incluir el módulo en el archivo que lo utilizará:

```cpp
#include "csv/csv.h"
```

Ejemplo para añadir en `setup()`, después de iniciar el puerto serie:

```cpp
const csv::Result result = csv::begin();
if (result != csv::Result::Ok) {
    Serial.printf("[CSV] %s\n", csv::resultMessage(result));
} else {
    uint16_t count = 0;
    if (csv::getRecordCount(count) == csv::Result::Ok) {
        Serial.printf("[CSV] %u/%u registros\n",
                      static_cast<unsigned>(count),
                      static_cast<unsigned>(csv::MAX_RECORDS));
    }
}
```

En cada arranque se valida la cabecera y se recorren las filas para recuperar
el contador. No se borra un registro existente válido. Si se encuentra una
cabecera incompatible, una fila inválida o más de 100 registros, la inicialización
devuelve un error y conserva el archivo.

## 7. Preparar y guardar una medida

La estructura que recibe el módulo es:

```cpp
csv::Record record;
// record.sampleId
// record.timestampMs
// record.calibrated[18]
```

Esta función de ejemplo recibe una copia completa de los datos de adquisición:

```cpp
csv::Result guardarMedida(uint32_t sampleId,
                         uint32_t acquiredAtMs,
                         const float (&values)[csv::CHANNEL_COUNT],
                         const char* label) {
    csv::Record record;
    record.sampleId = sampleId;
    record.timestampMs = acquiredAtMs;
    for (uint8_t i = 0; i < csv::CHANNEL_COUNT; ++i) {
        record.calibrated[i] = values[i];
    }
    return csv::append(record, label);
}
```

El llamador debe comprobar el resultado y mostrar éxito únicamente cuando sea
`csv::Result::Ok`. Si devuelve `Full`, el archivo permanece igual: la rotación
está pendiente de implementar.

El módulo no dispara medidas, no lee el sensor y no obtiene automáticamente
`millis()`. Recibe los datos y su tiempo para que el registro corresponda a la
adquisición elegida, no al instante posterior en que se termina de escribir el nombre.

### Integración propuesta con LOG, todavía pendiente

1. `Runtime()` termina de leer una muestra y la publica como disponible.
2. Al pulsar LOG, se copia la última muestra completa a un registro pendiente.
3. El teclado permite introducir su nombre.
4. Al confirmar, se guarda la copia pendiente y se comprueba el resultado.
5. Al cancelar, se descarta la copia pendiente sin escribir.

Si LIVE sigue activo durante la edición del nombre, la copia evita que el registro
cambie con las nuevas adquisiciones. LOG debería estar deshabilitado hasta tener
una muestra completa. Este flujo es una propuesta de integración; no se ha añadido
a los eventos de la interfaz.

## 8. Exportar el archivo

Ejemplo para enviarlo por serie:

```cpp
const csv::Result result = csv::exportTo(Serial);
if (result != csv::Result::Ok) {
    Serial.printf("[CSV] Export error: %s\n", csv::resultMessage(result));
}
```

El receptor del ordenador debe capturar y guardar el contenido. El módulo no crea
un archivo en el PC. Durante la exportación conviene suspender los mensajes de
diagnóstico de adquisición para que no se mezclen con las filas del CSV.

Si LittleFS está montado, se permite exportar incluso un CSV inválido, para poder
recuperar sus datos antes de decidir si se borra.

## 9. Borrar el registro explícitamente

Ejemplo para una futura acción de borrado elegida por el usuario:

```cpp
const csv::Result result = csv::clear();
Serial.printf("[CSV] %s\n", csv::resultMessage(result));
```

`clear()` elimina todas las medidas y deja la cabecera. No formatea LittleFS ni
borra otros archivos. Puede utilizarse después de que `begin()` haya montado el
sistema pero detectado un CSV inválido. No llamarla automáticamente al arrancar
ni ante cualquier error de inicialización.

La cabecera se prepara en `/data.csv.tmp` antes de reemplazar `/data.csv`.

## 10. Resultados y tratamiento de errores

| Resultado | Significado y actuación |
| --- | --- |
| `Ok` | Operación completada |
| `NotInitialized` | Falta inicializar o el módulo ha bloqueado nuevas escrituras; revisar el resultado de `begin()` |
| `MountFailed` | LittleFS no se ha podido montar; revisar la preparación de la partición |
| `OpenFailed` | No se pudo abrir el archivo solicitado |
| `ReadFailed` | Falló la lectura del contenido |
| `WriteFailed` | Escritura incompleta; no considerar guardada la medida |
| `RenameFailed` | No se pudo sustituir el CSV por el temporal |
| `InvalidData` | Archivo incompatible, mal formado o incompleto; exportar antes de decidir si se borra |
| `InvalidRecord` | Nombre, configuración u otro campo del registro no admitido |
| `Full` | Ya hay 100 medidas; el guardado actual no elimina ninguna |
| `OutputFailed` | El destino no pudo recibir la exportación |

Después de una escritura parcial se bloquean los siguientes `append()` para
no añadir datos detrás de una fila incompleta. `begin()` vuelve a examinar el
archivo; si la fila sigue incompleta, devuelve `InvalidData`. No hay reparación
automática ni eliminación silenciosa de esa fila.

La validación comprueba el formato, los límites de las cadenas y que los campos
float sean finitos. No demuestra que la adquisición I²C haya sido correcta. La
robustez ante cortes de alimentación
debe comprobarse en hardware; escribir y cerrar un archivo no convierte toda la
secuencia de guardado en una transacción de aplicación.

## 11. Rotación propuesta: últimos 100, más recientes arriba

El diseño comentado para la siguiente versión es:

1. Escribir la cabecera en `/data.csv.tmp`.
2. Escribir la nueva medida.
3. Copiar como máximo las primeras 99 medidas del archivo anterior.
4. Cerrar y validar el temporal.
5. Sustituir `/data.csv` cuando el nuevo archivo esté completo.

El archivo anterior deberá estar ya ordenado de más reciente a más antiguo.
**La versión actual escribe en el orden contrario.** Antes de activar la rotación
habrá que migrar el archivo existente o exportarlo y empezar uno nuevo; no se
pueden copiar simplemente sus primeras 99 filas y asumir que son las más recientes.

Escribir después de la cabecera sobrescribe bytes; no inserta espacio ni desplaza
las filas. Por eso esta propuesta reconstruye el archivo en un temporal.
Con la rotación, el contador quedará en 100 y dejará de devolverse `Full` como
consecuencia normal de alcanzar la capacidad.

## 12. Verificación

El módulo se ha compilado con el firmware. Hay pruebas locales que compilan
`csv.cpp` con sustitutos de Arduino y LittleFS en memoria:

- Capacidad de 100 medidas y rechazo de la siguiente.
- Recuperación del contador al reinicializar.
- Nombres con comas y comillas; valores finitos extremos.
- Rechazo de datos inválidos y filas incompletas.
- Fallos de apertura, lectura, escritura parcial, sustitución y exportación.
- Borrado explícito y creación a partir de un archivo vacío.

Las instrucciones están en [test/csv_host/README.md](../../test/csv_host/README.md).
Estas pruebas no sustituyen la validación del montaje, persistencia tras reinicio
y comportamiento ante cortes de alimentación en el ESP32.
