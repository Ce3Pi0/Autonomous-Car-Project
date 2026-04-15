const unsigned int IR_L_1 = 10;
const unsigned int IR_L_2 = 9;
const unsigned int IR_L_3 = 8;

const unsigned int IR_R_1 = 11;
const unsigned int IR_R_2 = 12;
const unsigned int IR_R_3 = 13;

const float DIST = 1.27 / 2; 
const float SENS_W = 1.27;

const unsigned int NUM_SENSORS = 6;

const float weights[NUM_SENSORS]= {-(DIST + SENS_W * 4), -(DIST + SENS_W * 2), -DIST, DIST, DIST + SENS_W * 2, DIST + SENS_W * 4};

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
  const int sensl1 = digitalRead(IR_L_1) - 1;
  const int sensl2 = digitalRead(IR_L_2) - 1;
  const int sensl3 = digitalRead(IR_L_3) - 1;
  const int sensr1 = digitalRead(IR_R_1) - 1;
  const int sensr2 = digitalRead(IR_R_2) - 1;
  const int sensr3 = digitalRead(IR_R_3) - 1;

  const int sensValues[NUM_SENSORS] = {sensl3, sensl2, sensl1, sensr1, sensr2, sensr3};

  // Serial.print(sensl3);
  // Serial.print(sensl2);
  // Serial.print(sensl1);
  // Serial.print(sensr1);
  // Serial.print(sensr2);
  // Serial.print(sensr3);

  Serial.println();
  Serial.println(sumWeights(sensValues));
}

float sumRight (int rightValues[]) {}

float sumLeft (int leftValues[]) {}

float sumWeights (int sensValues[]) {
  float acc = 0;
  for (int i = 0; i < NUM_SENSORS; i++) {
    acc += weights[i] * sensValues[i];
  }

  return acc;
}
