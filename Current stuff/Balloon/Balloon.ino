#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS 4
#define RFM95_RST 2
#define RFM95_INT 3
#define RF95_FREQ 434.0
#define LED 13

RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(9600);
  while (!Serial);

  Serial.println("Arduino LoRa RX Test!");

  // Reset LoRa module
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  if (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

  rf95.setTxPower(23, false); // Max power using PA_BOOST
}

void loop() {
  // Check if a message is available
  if (!rf95.available()) return;

  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (!rf95.recv(buf, &len)) {
    Serial.println("Receive failed");
    return;
  }

  digitalWrite(LED, HIGH);

  // Print raw message
  RH_RF95::printBuffer("Received: ", buf, len);
  Serial.print("Got: ");
  Serial.println((char*)buf);
  Serial.print("RSSI: ");
  Serial.println(rf95.lastRssi(), DEC);

// Send a reply
String rcv = (char*)buf;             // Convert received buffer to String
String message = "KC1VVU got " + rcv; // Concatenate properly
uint8_t data[message.length() + 1];  // +1 for null terminator
message.getBytes(data, sizeof(data)); // Copy String into uint8_t array

rf95.send(data, sizeof(data));
rf95.waitPacketSent();
Serial.println("Sent a reply");
digitalWrite(LED, LOW);

}
