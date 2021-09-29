#include "mouse_sensor.h"         //includes functions for line sensor
#include "mouse_movement.h"       //includes function for mouse movement
#include "mouse_sound.h"          //includes all audio functions and tones

#include "template_functions.h"   //OPTIONAL. includes functions to print out binary

#define DEFAULT_SPEED 32  //speed of the car
#define TURN_SPEED    32

void setup() {
  Serial.begin(115200);

  //reflective sensors
  adc1.begin(ADC_1_CS);  
  adc2.begin(ADC_2_CS);

  //wheels motor
  pinMode(L_Motor_1, OUTPUT);
  pinMode(L_Motor_2, OUTPUT);
  pinMode(R_Motor_1, OUTPUT);
  pinMode(R_Motor_2, OUTPUT);

  PLAY_SOUND(calibrate_begin_sound, calibrate_begin_delay);
  calibrate_adc(adc_threshhold);
  PLAY_SOUND(calibrate_end_sound, calibrate_end_delay);
}

void loop() {
  uint16_t line_data;
  
  read_line(&line_data);
  printBinaryN(line_data, 13);

  if(((line_data&0b1111100000000) == 0b1111100000000) && ((line_data&0b0000000000111) == 0b0000000000000)){      //if car sees left intersection 
          //tone(BUZZ_PIN, NOTE_AS2, 500);      //remove later
          drive_stop(DEFAULT_SPEED);
          while((line_data&0b0000000110000) != 0b0000000110000){         //keep turning left until center sensor detects line
              tone(BUZZ_PIN, NOTE_AS2, 500);  //remove later
              read_line(&line_data);
              drive_left(TURN_SPEED);
          } 
          drive_stop(DEFAULT_SPEED);
          
  }
  else if(((line_data&0b1110000000111) == 0b1110000000111)){   //both side detected
      delay(100);                            //delay so car will stop an inch above the white line.
      drive_stop(DEFAULT_SPEED);
      delay(500); 
      uint16_t line_inch_data=0;      //temporary buffer to stores reading of line info an inch above the white line
      read_line(&line_inch_data);
   
      if(line_inch_data == 0b1111111111111){    //if it sees a solid line an inch above the previous solid line then it's the finish line          
          read_line(&line_data);
          while(line_data == 0b1111111111111){  //stop until vehicle is lifted off ground
              drive_stop(0);
              read_line(&line_data);
          }
      }
      else if((line_data&0b1111100000000) == 0b1111100000000){    //turn left                        
          while((line_data&0b0000000110000) != 0b0000000110000){  //keep turning left until center sensor detects line
              read_line(&line_data);
              drive_left(TURN_SPEED);
          } 
          drive_stop(DEFAULT_SPEED);
      } 
  }
  else{
      drive_forward(DEFAULT_SPEED, &line_data);
  }
}

/*~~~NOTES~~~
    Centered line value = 13'B0000001000000   //center point
    Centered line value = 13'B0000011100000   //center wide range
    
    Centered line value = 13'B1111000000000   //left point

*/
