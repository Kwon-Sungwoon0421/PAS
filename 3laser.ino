#include <Wire.h>
#include <VL53L0X.h>
#include <ESP8266WiFi.h>  
#include <ESP8266HTTPClient.h>  

const char* ssid = "iptimea704";  
const char* password = "";  

VL53L0X sensor1, sensor2, sensor3, sensor4, sensor5;

const int XSHUT_PIN_1 = D2;
const int XSHUT_PIN_2 = D3;
const int XSHUT_PIN_3 = D4;
const int XSHUT_PIN_4 = D5;  
const int XSHUT_PIN_5 = D6;  

uint16_t distance1, distance2, distance3, distance4, distance5; // 거리 측정 변수
unsigned long previousMillis = 0;
unsigned long previousMillisAvg = 0;
const long interval = 1000;  // 데이터 읽는 1초 간격
const long avgInterval = 5000;  // 평균값 만드는 5초 간격

uint16_t distance1Sum = 0, distance2Sum = 0, distance3Sum = 0, distance4Sum = 0, distance5Sum = 0;
uint8_t count = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(D1, D2); // RX TX

  pinMode(XSHUT_PIN_1, OUTPUT);
  pinMode(XSHUT_PIN_2, OUTPUT);
  pinMode(XSHUT_PIN_3, OUTPUT);
  pinMode(XSHUT_PIN_4, OUTPUT);  
  pinMode(XSHUT_PIN_5, OUTPUT);  

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // 센서 초기화 (주소 설정)
  initializeSensor(sensor1, XSHUT_PIN_1, 0x30);
  initializeSensor(sensor2, XSHUT_PIN_2, 0x31);
  initializeSensor(sensor3, XSHUT_PIN_3, 0x32);
  initializeSensor(sensor4, XSHUT_PIN_4, 0x33);  
  initializeSensor(sensor5, XSHUT_PIN_5, 0x34);  
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // 거리 측정
    measureDistance(sensor1, distance1, distance1Sum);
    measureDistance(sensor2, distance2, distance2Sum);
    measureDistance(sensor3, distance3, distance3Sum);
    measureDistance(sensor4, distance4, distance4Sum);  
    measureDistance(sensor5, distance5, distance5Sum);  
    count++;
  }

  if (currentMillis - previousMillisAvg >= avgInterval) {
    previousMillisAvg = currentMillis;

    // 평균값 계산
    float distance1Avg = calculateAverage(distance1Sum, count);
    float distance2Avg = calculateAverage(distance2Sum, count);
    float distance3Avg = calculateAverage(distance3Sum, count);
    float distance4Avg = calculateAverage(distance4Sum, count);  
    float distance5Avg = calculateAverage(distance5Sum, count); 

    sendToServer(distance1Avg, "http://www.all-tafp.org/sector_1.php");
    sendToServer(distance2Avg, "http://www.all-tafp.org/sector_2.php");
    sendToServer(distance3Avg, "http://www.all-tafp.org/sector_3.php");
    sendToServer(distance4Avg, "http://www.all-tafp.org/sector_4.php");  
    sendToServer(distance5Avg, "http://www.all-tafp.org/sector_5.php"); 

    // 초기화
    distance1Sum = distance2Sum = distance3Sum = distance4Sum = distance5Sum = 0;
    count = 0;
  }
}

// 센서 초기화 함수
void initializeSensor(VL53L0X &sensor, int xshutPin, int address) {
  digitalWrite(xshutPin, LOW);
  delay(10);
  digitalWrite(xshutPin, HIGH);
  delay(10);
  if (!sensor.init()) {
    Serial.println("센서 초기화 실패!");
    while (1);
  }
  sensor.setAddress(address);
}

// 거리 측정 함수
void measureDistance(VL53L0X &sensor, uint16_t &distance, uint16_t &distanceSum) {
  distance = sensor.readRangeSingleMillimeters();
  if (!sensor.timeoutOccurred()) {
    distanceSum += distance;
  }
}

// 평균값 계산 함수
float calculateAverage(uint16_t distanceSum, uint8_t count) {
  return (distanceSum / (float)count) / 10.0;  // cm
}

// 서버 전송 함수
void sendToServer(float avg, const char* url) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client; 
    HTTPClient http;   

    String fullURL = String(url) + "?AVG_value=" + String(avg);
    http.begin(client, fullURL); 

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("서버 응답: ");
      Serial.println(httpResponseCode);
      Serial.println(http.getString());
    } else {
      Serial.print("서버 요청 실패, 에러 코드: ");
      Serial.println(httpResponseCode);
    }

    http.end(); 
  } else {
    Serial.println("WiFi 연결 실패!");
  }
}

