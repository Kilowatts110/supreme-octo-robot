//LCD setup: RS, E, D4, D5, D6, D7
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
#include <math.h>

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
const int TRIG = A1;
const int ECHO = A2;

//constants
const int _TIME = 350;    //Oversteer correction time
const int _SPEED = 255;   //Oversteer correction speed
const int _FSpeed = 255;  //Forward speed
const int _TSpeed = 255;  //Turning speed

void setup() {
//LCD setup
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

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
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
void BREAK()
{
  while (1)
  {
    STOP(0);
  }
}

int Encoder(int z){
  int x=0;
  int y=0;
  int B=0;
  while (B<z){
    if (digitalRead(EN1) == HIGH && digitalRead(EN2) == HIGH) {
      ++x;
      ++y;
      //while(digitalRead(EN1)==HIGH && digitalRead(EN2)==HIGH){}
    } else if (digitalRead(EN2) == HIGH && digitalRead(EN1) == LOW) {
      ++y;
      //while(digitalRead(EN2)==HIGH){}
    } else if (digitalRead(EN1) == HIGH && digitalRead(EN2) == LOW) {
      ++x;
      //while(digitalRead(EN1)==HIGH){}
    }
    return (x, y);
  delay(50);
  ++B;
  }
return(x,y);
}
void PrintLCD(char x[],char y[])
{
  lcd.clear();
  //lcd.setCursor(0,0);
  lcd.print(x);
  lcd.setCursor(0,1);
  lcd.print(y);
  //delay(50);
}
void TTRIG(){
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
}
//Main Function
void loop() {
  long durian;
  int dist;
  while (1){
  TTRIG();
  durian=pulseIn(ECHO, HIGH);
  dist = durian * 0.034 / 2;
  if (dist<25){
    PrintLCD("Obstacle");
    STOP(400);
    LS(255);
    delay(500);
  }
  else if(dist>=25){
    lcd.clear();
    FWD(255,0);
  }
  }
}