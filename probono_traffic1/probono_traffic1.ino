#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>

#define YANG_TRAFFIC_ADDRESS 0x08
#define RYEONG_ADDRESS 0x09
#define JU_TRAFFIC_ADDRESS 0x06

#define SS_PIN 10
#define RST_PIN 9
#define I2C_TIMEOUT 1000  // 1 second timeout

int redPin = 2;
int greenPin = 3;
int greenTime = 5000;
bool extendGreenTime = false;
int receivedData = 0;
bool sentFlag = false;
int realdata = 0;

void setup() {
  Serial.begin(115200);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);

  Wire.begin(YANG_TRAFFIC_ADDRESS);
  Wire.onReceive(receiveData);
}

void receiveData(int byteCount) {
  receivedData = Wire.read();
  if (receivedData == 1) {
    Serial.println("카드 태깅을 신호등이 수신 하였습니다.\n");
    extendGreenTime = true;
    realdata = receivedData;
  }
}

void loop() {
  // 항상 5초 동안 빨간불을 켜줍니다.
  digitalWrite(redPin, HIGH);
  delay(35000);
  digitalWrite(redPin, LOW);
  delay(10);

  // 초록불은 extendGreenTime의 상태에 따라 켜는 시간을 결정합니다.
  digitalWrite(greenPin, HIGH);

  if (extendGreenTime) {
    Serial.println("신호등 녹색 신호를 25초로 증가시킵니다.\n");
    delay(20000);  // 8초 동안 켜집니다. (10초 중 2초를 제외한 시간)

    if (!sentFlag) {                 // 아직 sendToOtherSlave()를 호출하지 않았다면
      sendTOryeong(RYEONG_ADDRESS);  // 2초가 남았을 때 호출
      sentFlag = true;               // 호출 플래그 설정
      receivedData = 0;
    }

    delay(5000);              // 마지막 2초 동안 켜집니다.
    extendGreenTime = false;  // 연장 상태를 초기화합니다.
  }

  else {
    Serial.println("카드 태깅이 되지 않아 신호등 녹색 신호를 20초로 유지시킵니다.\n");
    delay(15000);  // 3초 동안 켜집니다. (5초 중 2초를 제외한 시간)

    if (!sentFlag) {  // 아직 sendToOtherSlave()를 호출하지 않았다면

      sendTOryeong(RYEONG_ADDRESS);  // 2초가 남았을 때 호출
      sentFlag = true;               // 호출 플래그 설정
      receivedData = 0;
    }

    delay(5000);  // 마지막 2초 동안 켜집니다.
  }

  digitalWrite(greenPin, LOW);
  delay(10);
  sentFlag = false;  // 플래그 초기화
}

bool sendTOryeong(byte address) {
  unsigned long startTime = millis();

  Wire.beginTransmission(RYEONG_ADDRESS);
  Wire.write(2);

  while (Wire.endTransmission() != 0) {
    if (millis() - startTime > I2C_TIMEOUT) {
      Serial.println("I2C timeout for address: " + String(address, HEX));
      return false;
    }
  }

  Serial.println("녹색불 신호가 5초 남아 잔여 시간 정보를 초음파 센서에 송신 성공하였습니다.\n");
  return true;
}