/*
  uint16_t temp_data = 0;
  temp_data = line_data;

  if((temp_data&0b1111000000000) == 0b1111000000000){
          inch_forward(DEFAULT_SPEED + velocity_correction, 120);
          turn_left(TURN_SPEED, &line_data, &velocity_correction);  
      }
       else if((temp_data&0b0000000001111) == 0b0000000001111){
          inch_forward(DEFAULT_SPEED + velocity_correction, 120);
          turn_right(TURN_SPEED, &line_data, &velocity_correction);  
      }
      else{
          drive_forward(DEFAULT_SPEED + velocity_correction, &line_data);
      }
*/

/*
  uint16_t temp_data = 0;  
  switch(current_state){
      case IDLE_STATE:
           drive_stop(0);
              //Add code to recieve bluetooth instruction here
           break;

      case F_STATE:
              drive_forward(DEFAULT_SPEED + velocity_correction, &line_data);

              temp_data = line_data;
              if((temp_data&0b1111000001111) == 0b1111000001111){   //Both turns detected
                  drive_stop(0);
                  bluetooth_buffer |= 0b101;
                  
                  read_line(&temp_data);
                  inch_forward(DEFAULT_SPEED + velocity_correction, 90);
                  if((temp_data&0b1111111111111) == 0b1111111111111){  //Finish line
                      bluetooth_buffer |= 0b1111;
                  }
                  else if(temp_data & 0b0000011100000)   //Middle path
                      bluetooth_buffer |= 0b010;

                  current_state = IDLE_STATE;
              }
              else if((temp_data&0b0000000001111) == 0b0000000001111){  //Right turn
                  drive_stop(0);
                  bluetooth_buffer |= 0b001;
                  
                  read_line(&temp_data);
                  inch_forward(DEFAULT_SPEED + velocity_correction, 90);
                  if(temp_data & 0b0000011100000)   //Middle path
                      bluetooth_buffer |= 0b010;

                  current_state = IDLE_STATE;
              }
              else if((temp_data&0b1111000000000) == 0b1111000000000){
                  drive_stop(0);
                  bluetooth_buffer |= 0b100;
                  
                  read_line(&temp_data);
                  inch_forward(DEFAULT_SPEED + velocity_correction, 90);
                  if(temp_data & 0b0000011100000)   //Middle path
                      bluetooth_buffer |= 0b010;

                  current_state = IDLE_STATE;
              }
              else if(temp_data == 0){      //Dead end.
                  drive_stop(0);
                  bluetooth_buffer &= 0xFFFFFFF0
                  
                  current_state = IDLE_STATE;
              }   
           break;
           
      case L_STATE:
           break;

      case R_STATE:
           break;

      case U_STATE:
           break;

      case END_STATE:
           break;
  };
  */


  /*
    char buf[50];
    sprintf(buf, "L: %i, R: %i\n",enc1.read(),enc2.read());
    Serial.print(buf);
    */
