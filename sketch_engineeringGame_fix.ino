#include <MsTimer2.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

Servo servo1;  //servo setup
Servo servo2;
const int servo1Pin = 2;
const int servo2Pin = 3;

LiquidCrystal_I2C lcd(0x27, 20, 4); //SCL = A4; SDA = A5;

const int startButtonPin = 10;  //starting button setup
int startButtonState = 0;

int firstp_Led = 4;
int secondp_Led = 6;

const int buttonPin1 = 8;  //1p button setup
int buttonState1 = 0;
int lastButtonState1 = 0;
int buttonPushCounter1;

const int buttonPin2 = 12;  //2p button setup
int buttonState2 = 0;
int lastButtonState2 = 0;
int buttonPushCounter2;

int winner = 0;  //1P or 2P
int finWinner = 0;
int cntFirstPWin = 0;
int cntSecondPWin = 0;
int stopper = 0;

void gameStarting();  //setting game
void btnCnt1();  //button counting function
void btnCnt2();
//int stopCountPush();  //function that ends game after 1m for MsTimer2
void moveBelt();  //move belt during game
void ShowResult();  //show game result
void gameReset();  //reset game

void setup() {
  Serial.begin(9600);

  lcd.begin();  //lcd setup
  lcd.backlight();

  servo1.attach(servo1Pin);  //servo setup
  servo2.attach(servo2Pin);
  
  pinMode(buttonPin1, INPUT);  //buttonPin setup
  pinMode(buttonPin2, INPUT);
  pinMode(startButtonPin, INPUT);

  pinMode(firstp_Led, OUTPUT);
  pinMode(secondp_Led, OUTPUT);

  MsTimer2::set(10000, stopCountPush);  //game play time = 20sec
}

void loop() {
  startButtonState = digitalRead(startButtonPin);
  if (startButtonState == HIGH) {
    gameStarting();
    
    lcd.setCursor(2, 0);
    lcd.print("Engineering Game");
    
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
  lcd.setCursor(2, 1);
  lcd.print("Engineering Game");  //Engineering Game
  delay(2000);
  lcd.clear();
  
  lcd.setCursor(9, 1);  //3 2 1
  lcd.print(3);
  delay(1000);
  lcd.print(2);
  delay(1000);
  lcd.print(1);
  delay(1000);
  
  lcd.setCursor(7, 1);  //start!
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

  /*
  lcd.setCursor(1, 2);  //print 1P's push count on lcd
  lcd.print("1P:");
  lcd.setCursor(4, 2);
  //buttonPushCounter = 3; //for test
  lcd.print(buttonPushCounter1);
  */
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
  
  /*
  lcd.setCursor(14, 2);  //print 2P's push count on lcd
  lcd.print("2P:");
  lcd.setCursor(17, 2);
  lcd.print(buttonPushCounter2);
  */
}

/*
int stopCountPush() {
  stopper = 1;
}
*/

void moveBelt() {
  if (cntFirstPWin > cntSecondPWin) {  //1P win
    finWinner = 1;
    
    digitalWrite(firstp_Led, LOW);
    servo1.write(100);
    servo2.write(100);
    delay(2000);
    servo1.write(90);
    servo2.write(90);
    delay(3000);
  }
  else if (cntFirstPWin < cntSecondPWin) {  //2P win
    finWinner = 2;
    
    servo1.write(80);
    servo2.write(80);
    delay(2000);
    servo1.write(90);
    servo2.write(90);
    delay(3000);
  }
  else { 
    finWinner = 3;

    digitalWrite(firstp_Led, HIGH);
    digitalWrite(secondp_Led, HIGH);
    lcd.setCursor(8, 3);  //tie
    lcd.print("Tie!");
    delay(10000);
    digitalWrite(firstp_Led, LOW);
    digitalWrite(secondp_Led, LOW);
  }
}

void showResult() {
  switch (finWinner) {
    case 1:
      digitalWrite(firstp_Led, HIGH);
      lcd.setCursor(2, 3);
      lcd.print("Win!");
      lcd.setCursor(14, 3);
      lcd.print("Lose");
      //delay(5000);

      servo1.write(100);  //move 2cm
      servo2.write(100);
      delay(2000);
      servo1.write(90);
      servo2.write(90);
      //delay(3000);
      digitalWrite(firstp_Led, LOW);
      break;

    case 2:
      digitalWrite(secondp_Led, HIGH); 
      lcd.setCursor(2, 3);
      lcd.print("Lose");
      lcd.setCursor(14, 3);
      lcd.print("Win!");
      //delay(5000);

      servo1.write(80);
      servo2.write(80);
      delay(2000);
      servo1.write(90);
      servo2.write(90);
      //delay(3000);
      digitalWrite(secondp_Led, LOW);
      break;

    case 3:
      

  }
}

void gameReset() {
  lcd.clear();
  stopper = 0;
  buttonPushCounter1 = 0;
  buttonPushCounter2 = 0;
  winner = 0;
  

  lcd.setCursor(2, 1);
  lcd.print("Game Resetting..");
  
  switch (finWinner) {
    case 1:
      winner = 0;
      servo1.write(80);  //+10cm move
      servo2.write(80);
      delay(2220);
      servo1.write(90);
      servo2.write(90);
      break;
      
    case 2:
      winner = 0;
      servo1.write(100);  //-10cm move
      servo2.write(100);
      delay(2220);
      servo1.write(90);
      servo2.write(90);
      break;
      
    case 3:
      winner = 0;
      break;
  }

  delay(1500);
  lcd.clear();
  lcd.setCursor(3, 1);
  lcd.print("Game Resetted!");
  delay(1500);
  lcd.clear();
}
