#include <SPI.h>
#include <RH_RF95.h>
#include <PWMServo.h>

#define RFM95_CS  4
#define RFM95_RST 2
#define RFM95_INT 3
#define RF95_FREQ 434.0

RH_RF95 rf95(RFM95_CS, RFM95_INT);

PWMServo myservoX;
PWMServo myservoY;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  myservoX.attach(9);
  myservoY.attach(10);

  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

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

  Serial.println("LoRa RX ready");
}

void loop() {
  if (rf95.available()) {
    uint8_t buf[2];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len)) {
      int posx = buf[0];
      int posy = buf[1];

      myservoX.write(posx);
      myservoY.write(posy);
      delay(10);
      Serial.print("Received X: ");
      Serial.print(posx);
      Serial.print(" Y: ");
      Serial.println(posy);
    }
  }
}
