#include <MsTimer2.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

Servo servo1;  //서보모터 객체 선언
Servo servo2;
const int servo1Pin = 2;
const int servo2Pin = 3;

LiquidCrystal_I2C lcd(0x27, 20, 4); //lcd 스크린 설정

const int startButtonPin = 8;  //시작 버튼 설정
int startButtonState = LOW;

int firstp_Led = 4;  //Led 설정
int secondp_Led = 6;

const int buttonPin1 = 10;  //1p 버튼 설정
int buttonState1 = 0;
int lastButtonState1 = 0;
int buttonPushCounter1 = 0;

const int buttonPin2 = 12;  //2p 버튼 설정
int buttonState2 = 0;
int lastButtonState2 = 0;
int buttonPushCounter2 = 0;

int finWinner = 0;  //최종 우승자
int cntFirstPWin = 0;  //최종우승자 결정위한 값 저장
int cntSecondPWin = 0;
int stopper = 0;  //내부 무한루프 탈출용
int i = 0;  //외부 무한루프 탈출용

void gameStarting();  //게임 셋업 함수
void btnCnt1();  //버튼 누른 횟수 세는 함수
void btnCnt2();
int stopCountPush();  //stopper 값 변경 함수
void moveBelt();  //모터 구동 함수
void calcFinWinner();  //최종 우승자 결정 함수
void ShowResult();  //게임 결과 출력 함수
void gameReset();  //게임 리셋 함수

void setup() {
  Serial.begin(9600);

  lcd.begin();
  lcd.backlight();

  servo1.attach(servo1Pin); 
  servo2.attach(servo2Pin);
  
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(startButtonPin, INPUT);

  pinMode(firstp_Led, OUTPUT);
  pinMode(secondp_Led, OUTPUT);

  MsTimer2::set(1000, stopCountPush);  //playing time = 20sec
}

void loop() {
  startButtonState = digitalRead(startButtonPin);
  if (startButtonState == HIGH) {
    gameStarting();
    
    for (;;) {
      lcd.setCursor(2, 0);
      lcd.print("Engineering Game");
      
      MsTimer2::start();
      for (;;) {
        btnCnt1();
        btnCnt2();
    
        if (stopper == 1) {
          MsTimer2::stop();
          break;
        }
      }
      moveBelt();
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Engineering Game");
      stopper = 0;
      i++;
      
      if (i == 20) {
        i = 0;
        break;
      }
    }
    
    calcFinWinner();
    delay(500);
    showResult();
    gameReset();
  }
}

void gameStarting() {
  lcd.clear();
  lcd.setCursor(2, 1);
  lcd.print("Engineering Game");
  delay(2000);
  lcd.clear();
  
  lcd.setCursor(9, 1);
  lcd.print(3);
  delay(1000);
  lcd.print(2);
  delay(1000);
  lcd.print(1);
  delay(1000);
  
  lcd.setCursor(7, 1);
  lcd.print("start!");
  delay(1000);
  lcd.clear();
}

void btnCnt1() {
  buttonState1 = digitalRead(buttonPin1);
  
  if (buttonState1 != lastButtonState1) {
    if (buttonState1 == HIGH) {
      buttonPushCounter1++;
    }
  //delay(50);
  }
  lastButtonState1 = buttonState1;

  lcd.setCursor(1, 2);
  lcd.print("1P:");
  lcd.setCursor(4, 2);
  lcd.print(buttonPushCounter1);
}

void btnCnt2() {  
  buttonState2 = digitalRead(buttonPin2);
  
  if (buttonState2 != lastButtonState2) {
    if (buttonState2 == HIGH) {
      buttonPushCounter2++;
    }
  //delay(50);
  }
  lastButtonState2 = buttonState2;
  
  lcd.setCursor(14, 2);
  lcd.print("2P:");
  lcd.setCursor(17, 2);
  lcd.print(buttonPushCounter2);
}

int stopCountPush() {
  stopper = 1;
}

void moveBelt() {
  if (buttonPushCounter1 > buttonPushCounter2) {  //1P > 2P
    cntFirstPWin += 1;
    
    servo1.write(75);
    servo2.write(105);
    delay(100);
    servo1.write(90);
    servo2.write(90);
  }
  else if (buttonPushCounter1 < buttonPushCounter2) {  //1P < 2P
    cntSecondPWin += 1;
    
    servo1.write(105);
    servo2.write(75);
    delay(100);
    servo1.write(90);
    servo2.write(90);
  }
  else {
    cntFirstPWin = cntFirstPWin;  //1P == 2P
    cntSecondPWin = cntSecondPWin;
  }
}

void calcFinWinner() {
  if (cntFirstPWin > cntSecondPWin) {  //1P win
    finWinner = 1;
  }
  else if (cntFirstPWin < cntSecondPWin) {  //2P win
    finWinner = 2;
  }
  else {  //1P == 2P
    finWinner = 3;
  }
}

void showResult() {
  switch (finWinner) {
    case 1:  //1P win
      digitalWrite(firstp_Led, HIGH);
      lcd.setCursor(2, 2);
      lcd.print("Win!");
      lcd.setCursor(14, 2);
      lcd.print("Lose");
      //delay(5000);

      servo1.write(75);
      servo2.write(105);
      delay(1000);
      servo1.write(90);
      servo2.write(90);
      delay(3000);
      digitalWrite(firstp_Led, LOW);
      break;

    case 2:  //2P win
      digitalWrite(secondp_Led, HIGH); 
      lcd.setCursor(2, 2);
      lcd.print("Lose");
      lcd.setCursor(14, 2);
      lcd.print("Win!");
      //delay(5000);

      servo1.write(105);
      servo2.write(75);
      delay(1000);
      servo1.write(90);
      servo2.write(90);
      delay(3000);
      digitalWrite(secondp_Led, LOW);
      break;

    case 3:  //tie
      digitalWrite(firstp_Led, HIGH);
      digitalWrite(secondp_Led, HIGH);
      lcd.setCursor(8, 2);  //tie
      lcd.print("Tie!");
      delay(4000);
      digitalWrite(firstp_Led, LOW);
      digitalWrite(secondp_Led, LOW);
      break;

  }
}

void gameReset() {
  int j = 0;
  lcd.clear();
  stopper = 0; 
  buttonPushCounter1 = 0;  //reset every variables
  buttonPushCounter2 = 0;
  i = 0;

  lcd.setCursor(2, 1);
  lcd.print("Game Resetting..");
  
  switch (finWinner) {  //reset belt position
    case 1:
      finWinner = 0;

      servo1.write(105);  //+10cm move
      servo2.write(75);
      delay(800);
      servo1.write(90);
      servo2.write(90);


      for (j = 0; j < (cntFirstPWin - cntSecondPWin); j++) {
        servo1.write(105);
        servo2.write(75);
        delay(80);
        servo1.write(90);
        servo2.write(90);
      }
      break;
      
    case 2:
      finWinner = 0;

      servo1.write(80);  //-10cm move
      servo2.write(100);
      delay(800);
      servo1.write(90);
      servo2.write(90);

      for (j = 0; j < (cntSecondPWin - cntFirstPWin); j++) {
        servo1.write(75);
        servo2.write(105);
        delay(80);
        servo1.write(90);
        servo2.write(90);
      }
      break;
      
    case 3:
      finWinner = 0;
      delay(1000);
      break;
  }
  cntFirstPWin = 0;
  cntSecondPWin = 0;
  j = 0;
  
  lcd.clear();
  lcd.setCursor(3, 1);
  lcd.print("Game Resetted!");
  delay(1500);
  lcd.clear();
  startButtonState = LOW;
}
