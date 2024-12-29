const int irSensorPin = 2; 
const int in1Pin = 5;      // 정방향 제어
const int in2Pin = 6;      // 역방향 제어
const int enaPin = 7;      // 속도 제어

const int maxMoveTime = 5000; // 모터 이동 시간

bool actionInProgress = false; // 현재 동작이 진행 중인지 확인

void setup() {
  pinMode(irSensorPin, INPUT);  
  pinMode(in1Pin, OUTPUT);      
  pinMode(in2Pin, OUTPUT);      
  pinMode(enaPin, OUTPUT);      

  Serial.begin(9600);          

  initializePosition(); // 초기 위치 함수
  Serial.println("초기화 완료");
}

void loop() {
  int sensorState = digitalRead(irSensorPin);

  // 물체 감지 && 동작 X
  if (sensorState == HIGH && !actionInProgress) {
    actionInProgress = true; // 동작 시작

    stickOut(); // 정방향

    stickIn(); // 역방향

    initializePosition(); // 초기화

    actionInProgress = false; // 동작 종료
  }
}

/*
- 역방향
digitalWrite(in1Pin, LOW);     
digitalWrite(in2Pin, HIGH);

- 정방향
digitalWrite(in1Pin, HIGH);     
digitalWrite(in2Pin, LOW);

- 정지
digitalWrite(in1Pin, LOW);     
digitalWrite(in2Pin, LOW);

analogWrite(enaPin, 255); // 최대 속도
*/

// 초기 위치 함수 (0 위치)
void initializePosition() {
  digitalWrite(in1Pin, LOW);     
  digitalWrite(in2Pin, HIGH);
  analogWrite(enaPin, 255);      
  delay(maxMoveTime);            
  stopMotor();                   
}

// 정방향
void stickOut() {
  delay(2000);  
  digitalWrite(in1Pin, HIGH);    
  digitalWrite(in2Pin, LOW);
  analogWrite(enaPin, 255);      
  delay(maxMoveTime);            
  stopMotor();                   
}

// 역방향
void stickIn() {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, HIGH);
  analogWrite(enaPin, 255);
  delay(maxMoveTime);
  stopMotor();
}

void stopMotor() {
  digitalWrite(in1Pin, LOW);     
  digitalWrite(in2Pin, LOW);
  analogWrite(enaPin, 0);
}
