#include <LIDARLite_v4LED.h>
#include <Servo.h>

// The amount of steps in one full revolution of the stepper
const int stepsPerRev = 2048; // Number of steps for a full circle in full step mode: 2048
const int servoMinAngle = 159; // Servo value, minimum angle is 159 to prevent scanning the tower
const int servoMaxAngle = 0;  // 0 Degrees is straight up (max).  34 is a nice "open lid" angle that keeps the top off the scan, like an open jar.
int button = 7;
int button2 = 6;
int stepCount = 0;
int step_number = 0;
int theta; 
int i2cResetCount; // Every 10000 points collected, wait 20ms before pinging the next distance to "cool down" I2C bus
double stepSizeinDegrees = 0.17578125; // 360/2048 since there are 2048 total steps in 360 degrees.  0.17578125 is 1 step of the stepper in degrees.
double phi = 0.17578125;
bool flag = false;
bool servoDirection = true;
bool step_dir = true; // Button/stepper's direct direction reference, true is counter-clockwise
bool stepCountDirection = true; // Function's reference for automatically switching direction, true is counter-clockwise
Servo servo;
LIDARLite_v4LED myLIDAR;  // Pin A5 is the clock (SDC) and pin A4 is data (SDA)
// Wiring:
// Purple Wire: Power (5v)
// Yellow Wire: Ground
// Green Wire: SDC Clock to pin A5 (3.3v)
// Gray Wire: SDA Data to pin A4 (3.3v)
// From Sensor to Ribbon:
// Purple to red
// Yellow to Green
// Green to Yellow
// Gray to Orange

#define STEPPER_PIN_1 8
#define STEPPER_PIN_2 9
#define STEPPER_PIN_3 10
#define STEPPER_PIN_4 11

// Wiring:
// Pin 8 to IN1 on the ULN2003 driver
// Pin 9 to IN2 on the ULN2003 driver
// Pin 10 to IN3 on the ULN2003 driver
// Pin 11 to IN4 on the ULN2003 driver

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  pinMode(button, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);
  servo.attach(4);
  theta = servoMinAngle;

//  Wire.setClock(400000);
  Wire.begin(); //Join I2C bus

  //check if LIDAR will acknowledge over I2C
  if (myLIDAR.begin() == false) {
    Serial.println("Device did not acknowledge! Freezing.");
    while(1);
  }
//  Serial.println("LIDAR acknowledged!");
TWBR = ((F_CPU / 400000UL) - 16) / 2; // Set I2C frequency to 400kHz
//Wire.setClock(400000UL);
}

void loop() {

  if (digitalRead(button) == HIGH) {
    // Toggle Flag
    if (flag) {
      flag = false;
      Serial.println("9847");
      Serial.println("9847");
      Serial.println("9847");
      Serial.println("9847");
      Serial.println("9847");
    } else {
//      Serial.print("On\n");
      flag = true; 
      // Need to get servo into first layer position
      servo.write(theta);
      // Reset I2C reset counter
      i2cResetCount = 0;
    }
    while(digitalRead(button) == HIGH);
  }

// Only allow siwtching direction if the system isn't running
  if (digitalRead(button2) == HIGH && !flag) {
    // Toggle stepper direction
    if (step_dir){
      step_dir = false;
      // Make the system start by counting down if the stepper starts rotating clockwise.  This will prevent the sensor from producing a mirror image.
      // Initialize phi so that it starts at the maximum angle specified by the user
      phi = stepsPerRev*stepSizeinDegrees;
    } else {
      step_dir = true;
    }
    while(digitalRead(button2) == HIGH);
  }

  if (flag) {
    runSystem();
  }
}

void runSystem() {
  //HOW THE SYSTEM WORKS: It scans a room in layers.  First it will angle the sensor all the way down, then it will take a 360 degree scan, and repeat for every layer.
  
  // Run sensor:
  int dist = myLIDAR.getDistance();
  i2cResetCount++;
  if (i2cResetCount == 10000) {
    delay(20);
    i2cResetCount = 0; 
  }
//  Serial.print("Distance: ");
// These are the polar coordinates:
  Serial.println(dist); // radius
  Serial.println(phi); // Phi

  // Theta is reversed from how we want it.  Thus, it needs to be flipped
//  theta = theta - 180;
//  // If theta goes negative, get its absolute value
//  if (theta < 0) {
//    theta *= -1;
//  }
  Serial.println(theta); // Theta
  
  // Run stepper:
  // Step one revolution in one direction:
  OneStep(step_dir);

  // Increment step size taken in degrees
  if (step_dir) {
    phi += stepSizeinDegrees;
  } else {
    phi -= stepSizeinDegrees;
  }

  // Increment the number of steps taken
  if (stepCountDirection){
    stepCount++;
  } else{
    stepCount--;
  }

  // Change direction of stepper so it doesn't tangle the wires. Also jump to the next layer by angling the servo to the next degree
  if (stepCount >= stepsPerRev) {
    step_dir = !step_dir;
    stepCountDirection = !stepCountDirection;
    incrementLayer();
  } else if (stepCount <= 0) {
    step_dir = !step_dir;
    stepCountDirection = !stepCountDirection;
    incrementLayer();
  }
//  incrementLayer();
  
}

// Function that handles running the servo
void incrementLayer() {
  // Angle the servo
  if (!servoDirection) {
    theta++;
  } else {
    theta--;
  }
  
  // Change direction of servo.
  if (theta >= servoMinAngle || theta <= servoMaxAngle) {
    servoDirection = !servoDirection;
  }
  servo.write(theta);
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
