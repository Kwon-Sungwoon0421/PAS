#define PUL_pin1 8  
#define DIR_pin1 9  
#define ENA_pin1 10 

#define PUL_pin2 11 
#define DIR_pin2 12 
#define ENA_pin2 13 

#define stepsPerRevolution 200 // 모터 한 회전당 스텝 수
#define motorSpeed 8000 // 모터 속도 (스텝당 마이크로초)
#define stepDelay 2000 // 각 스텝 사이의 대기 시간 (마이크로초 단위)

unsigned long previousMillis1 = 0;  // 첫 번째 모터의 이전 시간
unsigned long previousMillis2 = 0;  // 두 번째 모터의 이전 시간

void setup() {
  pinMode(PUL_pin1, OUTPUT);
  pinMode(DIR_pin1, OUTPUT);  
  pinMode(ENA_pin1, OUTPUT);

  pinMode(PUL_pin2, OUTPUT);
  pinMode(DIR_pin2, OUTPUT);
  pinMode(ENA_pin2, OUTPUT);

  // 모터 드라이버 활성화
  digitalWrite(ENA_pin1, LOW); 
  digitalWrite(ENA_pin2, LOW); 

  // 반시계 방향 설정
  digitalWrite(DIR_pin1, LOW); 
  digitalWrite(DIR_pin2, LOW); 
}

void loop() {
  // 정밀 제어
  unsigned long currentMillis1 = micros();  // 현재 시간 (마이크로초 단위)

  if (currentMillis1 - previousMillis1 >= motorSpeed) {
    // 일정 시간 간격마다 펄스를 보냄
    digitalWrite(PUL_pin1, HIGH);
    previousMillis1 = currentMillis1;  // 시간 업데이트
    delayMicroseconds(1000);  
    digitalWrite(PUL_pin1, LOW);
  }

  unsigned long currentMillis2 = micros();  

  if (currentMillis2 - previousMillis2 >= motorSpeed) {
    digitalWrite(PUL_pin2, HIGH);
    previousMillis2 = currentMillis2;  
    delayMicroseconds(1000); 
    digitalWrite(PUL_pin2, LOW);
  }
}