#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;   // CS pin for SD card
File logFile;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  Serial.println("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card ready.");

  // Open file for appending

  }
}

void loop() {
  if (Serial.available()) {
    String incoming = Serial.readStringUntil('\n'); // read until newline
    Serial.print("Received: ");
    Serial.println(incoming);

    logFile = SD.open("log.txt", FILE_WRITE);
    if (!logFile) {
      Serial.println("Error opening log.txt");
    logFile.println(incoming)
    logFile.close()
  }
}
