#ifndef APP_H
#define APP_H

#include "csv/csv.h"

extern bool liveState;
extern bool measureamentInProgress;
extern bool firstMeasureament;

void Runtime();
void initUIObjectArrays();
// Copy the last fully collected spectrum. False until a sample is available.
bool copyLatestCsvRecord(csv::Record& record);
void processSerialCommands();

#endif
