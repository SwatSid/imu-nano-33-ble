////////////////////////////////////////////////////////////////////////////
//
//  This file is part of RTIMULib-Arduino
//
//  Copyright (c) 2014-2015, richards-tech
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy of 
//  this software and associated documentation files (the "Software"), to deal in 
//  the Software without restriction, including without limitation the rights to use, 
//  copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the 
//  Software, and to permit persons to whom the Software is furnished to do so, 
//  subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all 
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
//  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
//  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

//#include <Wire.h>
//#include "I2Cdev.h"
//#include "RTIMUSettings.h"
//#include "RTIMU.h"
#include "CalLib.h"
#include <EEPROM.h>

#include <Arduino_LSM9DS1.h>

//RTIMU *imu;                                           // the IMU object
//RTIMUSettings settings;                               // the settings object
CALLIB_DATA calData;                                  // the calibration data

//  SERIAL_PORT_SPEED defines the speed to use for the debug serial port

#define  SERIAL_PORT_SPEED  115200

void setup()
{
  calLibRead(0, &calData);                           // pick up existing mag data if there   

if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  
  calData.magValid = false;
  for (int i = 0; i < 3; i++) {
    calData.magMin[i] = 10000000;                    // init mag cal data
    calData.magMax[i] = -10000000;
  }
   
  Serial.begin(SERIAL_PORT_SPEED);
  Serial.println("ArduinoMagCal starting");
  Serial.println("Enter s to save current data to EEPROM");
//  Wire.begin();
   
//  imu = RTIMU::createIMU(&settings);                 // create the imu object
//  imu->IMUInit();
//  imu->setCalibrationMode(true);                     // make sure we get raw data
  Serial.print("ArduinoIMU calibrating device "); 
//  Serial.println(imu->IMUName());
}

void loop()
{  
  boolean changed;
//  RTVector3 mag;
  float mix, miy, miz;
//  if (IMU.magneticFieldAvailable()) {
//    IMU.readMagneticField(mix, miy, miz);
//    
//  }
//  int [3] mag_ = {mix,miy,miz};
  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(mix, miy, miz);
    float mag_[3] = {mix,miy,miz};
    changed = false;
//    mag = imu->getCompass();
    for (int i = 0; i < 3; i++) {
      if (mag_[i] < calData.magMin[i]) {
        calData.magMin[i] = mag_[i];
        changed = true;
      }
      if (mag_[i] > calData.magMax[i]) {
        calData.magMax[i] = mag_[i];
        changed = true;
      }
    }
//    Serial.println(mix,miy,miz);
    if (changed) {
      Serial.println("-------");
      Serial.print("minX: "); Serial.print(calData.magMin[0]);
      Serial.print(" maxX: "); Serial.print(calData.magMax[0]); Serial.println();
      Serial.print("minY: "); Serial.print(calData.magMin[1]);
      Serial.print(" maxY: "); Serial.print(calData.magMax[1]); Serial.println();
      Serial.print("minZ: "); Serial.print(calData.magMin[2]);
      Serial.print(" maxZ: "); Serial.print(calData.magMax[2]); Serial.println();
    }
  }
  
  if (Serial.available()) {
    if (Serial.read() == 's') {                  // save the data
      calData.magValid = true;
      calLibWrite(0, &calData);
      Serial.print("Mag cal data saved for device "); 
//      Serial.println(imu->IMUName());
    }
  }
}
