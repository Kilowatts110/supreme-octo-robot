

//LCD setup: RS, E, D4, D5, D6, D7
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
#include <math.h>
#include <SoftwareSerial.h>

// Motor A pins
const int IN1 = 2;
const int IN2 = 1;
const int ENA = 3;

// Motor B pins
const int IN3 = 12;
const int IN4 = 13;
const int ENB = 11;

//motor calibration
const float _OFFSETA = 1;
const float _OFFSETB = 0.9;

//Encoder Pins
const int EN1 = A3;
const int EN2 = A4;

//IR sensor Pins

//constants
const int _TIME = 350;    //Oversteer correction time
const int _SPEED = 255;   //Oversteer correction speed
const int _FSpeed = 255;  //Forward speed
const int _TSpeed = 255;  //Turning speed
SoftwareSerial BT(A1,A2);//TX RX

void setup() {
//LCD setup
  //Serial.begin(9600);
  BT.begin(9600);
  lcd.begin(16,2);
//Pin Modes
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(EN1, INPUT);
  pinMode(EN2, INPUT);

  //pinMode(1, OUTPUT);
  //pinMode(0, INPUT);
}
// Raw instructions
void motorA_F(int x) { 
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA,x*_OFFSETA);
}
void  motorA_B(int x) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA,x*_OFFSETB);
}
void motorA_S() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}
void motorB_F(int x) {
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB,x);
}

void motorB_B(int x) {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB,x);
}
void motorB_S() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
//Directional instructions
void FWD(int x, int y)
{
  if (y==1){
  motorA_F(255);
  motorB_F(255);
  delay(10);
  }
  motorA_F(x);
  motorB_F(x);
}

void BWD(int x, int y)
{
 if (y==1){
 motorA_B(255);
 motorB_B(255);
 delay(10);
 }
 motorA_B(x);
 motorB_B(x);
}

void LT(int x)
{
  //motorB_F(255);
  //delay(10);
  motorB_F(x);
  motorA_S();
}

void RT(int x)
{
  //motorA_F(255);
  //delay(10);
  motorB_S();
  motorA_F(x);
}

void RS(int x)
{
  //motorA_F(255);
 // motorB_B(255);
 // delay(10);
  motorA_F(x);
  motorB_B(x*0.7);
}

void LS(int x)
{
 // motorA_B(255);
 // motorB_F(255);
  //delay(10);
  motorA_B(x*0.5);
  motorB_F(x);//0.4

}

void STOP(int x)
{
  motorA_S();
  motorB_S();
  delay(x);
}
//Main Function
void loop() {
char c;
if(BT.available()){
c=BT.read();
  if(c=='8'){
    FWD(255,1);
  }
  else if(c=='2'){BWD(255,0);
  }
  else if(c=='4'){LS(255);
  }
  else if (c=='6'){RS(255);
  }
  else if (c=='5'){STOP(10);
  }
}
}