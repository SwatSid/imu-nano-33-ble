/*

   Test Program to find Roll and Pitch from just Accelerometer values from the onboard LSM IMU of Arduino Nano 33 BLE

*/

// Added Git stuff

#include <Arduino_LSM9DS1.h>

float x, y, z;
float roll, pitch;

void setup() {
  Serial.begin(9600);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

//  Serial.print("Accelerometer sample rate = ");
//  Serial.print(IMU.accelerationSampleRate());
//  Serial.println(" Hz");
//  Serial.println();
}

void loop() {

if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

//    Serial.print(x);
//    Serial.print('\t');
//    Serial.print(y);
//    Serial.print('\t');
//    Serial.println(z);

  roll = atan2(x,z) * 180/PI;
  pitch = atan2(y,z) * 180/PI;

  Serial.print(roll);
  Serial.print('\t');
  Serial.println(pitch);
  }
}
