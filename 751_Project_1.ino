//www.elegoo.com
//2016.12.08
#include <LiquidCrystal.h>
#include "SR04.h"
#include <PID_v1.h>
#define TRIG_PIN 5
#define ECHO_PIN 3

#include "Stepper.h"
#define STEPS  32 

Stepper small_stepper(STEPS, 2, 6, 4, 9);
LiquidCrystal lcd(7, 8, 13, 10, 11, 12);

SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
float a, b, c, duration, d, theta, s;

double Setpoint, Input, Output;
double Kp=0, Ki=10, Kd=0;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void setup() {
  Serial.begin(9600);
  delay(1000);

  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Distance (cm):");

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Setpoint=5.5;
  myPID.SetMode(AUTOMATIC);
  small_stepper.setSpeed(200);
  
}

void loop() 
{
  //CALCULATE DISTANCE
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);
  duration = pulseIn(ECHO_PIN, HIGH);
  d = (duration/2) / 29.1;


   lcd.setCursor(0, 1);
   lcd.print(d);
   lcd.print(" ");

  Input=d;
  myPID.Compute();
  a=map((Output),0,255,0,256); 
  
  if(d>Setpoint)
  {
    c=a+5;
    s=100*(d-Setpoint);
    delay(10);
  } 

  else if(d<Setpoint)
  {
    c=-1*a;
    s=100*(Setpoint-d);
    delay(10);
  }
  
//  else if(d=Setpoint)
//  {
//    c=0;
//    s=0;
//  }


  Serial.print("Distance:");
  Serial.print(d);
  Serial.print(",");
  Serial.print("Setpoint:");
  Serial.println(Setpoint);

  small_stepper.setSpeed(s);
  small_stepper.step(c);
//  

 
  delay(0);
  
}





//theta=acos((21-d)/18);
//  b=-256*(theta)/90;
//  
//  if (Output<-255)
//  {
//   //a=a+5;
//   small_stepper.step(0);
//   delay(100);
//  }
  
//  if (Output>45)
//  {
//   a=a-5;
//   small_stepper.step(a);
//   delay(100);   
//  }
