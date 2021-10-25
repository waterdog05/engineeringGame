#include <MsTimer2.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

Servo servo1;  //servo setup
Servo servo2;
const int servo1Pin = 2;
const int servo2Pin = 3;

LiquidCrystal_I2C lcd(0x27, 20, 4); //SCL = A4; SDA = A5;

const int startButtonPin = 8;  //starting button setup
int startButtonState = LOW;

int firstp_Led = 4;  //Led setup
int secondp_Led = 6;

const int buttonPin1 = 10;  //1p button setup
int buttonState1 = 0;
int lastButtonState1 = 0;
int buttonPushCounter1 = 0;

const int buttonPin2 = 12;  //2p button setup
int buttonState2 = 0;
int lastButtonState2 = 0;
int buttonPushCounter2 = 0;

int finWinner = 0;  //final winner
int cntFirstPWin = 0;  //for calculating final winner
int cntSecondPWin = 0;
int stopper = 0;  //break inner for(;;)
int i = 0;  //break outer for(;;)

void gameStarting();  //setting game
void btnCnt1();  //button counting function
void btnCnt2();
int stopCountPush();  //function that ends game after 1m for MsTimer2
void moveBelt();  //move belt during game
void calcFinWinner();  //choose final winner
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

  pinMode(firstp_Led, OUTPUT);  //LedPin setup
  pinMode(secondp_Led, OUTPUT);

  MsTimer2::set(1000, stopCountPush);  //game play time = 20sec
}

void loop() {
  startButtonState = digitalRead(startButtonPin);  //If button is pressed
  if (startButtonState == HIGH) {
    gameStarting();  //start game
    
    for (;;) {
      lcd.setCursor(2, 0);
      lcd.print("Engineering Game");
      
      MsTimer2::start();  //start Timer to break for(;;) in line 72
      for (;;) {
        btnCnt1();
        btnCnt2();
    
        if (stopper == 1) {
          MsTimer2::stop();
          break;
        }
      }
      moveBelt();  //move belt with the stored num
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Engineering Game");
      stopper = 0;
      i++;
      
      if (i == 20) {  //set i to break for(;;) in line 67
        i = 0;
        break;
      }
    }

    /*
    lcd.setCursor(4, 2);
    lcd.print(buttonPushCounter1);  //print win num
    lcd.setCursor(9, 2);
    lcd.print("vs");
    lcd.setCursor(15, 2);
    lcd.print(buttonPushCounter2);
    */
    
    calcFinWinner();  //calculate final winner
    delay(1000);
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

  lcd.setCursor(1, 2);  //print 1P's push count on lcd
  lcd.print("1P:");
  lcd.setCursor(4, 2);
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
  
  lcd.setCursor(14, 2);  //print 2P's push count on lcd
  lcd.print("2P:");
  lcd.setCursor(17, 2);
  lcd.print(buttonPushCounter2);
}

int stopCountPush() {
  stopper = 1;
}

void moveBelt() {
  if (buttonPushCounter1 > buttonPushCounter2) {  //1P win
    cntFirstPWin += 1;
    
    servo1.write(120);
    servo2.write(120);
    delay(100);
    servo1.write(90);
    servo2.write(90);
  }
  else if (buttonPushCounter1 < buttonPushCounter2) {  //2P win
    cntSecondPWin += 1;
    
    servo1.write(60);
    servo2.write(60);
    delay(100);
    servo1.write(90);
    servo2.write(90);
  }
  else {
    cntFirstPWin = cntFirstPWin;  //tie
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
  else {  //tie
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

      servo1.write(100);  //move +2cm
      servo2.write(100);
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

      servo1.write(80);  //move -2cm alter later
      servo2.write(80);
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
      delay(9000);
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
  
  switch (finWinner) {  //reset belt condition
    case 1:
      finWinner = 0;
      servo1.write(80);  //+10cm move
      servo2.write(80);
      delay(1000);
      servo1.write(90);
      servo2.write(90);

      for (j = 0; j < (cntFirstPWin - cntSecondPWin); j++) {
        servo1.write(60);
        servo2.write(60);
        delay(100);
      }
      servo1.write(90);
      servo2.write(90);
      break;
      
    case 2:
      finWinner = 0;
      servo1.write(100);  //-10cm move
      servo2.write(100);
      delay(1000);
      servo1.write(90);
      servo2.write(90);

      for (j = 0; j < (cntSecondPWin - cntFirstPWin); j++) {
        servo1.write(120);
        servo2.write(120);
        delay(100);
      }
      servo1.write(90);
      servo2.write(90);
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
