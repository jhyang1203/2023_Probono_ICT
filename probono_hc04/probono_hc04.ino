#include <Wire.h>
#include <U8glib.h>

#define RYEONG_ADDRESS 0x09

#define TRIG_PIN 6
#define ECHO_PIN 7
#define BUZER_PIN 10

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0 | U8G_I2C_OPT_NO_ACK | U8G_I2C_OPT_FAST);

int distance;
int receivedData = 0;

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZER_PIN, OUTPUT);
  u8g.setFont(u8g_font_osr18r);

  Wire.begin(RYEONG_ADDRESS);

  Wire.onReceive(receiveData);
}

void receiveData(int byteCount) {
  receivedData = Wire.read();
  if (receivedData == 2) {
    Serial.println("초록불 5초 남았습니다.");
  }
}

void measureDistance() {
  digitalWrite(ECHO_PIN, LOW);
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("측정 결과 거리는 ");
  Serial.print(distance);
  Serial.println(" 입니다.");
}

void showOLED(String message) {
  u8g.firstPage();
  do {
    u8g.setPrintPos(0, 20);  // 위치 조정
    u8g.print(message);
  } while (u8g.nextPage());
}

void loop() {

  if (receivedData == 2) {
    measureDistance();

    if (distance >= 30) {
      Serial.println("30cm이상의 위치에 사람이 존재하므로 버저를 울립니다.");
      digitalWrite(BUZER_PIN, HIGH);
      delay(1000);  //1second
      digitalWrite(BUZER_PIN, LOW);
    }

    receivedData = 0;
  }

  else {
    digitalWrite(BUZER_PIN, LOW);
  }
}