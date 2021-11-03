#include "mouse_sensor.h"         //includes functions for line sensor
#include "mouse_movement.h"       //includes function for mouse movement
#include "mouse_sound.h"          //includes all audio functions and tones

#include "template_functions.h"   //OPTIONAL. includes functions to print out binary

#define DEFAULT_SPEED 32  //speed of the car
#define TURN_SPEED    36
char str_buf[128];

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

  //encoder
  enc1.write(0);  //reset encoder to 0
  enc2.write(0);  //reset encoder to 0

  //Initiate sensor
  PLAY_SOUND(calibrate_begin_sound, calibrate_begin_delay);
  calibrate_adc(adc_threshhold); 
  PLAY_SOUND(calibrate_end_sound, calibrate_end_delay);
}

void loop() {
  uint16_t line_data;

  read_line(&line_data);
  printBinaryN(line_data, 13);

  //TEST CODE
  //drive_forward(DEFAULT_SPEED, &line_data);
  //turn_right(TURN_SPEED);
  //turn_left(TURN_SPEED);
  //inch_backward(DEFAULT_SPEED+10, 180);
  //delay(3000);

  if((line_data&0b1111100000000) == 0b1111100000000){
          turn_left(TURN_SPEED);  
      }
      else if((line_data&0b0000000011111) == 0b0000000011111){
          turn_right(TURN_SPEED);
      }
      else{
          drive_forward(DEFAULT_SPEED, &line_data);
      }

/*
  if((line_data&0b1111100000000) == 0b1111100000000){          //if car sees left turn 
      inch_forward(DEFAULT_SPEED, 60);    
      uint16_t inch_data;           //data 1 inch from intersection
      read_line(&inch_data);

      if((inch_data&0b1111111111111) == 0b1111111111111){         //FOUND THE END
          drive_stop(DEFAULT_SPEED);
          read_line(&line_data);
          
          while((line_data&0b1111111111111) == 0b1111111111111){
              //Serial.println("END GOAL");
              read_line(&line_data);
              //DO NOTHING
          }
      }
      else{
          turn_left(TURN_SPEED);
      }           
  }
  else if((line_data&0b0000000011111) == 0b0000000011111){    //if car is at intersection or right turn
      inch_forward(DEFAULT_SPEED, 90);    
      uint16_t inch_data;           //data 1 inch from intersection
      read_line(&inch_data);
      
      if(inch_data&0b0001111100000){
          drive_forward(DEFAULT_SPEED, &line_data);
      }
      else{
          turn_right(TURN_SPEED);
      }
  }
  else if(!line_data){   //line_data == 0x0000       //U-turn
      turn_left(TURN_SPEED);
  }
  else{
      //Serial.println("DRIVE FORWARD");                                                   
      drive_forward(DEFAULT_SPEED, &line_data);
  }
  */
  
}

/*~~~NOTES~~~
    Centered line value = 13'B0000001000000   //center point
    Centered line value = 13'B0000011100000   //center wide range
    
    Centered line value = 13'B1111000000000   //left point
*/
