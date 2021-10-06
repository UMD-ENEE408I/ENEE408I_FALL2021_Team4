#include "mouse_sensor.h"         //includes functions for line sensor
#include "mouse_movement.h"       //includes function for mouse movement
#include "mouse_sound.h"          //includes all audio functions and tones

#include "template_functions.h"   //OPTIONAL. includes functions to print out binary

#define DEFAULT_SPEED 34  //speed of the car
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

  //encoder
  //enc2.write(0);  //reset encoder to 0

  //Initiate sensor
  PLAY_SOUND(calibrate_begin_sound, calibrate_begin_delay);
  calibrate_adc(adc_threshhold); 
  PLAY_SOUND(calibrate_end_sound, calibrate_end_delay);
}

void loop() {
  uint16_t line_data;

  read_line(&line_data);
  printBinaryN(line_data, 13);

  if((line_data&0b1110000000000) == 0b1110000000000){          //if car sees left turn 
      //inch_forward(DEFAULT_SPEED, 60); 
      drive_left(TURN_SPEED);
      while((line_data&0b0000000011000) != 0b0000000011000  &&  (line_data&0b1111111100111) != 0b0000000000000){        
        //keep turning left until center sensor detects line //Serial.println("TURN LEFT");
          read_line(&line_data);
      }        
  }
  /*
  else if((line_data&0b0000000000111) == line_data&0b0000000000111){    //if car is at intersectio or right turn
      //Serial.println("INTERSECTION");
      inch_forward(DEFAULT_SPEED, 500);    
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
      else if(inch_data&0b0001111100000){
          drive_forward(DEFAULT_SPEED, &line_data);
      }
      else{
          inch_backward(DEFAULT_SPEED, 60);
          while((line_data&0b0001100000000) != 0b0001100000000){        //keep turning right until center sensor detects line
              //Serial.println("TURN RIGHT");
              read_line(&line_data);
              drive_right(TURN_SPEED);
          } drive_stop(DEFAULT_SPEED);
      }
  }*/
  else if((line_data&0b0000000000000) == 0b0000000000000){          //U-turn
      drive_left(TURN_SPEED);
      while((line_data&0b0000000011000) != 0b0000000011000  &&  (line_data&0b1111111100111) != 0b0000000000000){        
        //keep turning left until center sensor detects line //Serial.println("TURN LEFT");
          read_line(&line_data);
      } 
  } 
  else{
      //Serial.println("DRIVE FORWARD");                                                   
      drive_forward(DEFAULT_SPEED, &line_data);
  }
  
}

/*~~~NOTES~~~
    Centered line value = 13'B0000001000000   //center point
    Centered line value = 13'B0000011100000   //center wide range
    
    Centered line value = 13'B1111000000000   //left point
*/
