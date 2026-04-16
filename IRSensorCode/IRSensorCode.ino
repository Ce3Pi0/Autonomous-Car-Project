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

const float leftWeights[NUM_SENSORS / 2]  = {-(DIST + SENS_W * 4), -(DIST + SENS_W * 2), -DIST};
const float rightWeights[NUM_SENSORS / 2] = {(DIST + SENS_W * 4), (DIST + SENS_W * 2), DIST};

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


  const int leftSensValues[NUM_SENSORS / 2] = {sensl3, sensl2, sensl1};
  const int rightSensValues[NUM_SENSORS / 2] = {sensr3, sensr2, sensr1};

  const int sensValues[NUM_SENSORS] = {sensl3, sensl2, sensl1, sensr1, sensr2, sensr3};

  // for (int i = 0; i < NUM_SENSORS; i++) Serial.print(sensValues[i]);
  // Serial.println();

  if (isValid(sensValues)) {
    float left = sumLeft(leftSensValues);
    float right = sumRight(rightSensValues);

    Serial.println(sumWeights(left, right));
  } else {
    Serial.println("STOP");
  } 
}

bool isValid (int sensValues[]) {
  int count = 0;

  for (int i = 0; i < NUM_SENSORS; i++) {
    if (sensValues[i] == -1) count ++;
  }

  if (count > 2) return false;

  for (int i = 0; i < NUM_SENSORS; i++) {
    if (sensValues[i] == -1 && count == 2) {
      if (i == 0) return sensValues[i + 1] == -1;
      else if (i == NUM_SENSORS - 1) return sensValues[i - 1] == -1;

      return sensValues[i + 1] == -1;
    }
  }

  return true;
}

float sumPair(float a, float b) {
  if (a != 0 && b != 0) {
    return a > 0? a - DIST : a + DIST;
  }

  return a == 0? b : a;
}

float sumRight (int sensValues[]) {
  int count = 0;

  for (int i = 0; i < NUM_SENSORS / 2; i++) {
    if (sensValues[i] != 0) count += 1;
  }

  if (count == 1) {
    for (int i = 0; i < NUM_SENSORS / 2; i++) {
      if (sensValues[i] != 0) return sensValues[i] * rightWeights[i];
    }
  } else if (count == 2) {
    for (int i = 0; i < NUM_SENSORS / 2; i++) {
      if (sensValues[i] != 0) return sensValues[i] * rightWeights[i] + DIST;
    }
  }

  return 0.0;
}

float sumLeft (int sensValues[]) {
  int count = 0;

  for (int i = 0; i < NUM_SENSORS / 2; i++) {
    if (sensValues[i] != 0) count += 1;
  }

  if (count == 1) {
    for (int i = 0; i < NUM_SENSORS / 2; i++) {
      if (sensValues[i] != 0) return sensValues[i] * leftWeights[i];
    }
  } else if (count == 2) {
    for (int i = 0; i < NUM_SENSORS / 2; i++) {
      if (sensValues[i] != 0) return sensValues[i] * leftWeights[i] - DIST;
    }
  }

  return 0.0;
}

float sumWeights (float left, float right) {
  return left + right;
}
