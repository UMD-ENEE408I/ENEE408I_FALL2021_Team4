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

double x;
float bias = 0; 
float gx, gy, gz;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  float counter=0;
  float reg;
  while(1){
    if (IMU.gyroscopeAvailable()) {
    
    IMU.readGyroscope(gx, gy, gz);

    if (counter < 100)
    {
      counter++;
      continue;
    }
    counter++;
    reg = gx+reg;
    }

    if (counter==1000)
    {
      break;
    }
 
  }
  bias = reg  / (counter-100);
  //Serial.println(bias);
  
}


// set up the gyro, did a 90 degree turn, everytime it gave them 80 degrees
// quick fix is to multiply everything by 9/8s
// past 5 samples initialize so we aren't drawing from negative time


float dt = 0;
float current_angle=0;
float delta_angle=0;
float current_time;
float prev_time = 0;
void loop() {
//while 
  if (IMU.gyroscopeAvailable()) { // might be some minimum polling time, the gyroscope might lose availability after taking a sample
    
    IMU.readGyroscope(gx, gy, gz);


  
  current_time = (float) micros()*(1.0/1000000.0);
  dt = current_time - prev_time;
  prev_time = current_time;
  delta_angle= (gz-bias)*dt*9.0/8.0;
  current_angle = current_angle+delta_angle;
       
  
  Serial.print("Gyr: ");
  Serial.print(gz-bias);
  Serial.println('\t');

  Serial.print("AngleX= ");
  Serial.println(current_angle);
  
  // positive values = clockwise
  
  Serial.println("-----------------------------------------");
  /*Serial.print("Mag: ");
  Serial.print(mx);
  Serial.print('\t');
  Serial.print(my);
  Serial.print('\t');
  Serial.println(mz);*/
  }
  
}
