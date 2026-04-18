#include <Pixy2.h>
#include <Servo.h>
#include "IrSensorArray.hpp"

// Servo PIN
#define SERVO_PIN A0

// IR Sensors
#define IR_L_1 A4
#define IR_L_2 A5
#define IR_L_3 A7

#define IR_R_1 A3
#define IR_R_2 A2
#define IR_R_3 A1

// H-Bridge
#define IN_4 8
#define IN_3 9

#define ENA34 6

// Servo range: 20 - 160
#define SERVO_STRAIGHT 90
#define SERVO_LEFT 160
#define  SERVO_RIGHT 20

#define MAX_ERROR 5.72

//RANGE: 0 - 255
#define MAX_MOTOR_SPEED 150 // Adjust for real world usage

// PID
#define Kp 12.0
#define Ki 0.1
#define Kd 0.8

// Pixy
#define GREEN 1
#define YELLOW 2
#define RED 3

Pixy2 pixy;
Servo servoMotor;

float oldD = 0.0;
float prevError = 0.0;
float integral = 0.0;
unsigned long prevTime = 0;

int normalizeIrReading(int);
void printValues(int*, int); 
bool trafficLightCheck();
void doMotorDrive(int, float);

void setup() {
  Serial.begin(9600);

  pinMode(IN_4, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(ENA34, OUTPUT);
    
  // Test motor direction for the car (Switch low and high for opposite direction)
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);

  servoMotor.attach(SERVO_PIN);

  pinMode(IR_L_1, INPUT);
  pinMode(IR_L_2, INPUT);
  pinMode(IR_L_3, INPUT);

  pinMode(IR_R_1, INPUT);
  pinMode(IR_R_2, INPUT);
  pinMode(IR_R_3, INPUT);

  pixy.init();
}

void loop() {  
  const int sensL1 = normalizeIrReading(analogRead(IR_L_1));
  const int sensL2 = normalizeIrReading(analogRead(IR_L_2));
  const int sensL3 = normalizeIrReading(analogRead(IR_L_3));
  const int sensR1 = normalizeIrReading(analogRead(IR_R_1));
  const int sensR2 = normalizeIrReading(analogRead(IR_R_2));
  const int sensR3 = normalizeIrReading(analogRead(IR_R_3));

  const int irSensorArrayReadings[] = {sensL3, sensL2, sensL1, sensR1, sensR2, sensR3}; 

  IrSensorArray irSensorArray(irSensorArrayReadings);

  // printValues(irSensorArray.getValues(), irSensorArray._NUM_SENSORS);

  if (trafficLightCheck() && irSensorArray.isValid()) {
    const float error = irSensorArray.sumWeights();
    
    doMotorDrive(MAX_MOTOR_SPEED, error);

    unsigned long now = millis();
    float dt = (now - prevTime) / 1000.0;
    if (dt <= 0.0) dt = 0.001;

    const float P = error;
    integral += error * dt;
    const float rawD = (error - prevError) / dt;
    const float D = 0.8 * oldD + 0.2 * rawD;

    integral = constrain(integral, -10.0, 10.0);

    const float correction = Kp * P+  Kd * D + Ki * integral;
    int servoAngle = SERVO_STRAIGHT + (int)correction;
    servoAngle = constrain(servoAngle, SERVO_RIGHT, SERVO_LEFT);

    Serial.println(servoAngle);

    servoMotor.write(servoAngle);

    oldD = D;
    prevError = error;
    prevTime = now;
  } else {
    doMotorDrive(0);
    servoMotor.write(SERVO_STRAIGHT);
    Serial.println("STOP");
  } 
}

int normalizeIrReading(int reading) {
  return map(reading, 0, 1024, 0, 2);
}

void printValues(int* values, int size) {
  for (int i = 0; i < size; i++) {
    Serial.print(values[i]);
    Serial.print(" ");
  }
  Serial.println();
}


void doMotorDrive(int motorSpeed, float error) {
  if (error == 0)
    analogWrite(ENA34, motorSpeed);
  else 
    analogWrite(ENA34, motorSpeed / 2);

}

bool trafficLightCheck() {
  pixy.ccc.getBlocks();

  for (int i = 0; i < pixy.ccc.numBlocks; i++) {
    switch (pixy.ccc.blocks[i].m_signature) {
      case GREEN:
        Serial.println("Green light detected ");
        return true;
      case YELLOW:
        Serial.println("Yellow light detected ");
        return false;
      case RED:
        Serial.println("Red light detected ");
        return false;
      default:
        return true;
    }
 }

 return false;
}
