#include "IrSensorArray.hpp"

const unsigned int IR_L_1 = A4;
const unsigned int IR_L_2 = A5;
const unsigned int IR_L_3 = A7;

const unsigned int IR_R_1 = A3;
const unsigned int IR_R_2 = A2;
const unsigned int IR_R_3 = A1;


int normalizeIrReading(int);
void printValues(int*, int); 

void setup() {
  Serial.begin(9600);
  
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

  printValues(irSensorArray.getValues(), irSensorArray._NUM_SENSORS);

  if (irSensorArray.isValid()) {
    const float weightSum = irSensorArray.sumWeights();
    Serial.println(weightSum);
  } else {
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