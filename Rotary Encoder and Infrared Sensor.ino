// LCD setup: RS, E, D4, D5, D6, D7
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
#include <math.h>
#include <string.h>

// Motor A pins
const int IN1 = 2;
const int IN2 = 1;
const int ENA = 3;

// Motor B pins
const int IN3 = 12;
const int IN4 = 13;
const int ENB = 11;

//motor calibration
const float _OFFSETA = 0.83;
const float _OFFSETB = 0.9;

//Encoder Pins
const int EN1 = A3;
const int EN2 = A4;

//IR sensor Pins
const int IR1 = A1;
const int IR2 = A2;

//constants
const int _SPEED = 255;
const int _TIME = 150;
const float _DISTANCE=1.035/2;

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
//Directions
void FWD(int x,int y){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA,x*_OFFSETA);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB,x);
  delay(y);
}
void BWD(int x,int y){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA,x*_OFFSETB);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB,x);
  delay(y);
}
void RTF(int x,int y){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA,x*_OFFSETA);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB,x/2);
  delay(y);
}
void RTB(int x,int y){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB,x);
  delay(y);
}
void RS(int x,int y){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA,(x/1.25)*_OFFSETA);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB,x);
  delay(y);
}
void LTF(int x,int y){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA,x/2);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB,x);
  delay(y);
}
void LTB(int x,int y){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA,x*_OFFSETB);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  delay(y);
}
void LS(int x,int y){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA,(x)*_OFFSETB);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB,x/1.25);
  delay(y);
}
void STOP(int x){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);  
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);  
  delay(x);
}
void BREAK(){
  while (1)
  {
    STOP(0);
  }
}
//Functions
void Print(float x, float y){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(x);
  lcd.setCursor(0, 1);
  lcd.print(y/1000);
  //lcd.print(strcat(x,"cm"));
  delay(10);
}
int EncoderRAW(int T){
  int x=0;
  int y=0;
  int z=0;
  int B=0;
  while(z<(T/10)){
    if (digitalRead(EN1)==HIGH && digitalRead(EN2)==HIGH){
      ++x;
      ++y;
      //while(digitalRead(EN1)==HIGH && digitalRead(EN2)==HIGH){}
    }
    else if (digitalRead(EN2)==HIGH){
      ++y;
      //while(digitalRead(EN2)==HIGH){}
    }
    else if (digitalRead(EN1)==HIGH){
      ++x;
      //while(digitalRead(EN1)==HIGH){}
    }
    return(x,y);
    delay(10);
    ++z;
  }
}

//Main Function
void loop() {
  int x=0;
  int y=0;
  int L=0;
  int R=0;
  int z=1;
  int T=0;
  float Distance=0;
while (1){
if (digitalRead(IR1)==LOW && digitalRead(IR2)==LOW){
  //BREAK();
  Print(Distance*_DISTANCE*2, T*35);
  BREAK();
}
else if (digitalRead(IR1)==LOW){//Left Turn
  R=0;
  if (L<2){//Checks for repeated left turns, indicates a larger turn
    while (digitalRead(IR2)==HIGH){
     LS(200,50);
     x,y=EncoderRAW(50);
      Distance+=(-x+y);
      ++T;
    }
    RS(_SPEED,_TIME);//correctiing oversteer
    z=0;
    x,y=EncoderRAW(_TIME);
    Distance+=(x-y);
    //STOP(1000);
    ++L;
  }
  else{
    LS(200,0);
    x,y=EncoderRAW(50);
    Distance+=(-x+y);
  }
}
else if (digitalRead(IR2)==LOW){//Right Turn
  L=0;      
  if (R<2){//Checks for repeated right turns, indicates a larger turn
    while (digitalRead(IR1)==HIGH){
      RS(200,50);
      x,y=EncoderRAW(50);
      Distance+=(x-y);
      ++T;
    }
    LS(_SPEED,_TIME);//correcting oversteer
    z=0;
    x,y=EncoderRAW(_TIME);
    Distance+=(-x+y);
    //STOP(1000);
    ++R;
  }
  else{
    RS(200,50);
    x,y=EncoderRAW(50);
    Distance+=(x-y);       
  }
}
else{//Forwards
  FWD(255,20);
  x,y=EncoderRAW(20);
  Distance+=(x+y);
  ++T;
}
//delay(50);
//Print(Distance*_DISTANCE*2.5, x);
}
}