#include <Servo.h>
#include <LiquidCrystal_I2C.h> 
#include <SoftwareSerial.h>    
#include "HX711.h"             

SoftwareSerial mySerial(10, 11); // 바코드 스캐너 rx, tx

Servo Seoul; 
Servo Daejeon; 
Servo Incheon; 
Servo Busan; 

// 로드셀 설정 (DT: A0, SCK: A1)
HX711 scale;
const int loadCellDT = A0;
const int loadCellSCK = A1;

// 로드셀 보정값
const float CALIBRATION_FACTOR = 2280.0f; // 단위 g

LiquidCrystal_I2C lcd(0x27, 16, 2); // (I2C 주소: 0x27, 16x2 크기)

String lcdTopLine = "Barcode: None";
String lcdBottomLine = "Weight: 0.00 g";

bool isServoActive = false;

void setup() {
  Serial.begin(9600);   
  mySerial.begin(9600); // 바코드 스캐너용

  Seoul.attach(7); 
  Daejeon.attach(6); 
  Incheon.attach(5); 
  Busan.attach(4); 
  initializeServos();

  scale.begin(loadCellDT, loadCellSCK);
  scale.set_scale(CALIBRATION_FACTOR);
  scale.tare(); // 영점 조절

  lcd.init();
  lcd.backlight();
  lcd.print("System Ready");
  delay(1000);
  lcd.clear();
}

void loop() {
  // 서보 동작 중일 때는 새로운 데이터 갱신 방지
  if (isServoActive) {
    delay(200); // 서보 동작 완료까지 대기
    return;
  }

  String barcode = readBarcode();

  float weight = getAverageWeight(); // 실제 측정된 무게 값

  // LCD 화면 갱신
  if (barcode.length() == 0) { // 바코드가 인식되지 않았을 때
    lcdTopLine = "Barcode: None";
    lcdBottomLine = "Weight: 0.0 g";
  } 
  else if (barcode == "Seoul") {
    lcdTopLine = "Barcode: Seoul";
    lcdBottomLine = "Weight: " + String(weight, 1) + " g"; 
    activateServo(Seoul, 30, "Seoul"); 
  } 
  else if (barcode == "Daejeon") {
    lcdTopLine = "Barcode: Daejeon";
    lcdBottomLine = "Weight: " + String(weight, 1) + " g"; 
    activateServo(Daejeon, 30, "Daejeon"); 
  } 
  else if (barcode == "Incheon") {
    lcdTopLine = "Barcode: Incheon";
    lcdBottomLine = "Weight: " + String(weight, 1) + " g"; 
    activateServo(Incheon, 30, "Incheon"); 
  } 
  else if (barcode == "Busan") {
    lcdTopLine = "Barcode: Busan";
    lcdBottomLine = "Weight: " + String(weight, 1) + " g"; 
    activateServo(Busan, 30, "Busan"); 
  } 
  else { // 지정되지 않은 바코드
    lcdTopLine = "Barcode: Unknown";
    lcdBottomLine = "Weight: " + String(weight, 1) + " g"; 
  }

  // LCD 화면 출력
  updateLCD();
}

// 서보 모터 초기 위치
void initializeServos() {
  Seoul.write(90); 
  Daejeon.write(90); 
  Incheon.write(90); 
  Busan.write(90); 
}

// 바코드 데이터 읽기
String readBarcode() {
  String barcode = "";
  while (mySerial.available()) {
    char c = mySerial.read();
    if (c != '\n' && c != '\r') {
      barcode += c;
    }
  }
  return barcode;
}

// 평균 무게 측정
float getAverageWeight() {
  float sum = 0.0;
  const int readings = 10;
  for (int i = 0; i < readings; i++) {
    sum += scale.get_units();
    delay(50);
  }
  return sum / readings;
}

// 서보 모터 활성화
void activateServo(Servo &servo, int angle, const String &location) {
  isServoActive = true;   // 활성화 상태 설정
  updateLCD();          
  servo.write(angle);     

  if (location == "Seoul") {
    delay(8000);  
  } else if (location == "Daejeon") {
    delay(9000);  
  } else if (location == "Incheon") {
    delay(10000); 
  } else if (location == "Busan") {
    delay(11000);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Processing...");

  servo.write(90);        // 서보 모터가 일정 시간 유지 후 기존 위치로 복귀
  isServoActive = false;  
} 

// LCD 화면 업데이트
void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(lcdTopLine);
  lcd.setCursor(0, 1);
  lcd.print(lcdBottomLine);
}
