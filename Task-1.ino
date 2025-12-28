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
const int IR1 = A1;
const int IR2 = A2;

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

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
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
  motorA_B(x*0.7);
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
void PrintLCD(float x,float y)
{
  lcd.clear();
  //lcd.setCursor(0,0);
  lcd.print(x);
  lcd.setCursor(0,1);
  lcd.print(y);
  //delay(50);
}

//Main Function
void loop() {
  float _Distance=0;
  float _DistanceINT=0;
  int E1b=0;
  int E2b=0;
  float E1=0;
  float E2=0;
  long double x=0; //distance
  int y=0; //tracker
  int R=0;
  int L=0;
  int count=0;
  long double z=0; //time
  int ST=1;
  while(ST==1){
  ++z;
 if (digitalRead(IR1)==LOW && digitalRead(IR2)==LOW) //both black
  {
    ST=0;
    PrintLCD(x/0.9,z/144);
    BREAK();
  }
  else if (digitalRead(IR1)==LOW) //left
  {
    R = 0;
    if (L < 2) {  //Checks for repeated left turns, indicates a larger turn
       while (digitalRead(IR2) == HIGH) {
        LS(_TSpeed);
        delay(100);
        }
      RS(_SPEED);  //correctiing oversteer
      delay(_TIME);
      ++L;
      z=z+160;
    } 
    else {
      E1b,E2b=Encoder(1);
      LS(_TSpeed);
      

    }
  }
  else if (digitalRead(IR2)==LOW) //right
  {
    L = 0;
    if (R < 2) {  //Checks for repeated right turns, indicates a larger turn
      while (digitalRead(IR1) == HIGH) {
        RS(_TSpeed);
      }
      LS(_SPEED);  //correcting oversteer
      delay(_TIME);
      ++R;
      z=z+160;
    } 
    else {
      E1b,E2b=Encoder(1);
      RS(_TSpeed);
      
    }
  }
  else{
    FWD(_FSpeed,0);
    E1b,E2b=Encoder(1);
    E1+=E1b*1.035;
    E2+=E2b*1.035;
    if (round(x/0.9)==30 && y==0){
      STOP(0);
      delay(2000);
      z=z+320;
      ++y;
    }    
  }
  x=E1+E2; //distance
  lcd.clear();
  PrintLCD(x/0.9,z/144);
  }

}
