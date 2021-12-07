static uint32_t bluetooth_buffer;
static uint16_t line_data;
static uint8_t  current_state;
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

  //setupBLE();
  
  bluetooth_buffer = 0;   //preset buffer value
  current_state    = 0;
  
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
  printBinaryN(line_data, 13);

    switch(current_state){
        case IDLE_STATE:
            drive_stop(0);

            delay(500);
            if(bluetooth_buffer&0b1000){
                current_state = END_STATE;
            }
            else if(bluetooth_buffer&0b100){
                current_state = L_STATE;
            }
            else if(bluetooth_buffer&0b010){
                current_state = F_STATE;
            }
            else if(bluetooth_buffer&0b001){
                current_state = R_STATE;
            }
            else{
                current_state = U_STATE;
            }
            
            break;
        case F_STATE:                        
            while(!detectIntersection(&line_data)){   //continuously look for intersection
                drive_forward(DEFAULT_SPEED, &line_data);

                 if(line_data == 0){
                    current_state = U_STATE;
                    break;
                 }
            }
            drive_stop(0);
        
            if((line_data&0b1111000000000) == 0b1111000000000)
                bluetooth_buffer |= 0b100;
            if((line_data&0b0000000001111) == 0b0000000001111)
                bluetooth_buffer |= 0b001;                 

            if(current_state != U_STATE)
                inch_forward(DEFAULT_SPEED, 160, &line_data, &bluetooth_buffer); //changed from 180 to 160
            
            if(line_data&0b0000111110000)   //front path detected
                bluetooth_buffer |= 0b010;
            if((line_data&0b0000111110000) == 0b0000111110000)   //end point detected
                bluetooth_buffer |= 0b1000;

            if(current_state != U_STATE)
                current_state = IDLE_STATE;     
            break;
         case L_STATE:
            bluetooth_buffer = 0;    //reset buffer value 
            turn_left(TURN_SPEED, &line_data, 45);
            
            current_state = F_STATE;
            break;
         case R_STATE:
            bluetooth_buffer = 0;    //reset buffer value   
            turn_right(TURN_SPEED, &line_data, 45);
            
            current_state = F_STATE;
            break;
         case U_STATE:
            bluetooth_buffer = 0;    //reset buffer value  
            turn_right(TURN_SPEED, &line_data, 165);
        
            current_state = F_STATE;
            break;
         case END_STATE:
            bluetooth_buffer = 0;
            PLAY_SOUND(victory_sound, victory_delay);
            
            while((line_data&0b0000111110000) == 0b0000111110000){
                continue;
            }
            
            current_state = F_STATE;
            break;      
    }
  
  //turn_right(TURN_SPEED, &line_data, 45); //turn at least 45deg before searching for line
  //drive_right(TURN_SPEED);

  //turn_left(TURN_SPEED, &line_data, 45);
  //drive_left(TURN_SPEED);

  //inch_forward(DEFAULT_SPEED, 240, &line_data, &bluetooth_buffer);
  //delay(3000);
  
  //drive_straight(DEFAULT_SPEED);
  //drive_forward(DEFAULT_SPEED, &line_data);
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
