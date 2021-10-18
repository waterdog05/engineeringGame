#include <MsTimer2.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

Servo servo1;  //servo setup
Servo servo2;

LiquidCrystal_I2C lcd(0x27, 16, 2); //SCL = A4; SDA = A5;

const int startButtonPin = 12;  //starting button setup
int startButtonState = 0;

const int buttonPin1 = 8;  //1p button setup
int buttonState1 = 0;
int lastButtonState1 = 0;
int buttonPushCounter1;

const int buttonPin2 = 10;  //2p button setup
int buttonState2 = 0;
int lastButtonState2 = 0;
int buttonPushCounter2;

int winner = 0;  //1P or 2P
int stopper = 0;

void gameStarting();  //setting game
void btnCnt1();  //button counting function
void btnCnt2();
//int stopCountPush();  //function that ends game after 1m for MsTimer2
void showResult();  //show game result
void gameReset();  //reset game

void setup() {
  Serial.begin(9600);

  lcd.begin();  //lcd setup
  lcd.backlight();

  servo1.attach(2);  //servo setup
  servo2.attach(3);
  
  pinMode(buttonPin1, INPUT);  //buttonPin setup
  pinMode(buttonPin2, INPUT);
  pinMode(startButtonPin, INPUT);

  MsTimer2::set(20000, stopCountPush);  //game play time = 20sec
}

void loop() {
  startButtonState = digitalRead(startButtonPin);
  if (startButtonState == HIGH) {
    gameStarting();
    
    MsTimer2::start();
    
    for (;;) {
      btnCnt1();  //count push num
      btnCnt2();
      
      if (stopper == 1) {
        MsTimer2::stop();
        break;
      }
    }
    
    showResult();  //end game and show result
    gameReset();  //reset game
  }
}

void gameStarting() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Engineering Game");  //Engineering Game
  delay(2000);
  lcd.clear();
  
  lcd.setCursor(7, 0);  //3 2 1
  lcd.print(3);
  delay(1000);
  lcd.print(2);
  delay(1000);
  lcd.print(1);
  delay(1000);
  
  lcd.setCursor(5, 0);  //start!
  lcd.print("start!");
  delay(1000);
  lcd.clear();  //7sec
}

void btnCnt1() {
  buttonState1 = digitalRead(buttonPin1);  //count 1P's btn push
  
  if (buttonState1 != lastButtonState1) {
    if (buttonState1 == HIGH) {
      buttonPushCounter1++;
    }
  //delay(50);
  }
  lastButtonState1 = buttonState1;

  lcd.setCursor(0, 0);  //print 1P's push count on lcd
  lcd.print("1P:");
  lcd.setCursor(3, 0);
  //buttonPushCounter = 3; //for test
  lcd.print(buttonPushCounter1);
}

void btnCnt2() {  
  buttonState2 = digitalRead(buttonPin2);  //count 2P's btn push
  
  if (buttonState2 != lastButtonState2) {
    if (buttonState2 == HIGH) {
      buttonPushCounter2++;
    }
  //delay(50);
  }
  lastButtonState2 = buttonState2;

  lcd.setCursor(10, 0);  //print 2P's push count on lcd
  lcd.print("2P:");
  lcd.setCursor(13, 0);
  lcd.print(buttonPushCounter2);
}

int stopCountPush() {
  stopper = 1;
}

void showResult() {
  if (buttonPushCounter1 > buttonPushCounter2) {  //1P win
    winner = 1;
    
    lcd.setCursor(0, 1);
    lcd.print("Win!");
    lcd.setCursor(10, 1);
    lcd.print("Lose");
    delay(5000);
    
    servo1.write(180);
    servo2.write(180);
    delay(2000);
    servo1.write(90);
    servo2.write(90);
    delay(3000);

    lcd.clear();
  }
  else if (buttonPushCounter1 < buttonPushCounter2) {  //2P win
    winner = 2;
    
    lcd.setCursor(0, 1);
    lcd.print("Lose");
    lcd.setCursor(10, 1);
    lcd.print("Win!");
    delay(5000);
    
    servo1.write(0);
    servo2.write(0);
    delay(2000);
    servo1.write(90);
    servo2.write(90);
    delay(3000);
  }
  else { 
    winner = 3;
    lcd.setCursor(6, 1);  //tie
    lcd.print("Tie!");
    delay(10000);
  }
}

void gameReset() {
  lcd.clear();
  stopper = 0;
  buttonPushCounter1 = 0;
  buttonPushCounter2 = 0;
  
  switch (winner) {
    case 1:
      winner = 0;
      servo1.write(0);
      servo2.write(0);
      delay(2000);
      servo1.write(90);
      servo2.write(90);
      break;
      
    case 2:
      winner = 0;
      servo1.write(180);
      servo2.write(180);
      delay(2000);
      servo1.write(90);
      servo2.write(90);
      break;
    case 3:
      winner = 0;
      stopper = 0;
      break;
  }
  lcd.setCursor(0, 0);
  lcd.print("Game Resetting...");
  delay(3000);
  lcd.clear();
  
  lcd.setCursor(1, 0);
  lcd.print("Game Resetted!");
  delay(2500);
  lcd.clear();
}
