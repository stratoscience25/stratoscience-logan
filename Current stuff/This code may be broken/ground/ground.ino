#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS  4
#define RFM95_RST 2
#define RFM95_INT 3
#define RF95_FREQ 434.0

RH_RF95 rf95(RFM95_CS, RFM95_INT);

int posx = 0;
int posy = 0;

void setup() {
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(9600);
  while (!Serial);

  // Reset radio
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  if (!rf95.init()) {
    Serial.println("LoRa init failed");
    while (1);
  }

  rf95.setFrequency(RF95_FREQ);
  rf95.setTxPower(23, false);

  Serial.println("LoRa TX ready");
}

void loop() {
  posx = map(analogRead(A5), 0, 1023, 0, 180);
  posy = map(analogRead(A4), 0, 1023, 0, 180);

  uint8_t packet[2];
  packet[0] = posx;
  packet[1] = posy;
  packet[2] = "KC1VVU";

  rf95.send(packet, sizeof(packet));
  rf95.waitPacketSent();

  Serial.print("Sent X: ");
  Serial.print(posx);
  Serial.print(" Y: ");
  Serial.println(posy);

}
