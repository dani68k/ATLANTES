///Lonely Binary ESP32 Memory Information Test
#include <Arduino.h>
#include <esp_heap_caps.h>

void testPSRAM();
void testFlashMemory();
void testSRAM();

void setup() {
  Serial.begin(115200);
  delay(1000); // Wait for serial to initialize
  
  Serial.println("=== ESP32 Memory Information ===");
  Serial.println();
  
  // Test PSRAM
  testPSRAM();
  
  // Test Flash Memory
  testFlashMemory();
  
  // Test SRAM
  testSRAM();
  
  Serial.println("=== Memory Test Complete ===");
}

void loop() {
  // Nothing to do in loop
  delay(1000);
}

void testPSRAM() {
  Serial.println("--- PSRAM Test ---");

    // Get PSRAM size
  size_t psramSize = ESP.getPsramSize();
  if (psramSize > 0 ) {
    Serial.println("✅ PSRAM is available!");
    Serial.printf("PSRAM Size: %d bytes (%.2f MB)\n", psramSize, psramSize / 1024.0 / 1024.0);
    
    // Check if PSRAM is initialized
    if (psramInit()) {
      Serial.println("✅ PSRAM is initialized and working!");
    } else {
      Serial.println("❌ PSRAM initialization failed!");
    }
      } else {
    Serial.println("❌ PSRAM not found!");
    Serial.println("❌ PSRAM mode should be OPI PSRAM in Arduino Settings");
  }

  Serial.println();
}

void testFlashMemory() {
  Serial.println("--- Flash Memory Test ---");
  
  // Get Flash memory size
  size_t flashSize = ESP.getFlashChipSize();
  Serial.printf("Flash Size: %d bytes (%.2f MB)\n", flashSize, flashSize / 1024.0 / 1024.0);
  
  // Get Flash chip speed
  uint32_t flashSpeed = ESP.getFlashChipSpeed();
  Serial.printf("Flash Speed: %d MHz\n", flashSpeed / 1000000);
  
  // Get Flash chip mode
  uint8_t flashMode = ESP.getFlashChipMode();
  Serial.printf("Flash Mode: %d\n", flashMode);
  
  Serial.println("✅ Flash memory information retrieved!");
  Serial.println();
}

void testSRAM() {
  Serial.println("--- SRAM Test ---");
  
  // Get free heap size
  size_t freeHeap = ESP.getFreeHeap();
  Serial.printf("Free Heap: %d bytes (%.2f KB)\n", freeHeap, freeHeap / 1024.0);
  
  // Get minimum free heap size
  size_t minFreeHeap = ESP.getMinFreeHeap();
  Serial.printf("Minimum Free Heap: %d bytes (%.2f KB)\n", minFreeHeap, minFreeHeap / 1024.0);
  
  // Get maximum allocatable heap size
  size_t maxAllocHeap = ESP.getMaxAllocHeap();
  Serial.printf("Maximum Allocatable Heap: %d bytes (%.2f KB)\n", maxAllocHeap, maxAllocHeap / 1024.0);
  
  Serial.println("✅ SRAM information retrieved!");
  Serial.println();
}