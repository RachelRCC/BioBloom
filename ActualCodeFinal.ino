#include <MQ135.h>

bool buttonState;
const int ANALOGPIN=0;
const int MAX_POS = 1750;
int step_counter = MAX_POS;
int AQ_min = 600;
int AQ_max = 700;
MQ135 gasSensor = MQ135(A0);
const int PIN_RED   = 2;
const int PIN_GREEN = 3;
const int PIN_BLUE  = 4;
double scaled_conc;

void setup(){
  Serial.begin(9600);      // sets the serial port to 9600
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(A5, INPUT);
  pinMode(A0, INPUT);

  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);

}

void loop() {
  buttonState = digitalRead(A5);
  float aq = analogRead(A0);
  Serial.println (aq);

  scaled_conc = map(aq, 0, 255, 0, MAX_POS);
  setColor(scaled_conc, 255 - scaled_conc, 0);

  // adjust max and min values for room conditions
  if(aq < AQ_min) {
    AQ_min = aq;
  }
  else if(aq > AQ_max) {
    AQ_max = aq;
  }
  float movement = map(aq, AQ_min, AQ_max, 0, MAX_POS);
  Serial.println (movement);
  moveStepper(movement);
  delay(5000);
}

// 200 steps raises bolt 8mm
// total movement available is 70mm(1750 steps)
void moveStepper(int new_pos) {
  if(step_counter < new_pos) {
    digitalWrite(12, HIGH); // set positive direction
    while(step_counter < new_pos) { // rotates screw up until it reaches desired position
      digitalWrite(13, HIGH);
      delay(10);
      digitalWrite(13, LOW);
      delay(10);
      step_counter++;
    }
  }
  else {
    digitalWrite(12, LOW); // set negative direction
    while(step_counter > new_pos) { // rotates screw down until it reaches desired position
      digitalWrite(13, HIGH);
      delay(10);
      digitalWrite(13, LOW);
      delay(10);
      step_counter--;
    }
  }
}

void resetStepper() {
  digitalWrite(12, HIGH); // set positive direction
  while(step_counter < MAX_POS) {
    digitalWrite(13, HIGH);
    delay(10);
    digitalWrite(13, LOW);
    delay(10);
    step_counter++;
  }
}

void setColor(int R, int G, int B) {
  analogWrite(PIN_RED,   R);
  analogWrite(PIN_GREEN, G);
  analogWrite(PIN_BLUE,  B);
}
