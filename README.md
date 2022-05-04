# Low-Cost-3D-Scanning-with-LiDAR
This project utilizes a Garmin LiDAR Lite v4 LED as well as an Arduino Uno Starter kit to take 3D scans of rooms.  
The Arduino runs the hardware and collects data including the distance measured, the rotation angle, and sensor angle, 
then sends this data into a MATLAB script running on the computer it's connected to in order to produce a 3D point cloud of the room being scanned.

How To Set Up Scanner:
Primary button: left (pin 7)
Secondary Button: right (pin 6)

Included with this repository are additional Arduino programs including sensor_calibration.ino and stepper_calibration.ino.  
These programs are to be used to set up the room scanner to scan just a section of a room, instead of the complete room.  The first one, sensor_calibration.ino, 
will start taking distance measurements once the system is activated.  Then, the user can press the secondary button to start angling the sensor up one degree at at time
until it reaches the precise angle desired to scan a section of a room.  Once this angle is reached, pressing the primary button again will stop the system from taking
distance measurements and spit out the angle reached by the servo.

The other program, stepper_calibration.ino, does a similar thing as sensor_calibration.ino, except this program just turns the stepper in order to see how many steps it
will need to take in order to cover the desired area.  Upon pushing the primary button, the stepper will begin to turn counter-clockwise.  Pressing the secondary button
will reverse the direction of rotation.  Once the desired angle is reached, pressing the primary button will stop rotating the stepper and spit out the amount of steps
needed to reach that angle from wherever it started.
