//LCD setup: RS, E, D4, D5, D6, D7
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
#include <math.h>
#include <MPU6050.h>
#include <wire.h>
MPU6050 mpu;
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

//IR sensor Pins
const int IR1 = A4;
const int IR2 = A5;

//constants
const int _FSpeed = 255;  //Forward speed
const int GYRO=131;
const int ACC=16384;
const int CALI=10;


void setup(){
//Gyro setup
  Wire.begin();
  Serial.begin(115200);
  mpu.initialize();
  mpu.setSleepEnabled(false);
  mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_250);
  mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
//LCD setup
  lcd.begin(16,2);
//Pin Modes
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

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

void BWD(int x, int y){
 if (y==1){
 motorA_B(255);
 motorB_B(255);
 delay(10);
 }
 motorA_B(x);
 motorB_B(x);
}

void RS(int x){
  //motorA_F(255);
 // motorB_B(255);
 // delay(10);
  motorA_F(x);
  motorB_B(x*0.7);
}

void LS(int x){
 // motorA_B(255);
 // motorB_F(255);
  //delay(10);
  motorA_B(x*0.7);
  motorB_F(x);//0.4

}

void STOP(){
  motorA_S();
  motorB_S();
}
void BREAK(){
  while (1)
  {
    STOP();
  }
}

void PrintLCD(float x, int col, int row){
lcd.setCursor(col, row);
lcd.print(x);
}
void PrintLcd(char x[], int col, int row){
lcd.setCursor(col, row);
lcd.print(x);
}

//Main Function
void loop(){
int16_t ax,ay,az,gx,gy,gz; 
int i;
float axr; //roll left -ve
float ayr; //pitch up -ve
float azr; //follow gravity
float gzr; //clockwise -ve
float pitch;
float roll;
float rampAngle=0;
int peak=0;
float Z=0;
float t1;
float t2;
float dt;
int lol=1;

//Calibration
mpu.getMotion6(&ax,&ay,&az,&gx,&gy,&gz);
axr=ax;
ayr=ay;
azr=az;
gzr=gz;
lcd.clear();
PrintLcd("Calibrating...",0,0);
delay(1000);
int Cali[3]={axr,ayr,azr};
delay(1000);
lcd.clear();
PrintLcd("Done Calibrating",0,0);
delay(1000);
lcd.clear();
while(lol==1){
  FWD(200,0);
  mpu.getMotion6(&ax,&ay,&az,&gx,&gy,&gz);
  axr=ax-Cali[0];
  ayr=ay-Cali[1];
  azr=az;
  gzr=gz;
  pitch=atan2(-ayr,azr)*180.0/PI;
  roll=atan2(axr,azr)*180.0/PI;

  //ramp angle
  if(rampAngle>pitch){}
  else{rampAngle=pitch;}

  //go straight
  if(round(roll)<10){
    RS(200);
    delay(100);
  }
  else if(round(roll)>10){
    LS(200);
    delay(100);
  }

  //peak
  if(rampAngle>10 && abs(pitch)<6 && peak==0){
    peak=1;
    delay(200);
    RS(150)
    t1=millis();
    while(Z<360){
    t2=millis();
    dt=t2-t1;
    t1=t2;
    Z+=gzr*dt;
    }
    FWD(200,0);
    lol=0;
    }
  else{FWD(200,0);}
//DEBUGGING
PrintLCD(pitch,0,1);

}
lcd.clear();
PrintLcd("DONEE",0,0);
delay(10000);

}
