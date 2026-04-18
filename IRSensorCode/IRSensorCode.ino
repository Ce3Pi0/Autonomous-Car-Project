#include <Servo.h>
#include "IrSensorArray.hpp"

Servo servoMotor;

// Servo PIN
const unsigned int SERVO_PIN = A0;

// Servo range: 20 - 160
const unsigned int SERVO_STRAIGHT = 90;
const unsigned int SERVO_LEFT = 160;
const unsigned int  SERVO_RIGHT = 20;

// IR Sensors
const unsigned int IR_L_1 = A4;
const unsigned int IR_L_2 = A5;
const unsigned int IR_L_3 = A7;

const unsigned int IR_R_1 = A3;
const unsigned int IR_R_2 = A2;
const unsigned int IR_R_3 = A1;

const float MAX_ERROR = 5.72;

// H-Bridge
const unsigned int IN_4 = 8;
const unsigned int IN_3 = 9;

// 10 & 11 can't be used with Servo.h
const unsigned int ENA34 = 6;

//RANGE: 0 - 255
const unsigned int MAX_MOTOR_SPEED = 150; // Adjust for real world usage

// PID
const float Kp = 12.0;
const float Ki = 0.1;
const float Kd = 0.8;

float oldD = 0.0;
float prevError = 0.0;
float integral = 0.0;
unsigned long prevTime = 0;

int normalizeIrReading(int);
void printValues(int*, int); 
void doMotorDrive(int);

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

  if (irSensorArray.isValid()) {
    doMotorDrive(MAX_MOTOR_SPEED);

    const float error = irSensorArray.sumWeights();

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
    //Serial.println("STOP");
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


void doMotorDrive(int motorSpeed) {
  analogWrite(ENA34, motorSpeed);
}
