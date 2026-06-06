#include <SPI.h>
#include <RH_RF95.h>
#include <string.h>

#define RFM95_CS  4
#define RFM95_RST 2
#define RFM95_INT 3
#define RF95_FREQ 434.0

#define HOLD_SWITCH_PIN 6
#define HOLD_LED_PIN    7
#define TX_SWITCH_PIN   8
#define TX_LED_PIN      9

RH_RF95 rf95(RFM95_CS, RFM95_INT);

const char CALLSIGN[] = "KC1VVU";

struct Packet {
  uint8_t x;
  uint8_t y;
  char callsign[7];   // 6 chars + null terminator
};

int posx = 0;
int posy = 0;
int lastPosX = 0;
int lastPosY = 0;

bool lastTxState = false;
bool lastHoldState = false;

void setup() {
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  pinMode(HOLD_SWITCH_PIN, INPUT_PULLUP);
  pinMode(HOLD_LED_PIN, OUTPUT);
  pinMode(TX_SWITCH_PIN, INPUT_PULLUP);
  pinMode(TX_LED_PIN, OUTPUT);

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
  bool hold = digitalRead(HOLD_SWITCH_PIN) == LOW;
  bool txEnable = digitalRead(TX_SWITCH_PIN) == LOW;

  // Update LEDs
  digitalWrite(HOLD_LED_PIN, hold ? HIGH : LOW);
  digitalWrite(TX_LED_PIN, txEnable ? HIGH : LOW);

  // Print TX status only if changed
  if (txEnable != lastTxState) {
    Serial.println(txEnable ? "TXON" : "TXOFF");
    lastTxState = txEnable;
  }

  // Print Hold status only if changed
  if (hold != lastHoldState) {
    Serial.println(hold ? "HOLDON" : "HOLDOFF");
    lastHoldState = hold;
  }

  // Read analog only if not holding
  if (!hold) {
    posx = map(analogRead(A5), 0, 1023, 0, 180);
    posy = map(analogRead(A4), 0, 1023, 0, 180);
    lastPosX = posx;
    lastPosY = posy;
  } else {
    posx = lastPosX;
    posy = lastPosY;
  }

  // Transmit only if TX switch is pressed
  if (txEnable) {
    Packet packet;

    packet.x = (uint8_t)posx;
    packet.y = (uint8_t)posy;
    strcpy(packet.callsign, CALLSIGN);

    rf95.send((uint8_t *)&packet, sizeof(packet));
    rf95.waitPacketSent();

    Serial.print("Sent X: ");
    Serial.print(packet.x);
    Serial.print(" Y: ");
    Serial.print(packet.y);
    Serial.print(" Callsign: ");
    Serial.println(packet.callsign);
  }

  delay(50); // debounce switches
}