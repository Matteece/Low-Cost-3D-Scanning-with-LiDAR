#define STEPPER_PIN_1 8
#define STEPPER_PIN_2 9
#define STEPPER_PIN_3 10
#define STEPPER_PIN_4 11

const int stepsPerRev = 2048; // Number of steps in full step mode: 2048
int button = 7;
int button2 = 6;
int step_number = 0;
int stepCount = 0;
bool flag = false;
bool step_dir = true; // Button/stepper's direct direction reference

void setup() {
    Serial.begin(115200);
  pinMode(button, INPUT);
  pinMode(button2, INPUT);
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);

}

void loop() {
  if (digitalRead(button) == HIGH) {
    // Toggle Flag
    if (flag) {
      flag = false;
      Serial.println(stepCount);
    } else {
//      Serial.print("On\n");
      flag = true; 
      // Need to get servo into first layer position
    }
    while(digitalRead(button) == HIGH);
  }

// Only allow siwtching direction if the system isn't running
  if (digitalRead(button2) == HIGH) {
    // Toggle Flag
    step_dir = !step_dir;
    while(digitalRead(button2) == HIGH);
  }

  if (flag) {
    runSystem();
  }

}

void runSystem() {
  OneStep(step_dir);
  delay(20);

   // Increment step size taken in degrees
  if (step_dir) {
    stepCount++;
  } else {
    stepCount--;
  }

  // Change direction of stepper so it doesn't tangle the wires. Also jump to the next layer by angling the servo to the next degree
  if (stepCount >= stepsPerRev) {
    step_dir = !step_dir;
  } else if (stepCount <= 0) {
    step_dir = !step_dir;
  }
}

void OneStep(bool dir){
  //Written By Nikodem Bartnik - nikodembartnik.pl
    if(dir){
switch(step_number){
  case 0:
  digitalWrite(STEPPER_PIN_1, HIGH);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, LOW);
  break;
  case 1:
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, HIGH);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, LOW);
  break;
  case 2:
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, HIGH);
  digitalWrite(STEPPER_PIN_4, LOW);
  break;
  case 3:
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, HIGH);
  break;
} 
  }else{
    switch(step_number){
  case 0:
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, HIGH);
  break;
  case 1:
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, HIGH);
  digitalWrite(STEPPER_PIN_4, LOW);
  break;
  case 2:
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, HIGH);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, LOW);
  break;
  case 3:
  digitalWrite(STEPPER_PIN_1, HIGH);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, LOW);
 
  
} 
  }
step_number++;
  if(step_number > 3){
    step_number = 0;
  }
}
