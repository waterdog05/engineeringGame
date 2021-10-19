#include <Servo.h> //서보관련 라이브러리를 사용하기 위해

Servo myservo;  // 서보 변수 선언
Servo myservo1; 

const int servoPin = 3; // 서보 핀
const int servo1Pin = 2;

void setup() {
  myservo.attach(servoPin); //서보로 9핀 사용하겠다고 설정 
  myservo1.attach(servo1Pin);
}

void loop() {
  myservo.write(100);  //0도로 이동        
  myservo1.write(100);     
  delay(2000);
  myservo.write(80);
  myservo1.write(80);
  delay(2000);
}
