#include <Servo.h>
#include <LiquidCrystal_I2C.h>

Servo servo1;  //servo setup
Servo servo2;
int pos1 = 90;
int pos2 = 90;

LiquidCrystal_I2C lcd(0x27, 16, 2); //SCL = A4; SDA = A5;

int trigPin1 = 4;  //trig, echo setup
int echoPin1 = 5;
int trigPin2 = 6;
int echoPin2 = 7;

const int buttonPin1 = 8;  //1p button setup
int buttonState1 = 0;
int lastButtonState1 = 0;
int buttonPushCounter1;

const int buttonPin2 = 10;  //2p button setup
int buttonState2 = 0;
int lastButtonState2 = 0;
int buttonPushCounter2;

int btnCnt1();  //button counting function
int btnCnt2();
void showResult();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  lcd.begin();
  lcd.backlight();
  
  servo1.attach(2);
  servo2.attach(3);
  
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:  
  buttonPushCounter1 = btnCnt1();  //count push
  buttonPushCounter2 = btnCnt2();

  lcd.setCursor(0, 0);
  lcd.print("1P:");
  lcd.setCursor(10, 0);
  lcd.print("2P:");
  lcd.setCursor(3, 0);
  lcd.print(buttonPushCounter1);
  lcd.setCursor(13, 0);
  lcd.print(buttonPushCounter2);
  delay(70);
  lcd.clear();
  
  
}

int btnCnt1() {
  buttonState1 = digitalRead(buttonPin1);
  
  if (buttonState1 != lastButtonState1) {
    if (buttonState1 == HIGH) {
      buttonPushCounter1++;
    }

  }
  lastButtonState1 = buttonState1;

  return buttonPushCounter1;
}

int btnCnt2() {
  buttonState2 = digitalRead(buttonPin2);
  
  if (buttonState2 != lastButtonState2) { //72 86
    if (buttonState2 == HIGH) {
      buttonPushCounter2++;
    }

  }
  lastButtonState2 = buttonState2;

  return buttonPushCounter2;
}
