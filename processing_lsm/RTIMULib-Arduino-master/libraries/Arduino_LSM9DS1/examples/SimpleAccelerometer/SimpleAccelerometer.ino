/*
  Arduino LSM9DS1 - Simple Accelerometer

  This example reads the acceleration values from the LSM9DS1
  sensor and continuously prints them to the Serial Monitor
  or Serial Plotter.

  The circuit:
  - Arduino Nano 33 BLE Sense

  created 10 Jul 2019
  by Riccardo Rizzo

  This example code is in the public domain.
*/

#include <Arduino_LSM9DS1.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);
  //Serial.println("Started");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

//  Serial.print("Accelerometer sample rate = ");
//  Serial.print(IMU.accelerationSampleRate());
//  Serial.println(" Hz");
//  Serial.println();
//  Serial.println("Acceleration in G's");
//  Serial.println("X\tY\tZ");
}

void loop() {
  float aix, aiy, aiz, gix, giy ,giz;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(aix, aiy, aiz);
    IMU.readGyroscope(gix, giy, giz);

    aix = map(aix,-4,4,-2,2);
    aiy = map(aiy,-4,4,-2,2);
    aiz = map(aiz,-4,4,-2,2);

    gix = map(aix,-2000,2000,-250,250);
    giy = map(giy,-2000,2000,-250,250);
    giz = map(giz,-2000,2000,-250,250);
    Serial.print(aix);
    Serial.print(',');
    Serial.print(aiy);
    Serial.print(',');
    Serial.print(aiz);
    Serial.print(" ");

    Serial.print(gix);
    Serial.print(',');
    Serial.print(giy);
    Serial.print(',');
    Serial.println(giz);
  }
}
