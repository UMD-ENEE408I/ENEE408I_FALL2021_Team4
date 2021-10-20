#include "mouse_sensor.h"         //includes functions for line sensor
#include "mouse_movement.h"       //includes function for mouse movement
#include "mouse_sound.h"          //includes all audio functions and tones

#include "template_functions.h"   //OPTIONAL. includes functions to print out binary

#define DEFAULT_SPEED 34  //speed of the car
#define TURN_SPEED    33

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
  

  /*
  if((line_data&0b0000000000111) == 0b0000000000111){    //if car is at intersectio or right turn
      //Serial.println("INTERSECTION");  
      drive_right(TURN_SPEED);
      while((line_data&0b0001100000000) != 0b0001100000000  &&  (line_data&0b1110011111111) != 0b0000000000000){        //keep turning right until center sensor detects line
          //Serial.println("TURN RIGHT");
          read_line(&line_data);             
      }
  }*/
  
  

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
          drive_left(TURN_SPEED);
          while(1){        
            //keep turning left until center sensor detects line //Serial.println("TURN LEFT");
              read_line(&line_data);

              if((line_data&0b0000000110000) != 0b0000000110000  &&  (line_data&0b1111111001111)!=0)
                break;
          } 
      }           
  }
  else if((line_data&0b0000000011111) == 0b0000000011111){    //if car is at intersection or right turn
      //Serial.println("INTERSECTION");
      inch_forward(DEFAULT_SPEED, 90);    
      uint16_t inch_data;           //data 1 inch from intersection
      read_line(&inch_data);
      
      if(inch_data&0b0001111100000){
          drive_forward(DEFAULT_SPEED, &line_data);
      }
      else{
          drive_right(TURN_SPEED);
          read_line(&line_data);
          //tone(BUZZ_PIN, NOTE_A5, 500);
          Serial.println("DRIVE RIGHT");   
          printBinaryN(line_data, 13);
          while(1){        //keep turning right until center sensor detects line
              read_line(&line_data);       

              if((line_data&0b0000110000000) != 0b0000110000000 &&  (line_data&0b1111001111111)!=0)
                break;
          }
      }
  }
  else if(!line_data){   //line_data == 0x0000       //U-turn
      drive_left(TURN_SPEED);
      while(1){        
        //keep turning left until center sensor detects line //Serial.println("TURN LEFT");
          read_line(&line_data);
          
          if((line_data&0b0000000110000) != 0b0000000110000  &&  (line_data&0b1111111001111)!=0)
                break;
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
