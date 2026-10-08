# Manual del módulo CSV

Este módulo guarda espectros de 18 bandas calibradas en la flash del ESP32,
utilizando LittleFS. La API está declarada en [csv.h](csv.h) y su implementación
está en [csv.cpp](csv.cpp).

La adquisición publica las medidas y la pantalla LOG permite guardar una copia
con una etiqueta. `csv::append()` conserva como máximo las últimas 100 medidas:
al guardar la número 101, elimina la más antigua y añade la nueva al final.

## 1. Comportamiento actual

| Aspecto | Comportamiento |
| --- | --- |
| Capacidad | 100 medidas más la cabecera |
| Orden | De la más antigua a la más reciente |
| Al llegar a 100 medidas | Elimina la más antigua y guarda la nueva |
| Escritura cuando rota | Reconstruye y valida `/data.csv.tmp` antes de sustituir el CSV |

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
- La integración y temperatura se escriben con dos decimales; los calibrados,
  con cuatro. Se rechazan `NaN` e infinito.
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
| `append(record, label)` | Añadir una medida; si hay 100, descartar la más antigua |
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
// Populate sampleId, whiteLed, uvLed, irLed, gain, integrationTimeMs,
// measureTime, temperature, raw[18] and calibrated[18] from the acquisition.
```

La etiqueta introducida en LOG se pasa por separado a `append()`:

```cpp
csv::Result guardarMedida(const csv::Record& record, const char* label) {
    return csv::append(record, label);
}
```

El llamador debe comprobar el resultado y mostrar éxito únicamente cuando sea
`csv::Result::Ok`. Al guardar con 100 registros, `append()` reconstruye el CSV
con los 99 más recientes y el nuevo registro.

El módulo no dispara medidas ni lee el sensor. Recibe los datos de la adquisición
y su período junto con la etiqueta elegida por el usuario.

### Guardado desde LOG

Al pulsar LOG se copia la última muestra completa a un registro pendiente. El
teclado permite introducir su etiqueta; al confirmar se guarda esa copia y al
cancelar se descarta sin escribir. La copia evita que una adquisición nueva
cambie los datos mientras se introduce la etiqueta.

LOG solo permite continuar cuando hay una muestra completa disponible.

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
| `Full` | Reservado; alcanzar el límite no impide guardar porque se elimina la fila más antigua |
| `OutputFailed` | El destino no pudo recibir la exportación |

Si falla una escritura directa antes de alcanzar la capacidad, se bloquean los
siguientes `append()` para no añadir datos detrás de una fila incompleta.
`begin()` vuelve a examinar el archivo; si la fila sigue incompleta, devuelve
`InvalidData`. Si falla la escritura del temporal durante una rotación, el CSV
original se conserva y sigue disponible. No hay reparación automática ni
eliminación silenciosa de filas incompletas.

La validación comprueba el formato, los límites de las cadenas y que los campos
float sean finitos. No demuestra que la adquisición I²C haya sido correcta. La
robustez ante cortes de alimentación debe comprobarse en hardware; escribir y
cerrar un archivo no convierte toda la secuencia de guardado en una transacción
de aplicación.

## 11. Rotación al alcanzar la capacidad

Cuando ya hay 100 medidas, `append()` escribe en `/data.csv.tmp` la cabecera,
omite la primera (más antigua) fila, copia las otras 99 y añade la nueva fila.
Valida el temporal completo y solo entonces lo renombra como `/data.csv`.
El orden del archivo se mantiene de la medida más antigua a la más reciente.

Si falla la lectura, escritura, validación o sustitución, se devuelve el error
y el CSV anterior no se sustituye. La rotación requiere espacio libre en LittleFS
para crear temporalmente una segunda copia del archivo. Tras un reinicio, el
contador se recupera mediante la validación de `begin()`.

## 12. Verificación

Compilar el firmware con PlatformIO desde la raíz del proyecto:

```powershell
& "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32dev
```

La compilación no sustituye la validación en el ESP32. En hardware, comprobar
la escritura de más de 100 medidas, que el archivo mantenga 100 registros y que
el registro más antiguo desaparezca mientras el nuevo queda al final. Verificar
también la persistencia después de reiniciar y el comportamiento si LittleFS no
tiene espacio suficiente para construir el archivo temporal.
