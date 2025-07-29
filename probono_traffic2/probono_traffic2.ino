#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>

#define YANG_TRAFFIC_ADDRESS 0x08
#define JU_TRAFFIC_ADDRESS 0x06
#define NIM_ADDRESS 0x07

#define SS_PIN 10
#define RST_PIN 9
#define I2C_TIMEOUT 40000  // 1 second timeout

int redPin = 2;
int greenPin = 3;
//int greenTime = 5000;
//bool extendGreenTime = false;
int receivedData = 0;
//bool sentFlag = false;
int realdata=0;

void setup() {
  Serial.begin(115200);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);

  Wire.begin(JU_TRAFFIC_ADDRESS);
  Wire.onReceive(receiveData);
}

void receiveData(int byteCount) {
  receivedData = Wire.read();
  
  if(receivedData==10){
    Serial.println("not increase");
    realdata=receivedData;

  }

  if(receivedData==11){
    Serial.println("increase");
    //sendToyangtraffic(YANG_TRAFFIC_ADDRESS);
    realdata=receivedData;
  }
/*
  if(receivedData==50){
    Serial.println("receive to yang");
  }
  */
}

void loop() {
  // 항상 5초 동안 빨간불을 켜줍니다.
  digitalWrite(redPin, HIGH);
  delay(35000);
  digitalWrite(redPin, LOW);
  delay(10);

  // 초록불은 extendGreenTime의 상태에 따라 켜는 시간을 결정합니다.
  digitalWrite(greenPin, HIGH);
  //delay(20000);
  if (realdata==11) {//증가
    sendTOnim_increase(NIM_ADDRESS);
    delay(25000);
    realdata=0;
  } 
  
  else if(realdata==10){ //증가하지 않음
    sendTOnim_no_increase(NIM_ADDRESS);
    delay(20000);
    realdata=0;
  }
  
  else{
  delay(20000);   
  }
  
  digitalWrite(greenPin, LOW);
  delay(10);
  //sentFlag = false;  // 플래그 초기화

}

void sendTOnim_increase(byte address) {
  Serial.println("in increase nim");
  unsigned long startTime = millis();

  Wire.beginTransmission(NIM_ADDRESS);
  Wire.write(100);
  Wire.endTransmission();
  
  Serial.println("send to nim");
}

bool sendTOnim_no_increase(byte address) {
  Serial.println("in not increase nim");
  unsigned long startTime = millis();

  Wire.beginTransmission(NIM_ADDRESS);
  Wire.write(200);
  Wire.endTransmission();

  Serial.println("send to nim");
  return true;
}
/*
bool sendToyangtraffic(byte address) {
  Serial.println("in yangtraffic");
  unsigned long startTime = millis();

  Wire.beginTransmission(YANG_TRAFFIC_ADDRESS);
  Wire.write(60);

  while (Wire.endTransmission() != 0) {
    if (millis() - startTime > I2C_TIMEOUT) {
      Serial.println("I2C timeout for address: " + String(address, HEX));
      return false;
    }
  }
  
  Serial.println("send to yang traffic");
  
  return true;
}
*/