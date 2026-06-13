#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;   // CS pin for SD card
File logFile;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for Serial port to be ready
  }

  if (!SD.begin(chipSelect)) {
    while (1); // stop here if SD card fails
  }
}

void loop() {
  if (Serial.available()) {
    String incoming = Serial.readStringUntil('\n'); // read until newline

    // Open file for appending
    logFile = SD.open("log.txt", FILE_WRITE);
    if (!logFile) {
    } else {
      logFile.println(incoming); // <-- fixed missing semicolon
      logFile.close();           // <-- fixed missing semicolon
    }
  }
}
