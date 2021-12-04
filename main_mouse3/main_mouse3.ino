static uint32_t bluetooth_buffer;
static uint16_t line_data;
static uint8_t  current_state;
static uint8_t  turn_flag;            //=1 whenever a turn is made. will be turned off in t2_func()
static int      velocity_correction;

#include <ArduinoBLE.h>

#include "mouse_sensor.h"         //includes functions for line sensor
#include "mouse_movement.h"       //includes function for mouse movement
#include "mouse_sound.h"          //includes all audio functions and tones
#include "main_definitions.h"     //RTOS and other stuff
#include "mouse_bluetooth.h"

#include "template_functions.h"   //OPTIONAL. includes functions to print out binary

#define DEFAULT_SPEED   36  //speed of the car
#define TURN_SPEED      36
#define MAX_CORRECTION  12  //max correction value for velocity
char str_buf[128];


void setup() {
  Serial.begin(115200);

  setupBLE();
  
  bluetooth_buffer = 0;   //preset buffer value
  current_state    = 0;
  turn_flag       = 0;   //initially use right wheel as constant velocity
  
  //PID control
  setupPID();  
  
  //reflective sensors
  adc1.begin(ADC_1_CS);  
  adc2.begin(ADC_2_CS);

  //wheels motor
  pinMode(L_Motor_1, OUTPUT);
  pinMode(L_Motor_2, OUTPUT);
  pinMode(R_Motor_1, OUTPUT);
  pinMode(R_Motor_2, OUTPUT);

  //encoder
  enc1.write(0);  //reset encoder to 0
  enc2.write(0);  //reset encoder to 0

  //Gyro
  setupGyro();

  //Initiate sensor
  PLAY_SOUND(calibrate_begin_sound, calibrate_begin_delay);
  calibrate_adc(adc_threshhold); 
  PLAY_SOUND(calibrate_end_sound, calibrate_end_delay);
  
  //Initiate rtos functions
  t1.start(t1_func);
}



void loop() {
  //read_line(&line_data);
  //printBinaryN(line_data, 13);
  
  //turn_right(TURN_SPEED, &line_data, 45); //turn at least 45deg before searching for line
  //drive_right(TURN_SPEED);

  //turn_left(TURN_SPEED, &line_data, 45);
  //drive_left(TURN_SPEED);

  //inch_forward(DEFAULT_SPEED, 240, &line_data, &bluetooth_buffer);
  //delay(3000);
  
  //drive_straight(DEFAULT_SPEED);
  drive_forward(DEFAULT_SPEED, &line_data);
}

//THREAD1: Continuosly updates the line_data
void t1_func(){
    while(1){
        read_line(&line_data);
    }
}





/*~~~NOTES~~~
    Centered line value = 13'B0000001000000   //center point
    Centered line value = 13'B0000011100000   //center wide range
    
    Centered line value = 13'B1111000000000   //left point
*/
