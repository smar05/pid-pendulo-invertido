#include <Wire.h>
#include <PID_v1.h>

#define in1 6
#define in2 7
#define pot A0

int angulo=0;
double Setpoint = 536;
double Input = 0;
double Output = 0;
const int posEquilibrio = 536;

double aggKp=12.23, aggKi=21.0172, aggKd=1.7592;
double consKp=15, consKi=5, consKd=5;//double consKp=20 | 15, consKi=5, consKd=5;

PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);

int encoder;

double entrada;

void setup() {
  // put your setup code here, to run once:
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  //pinMode(pot, OUTPUT);  

  Serial.begin(9600);
  Wire.begin();

  Input = 0;
  Setpoint = posEquilibrio;
  Output = 0;
  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(20);
  myPID.SetOutputLimits(-255, 255);

  analogWrite(in1, 0);
  analogWrite(in2, 0);

  delay(3000);
}

void loop() {   
  encoder = analogRead(pot);  
  //angulo = posEquilibrio - map(encoder, 0, 1023, 0, 300);  // Mapear el valor leído al rango de ángulo deseado  

  //Input = encoder;//angulo;

  /*if (abs(angulo) < 10) {
    myPID.SetTunings(consKp, consKi, consKd);
  } else {
    myPID.SetTunings(aggKp, aggKi, aggKd);
  }*/  

  if (encoder > 650 || encoder < 350) {
    analogWrite(in1, 0);
    analogWrite(in2, 0);    
  } else {
    if (abs(posEquilibrio - encoder) < 1) {
      analogWrite(in1, 0);
      analogWrite(in2, 0);
      Input = posEquilibrio;
      
      myPID.Compute();       
    } else {
      Input = encoder;
      
      myPID.Compute();
      
      if (Output > 0) {
        analogWrite(in1, abs(Output));
        analogWrite(in2, 0);
      } else if (Output < 0) {
        analogWrite(in1, 0);
        analogWrite(in2, abs(Output));      
      } else {
        analogWrite(in1, 0);
        analogWrite(in2, 0);  
      }
    }    
  }  

  Serial.print("Input: ");
  Serial.print(Input);
  Serial.print(", Setpoint: ");
  Serial.print(Setpoint);
  Serial.print(", Output: ");
  Serial.println(Output);

  //Serial.println(angulo1);
}
