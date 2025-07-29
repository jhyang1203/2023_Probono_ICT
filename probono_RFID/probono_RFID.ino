#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>

#define YANG_TRAFFIC_ADDRESS 0x08
#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  Wire.begin();  // 마스터로 시작
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.println("교통약자 카드 태깅이 감지되었습니다.");
    Wire.beginTransmission(YANG_TRAFFIC_ADDRESS);
    Wire.write(1);  // 데이터 전송
    Wire.endTransmission();
    Serial.println("\n");

    if (Wire.endTransmission() == 0) {
      Serial.println("신호등과 통신 성공하였습니다.");
    } else {
      Serial.println("error");
    }
    mfrc522.PICC_HaltA();
  }
}