#include <Arduino_LSM9DS1.h>
#include <MadgwickAHRS.h>
#include <MahonyAHRS.h>
//Mahony filter;
Madgwick filter;
unsigned long microsPerReading, microsPrevious;
float accelScale, gyroScale;

float aix_, aiy_, aiz_;
float gix_, giy_, giz_;
float mix_, miy_, miz_;
  
void setup() {
  Serial.begin(115200);

  // start the IMU and filter
//  CurieIMU.begin();
//  CurieIMU.setGyroRate(25);
//  CurieIMU.setAccelerometerRate(25);

if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  
  filter.begin(25);

  // Set the accelerometer range to 2G
//  CurieIMU.setAccelerometerRange(2);
  // Set the gyroscope range to 250 degrees/second
//  CurieIMU.setGyroRange(250);

  // initialize variables to pace updates to correct rate
  microsPerReading = 1000000 / 25;
  microsPrevious = micros();
}

void loop() {
  float aix, aiy, aiz;
  float gix, giy, giz;
  float mix, miy, miz;
  float ax, ay, az;
  float gx, gy, gz;
  float roll, pitch, heading;
  unsigned long microsNow;

  // check if it's time to read data and update the filter
  microsNow = micros();
  if (microsNow - microsPrevious >= microsPerReading) {

    // read raw data from CurieIMU
    if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(aix, aiy, aiz);
    
  } else {
    aix = aix_;
    aiy = aiy_;
    aiz = aiz_;
  }
  
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gix, giy, giz);
    
  } else {
    gix = gix_;
    giy = giy_;
    giz = giz_;
  }

  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(mix, miy, miz);
    
  } else {
    mix = mix_;
    miy = miy_;
    miz = miz_;
  }

  
  
//    CurieIMU.readMotionSensor(aix, aiy, aiz, gix, giy, giz);
//    aix /= 2;
//    aiy /= 2;
//    aiz /= 2;
//
//    gix /= 8;
//    giy /= 8;
//    giz /= 8;
    // convert from raw data to gravity and degrees/second units
    float a_norm = sqrt(pow(aix,2)+pow(aiy,2)+pow(aiz,2));

    ax = aix/ a_norm;;//convertRawAcceleration(aix);
    ay = aiy/ a_norm;;//convertRawAcceleration(aiy);
    az = aiz/ a_norm;;//convertRawAcceleration(aiz);

    
    
    gx = gix;//convertRawGyro(gix);
    gy = giy;//convertRawGyro(giy);
    gz = giz;//convertRawGyro(giz);

    // update the filter, which computes orientation
    filter.updateIMU(gx, gy, gz, ax, ay, az);//, mix,miy,miz);

    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();
    Serial.print("Orientation: ");
    Serial.print(heading);
    Serial.print(" ");
    Serial.print(pitch);
    Serial.print(" ");
    Serial.println(roll);

    // increment previous time, so we keep proper pace
    microsPrevious = microsPrevious + microsPerReading;

    aix_ = aix;
    aiy_ = aiy;
    aiz_ = aiz;

    mix_ = mix;
    miy_ = miy;
    miz_ = miz;

    mix_ = mix;
    miy_ = miy;
    miz_ = miz;
  }
  delay(25);
}

float convertRawAcceleration(int aRaw) {
  // since we are using 2G range
  // -2g maps to a raw value of -32768
  // +2g maps to a raw value of 32767
  
  float a = (aRaw * 2.0) / 32768.0;
  return a;
}

float convertRawGyro(int gRaw) {
  // since we are using 250 degrees/seconds range
  // -250 maps to a raw value of -32768
  // +250 maps to a raw value of 32767
  
  float g = (gRaw * 250.0) / 32768.0;
  return g;
}
