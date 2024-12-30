# PAS (Parcel Automation System)
개발기간: 2024-11-26~2024-12-10  
개발인원: 3명  
맡은 업무: PHP 제작 및 서버 연결, Anroid Studio 앱 개발, Arduino 개발, 하드웨어 제작

## 프로젝트 배경 및 개요
- 오프라인 시장이 축소되고 온라인 시장이 급격히 성장하면서 택배 수요가 매년 폭발적으로 증가
- 특정 시기에는 물류센터의 업무량이 기하급수적으로 증가해 택배 노동자들의 과도한 작업 강도 우려
- 이를 해결하기 위한 방안으로 속도와 정확도를 동시에 추구하는 자동화된 택배 분류 컨베이어 시스템을 개발
- 노동자들의 부담을 줄이고 소비자들에게는 신속하고 정확한 서비스를 제공  

## 기술 스택
- **프론트엔드**: Android Studio (Java, XML)
- **백엔드**: PHP, MySQL
- **하드웨어**: Arduino, ESP8266, HX711, DE1200, TCRT5000, SMPS, Nema17 StepMotor, VL53L0X, LM2596, L298N, TB6600, SG-90, 1602LCD, Linear Motor
- **통신**: Wi-Fi 모듈 (ESP8266)  

## 주요 기능
1. **택배 정보 확인**
   - 바코드를 통한 택배의 도착지 정보
   - 로드셀센서를 이용하여 택배의 무게 측정
2. **택배의 이동**
   - 택배가 로드셀 센서 위에 올려졌을 때 IR센서가 택배 인식
   - IR센서에 인식된 택배를 리니어모터를 통해 컨베이어 벨트 위로 이동
   - 벨트는 스텝모터 두개를 이용하여 택배의 이동 속도를 약 5cm/s로 설정
3. **택배의 분류**
   - 바코드 스캐너를 통해 스캔된 바코드에 정보에 따라 해당되는 서보모터 작동
   - 각 서보모터는 두 개의 각도를 지정해 초기 위치와 택배의 움직임을 막는 위치로 나뉨
4. **실시간 모니터링**
   - Wi-Fi를 이용해 어플에서 실시간으로 각 섹터의 상태를 확인

## 시스템 구성 요소
   1. **하드웨어 구성**
        
      (1) **컨베이어 시스템**  
         - Linear Motor: 택배를 컨베이어 벨트 위로 이동  
         - Nema17 Step Motor: 5cm/s의 속도로 컨베이어 벨트 구동  
         - SG-90 Servo Motor: 택배 정보에 따라 택배를 분류  

      (2) **택배 인식 시스템**  
         - TCRT5000 IR Sensor: 로드셀 센서 위의 택배를 감지  
         - HX711 LoadCell Sensor: 택배의 무게를 측정  
         - DE1200 Barcode Scaner: 택배의 정보를 인식
         - VL53L0X Laser Sensor: 택배의 끼임 상태를 확인

      (3) **전원**  
         - SMPS: 220V 전압을 24V, 12V로 변환하여 스텝 모터와 리니어 모터에 공급
         - LM2596: 12V를 5V로 변환하여 아두이노와 서보 모터 등 여러 모듈에 공급

   3. **소프트웨어 구성**
        
      (1) **안드로이드 앱**
         - Wi-Fi를 이용하여 실시간 모니터링 화면을 통해 섹터의 상태를 확인

      (2) **서버**
         - PHP 파일을 통해 데이터를 업데이트 및 클라이언트에 상태 전송
         - 일정 값 이하의 데이터가 들어올 경우 문제 발생 상태로 인식

      (3) **데이터베이스**
         - 각 섹터의 레이저 센서 데이터 저장  
  
## 구조

## DFD

## FlowChart

## 코드 파일
1. **php**
   - sector_1 // VL53L0X 레이저 센서 1의 상태 업데이트
   - sector_2 // VL53L0X 레이저 센서 2의 상태 업데이트
   - sector_3 // VL53L0X 레이저 센서 3의 상태 업데이트
   - sector_4 // VL53L0X 레이저 센서 4의 상태 업데이트
   - sector_insert // sector_1~4의 쿼리문이 UPDATE이므로 첫 데이터 삽입을 위한 INSERT 쿼리문  
  
2. **Android Studio**  
   (1) Java  
      - MainActivity // 실시간 모니터링 화면  

   (2) XML  
      - activity_main  

3. **Arduino**  
   - 3laser // esp8266, 레이저 센서
   - linear // 리니어 모터, IR센서
   - servo_barcode_lcd // 서보모터, 바코드 스캐너, lcd, 로드셀 무게센서
   - stepmotor // 스텝 모터

## 데이터베이스 테이블 구조  
### sector_1 테이블
| 필드명           | 데이터 타입   | 설명                 |
|------------------|---------------|-----------------------|
| id (PRIMARY KEY) | INT(11)       | UPDATE 위한 접근 번호 |
| Times            | DATETIME      | 데이터 삽입 시간      |
| AVG_value        | FLOAT         | VL53L0X의 평균 값     |
| state            | VARCHAR(20)   | 현재 상태(에러/정상)  |  

- sector_2~4 모두 동일한 테이블
  
## 사진 및 영상
### 하드웨어 사진  
![image01](https://github.com/user-attachments/assets/314d692a-7076-44ff-8528-0803608eff3b) 
![image02](https://github.com/user-attachments/assets/455a404e-87f6-4c7c-b93e-45683171e1ab)  

### 테이블
![테이블](https://github.com/user-attachments/assets/84337b07-35ad-40ac-9e1b-0b10e5288f9b) ![테이블 2](https://github.com/user-attachments/assets/b2e0981b-26c6-4fe6-aa5b-b67b2f778fed)  

### 시연 영상
[https://youtube.com/shorts/NbpUv54LRSM?feature=share](https://youtu.be/QqGUVOrVUVM)
