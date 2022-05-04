#include <Servo.h>
#include "LIDARLite_v4LED.h"

Servo servo;
int button = 7;
int button2 = 6;
bool flag = false;
bool step_dir = false;
int count = 180;
LIDARLite_v4LED myLIDAR;  // Pin A5 is the clock (SDC) and pin A4 is data (SDA)
// Purple Wire: Power (5v)
// Yellow Wire: Ground
// Green Wire: SDC Clock to pin A5 (3.3v)
// Gray Wire: SDA Data to pin A4

void setup() {
  // put your setup code here, to run once:
  servo.attach(4);
  Serial.begin(115200);
  pinMode(button, INPUT);
  pinMode(button2, INPUT);
  Wire.begin(); //Join I2C bus
  Serial.println("I2C Bus joined");

  //check if LIDAR will acknowledge over I2C
  if (myLIDAR.begin() == false) {
    Serial.println("Device did not acknowledge! Freezing.");
    while(1);
  }
  Serial.println("LIDAR acknowledged!");
}

void loop() {
  if (digitalRead(button) == HIGH) {
    // Toggle Flag
    if (flag) {
      flag = false;
      Serial.print("Off\n");
      Serial.print("Servo Angle: ");
      Serial.println(count);
    } else {
      Serial.print("On\n");
      flag = true; 
    }
    while(digitalRead(button) == HIGH);
  }

  if (digitalRead(button2) == HIGH) {
    // Advance servo angle by 1
    count--;
//    while(digitalRead(button2) == HIGH);
  }

  if (flag){
    servo.write(count);
    float dist = myLIDAR.getDistance();
    Serial.println(dist);
    delay(20);
  }
}
