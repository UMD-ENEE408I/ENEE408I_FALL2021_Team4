#include "Arduino.h"
#include "mouse_movement.h"
#include "mouse_sensor.h"

#define SLOW_COEFF 0

Encoder enc1(M1_ENC_A, M1_ENC_B); //left wheel
Encoder enc2(M2_ENC_A, M2_ENC_B); //right wheel

int prev_error_center;

const double Kp_enc = 0.1;  //2 
const double Kd_enc = 0.05; //.4
int prev_error_enc;

const double Kp_turn = 3;   
const double Kd_turn = 2;   //1.6
int prev_error_turn;



/*  NOTES ~~Ignore This~~Different variation of how the camera define when to turn
  Centered line value = 13'B0000001000000
  Tilted Left         = 13'B0000010000000
  Tilted Right        = 13'B0000000100000

  Centered line value = 13'B0000011100000
  Tilted Left         = 13'B0000100000000
  Tilted Right        = 13'B0000000010000
  
  Centered line value = 13'B0000011000000
  Tilted Left         = 13'B0000100000000
  Tilted Right        = 13'B0000000100000
*/


int center_error(uint16_t *line_data){
    int L_error, R_error;
    L_error = R_error = 0;
    for(int i=0; i<5; i++){
        if((0x80<<i) & *line_data)
          L_error = i;
        if((0x20>>i) & *line_data)
          R_error = i;
    }
    
    double Kp_center = 7;     //3  4.2  5.4
    double Kd_center = 4;     //3  3.8  4.0  
    int error = L_error - R_error;

    //Increase scaling coefficient when closer to the center
    if(abs(error) == 1){
        Kp_center = 9;
        Kd_center = 3;
    }

    prev_error_center = error;
    int error_correction = int((error)*Kp_center + (error - prev_error_center)*Kd_center); 

    return error_correction;
}

int encoder_error(){
    int error = enc1.read() - enc2.read();

    int P, D;
    P = error;
    D = error - prev_error_enc;
    prev_error_enc = error;
    int error_correction = int(P*Kp_enc + D*Kd_enc);

    if(error_correction > 5)
      error_correction = 5;
      
    //return error_correction%10; //works
    return error_correction;
}

int turn_error(){
    int error = enc1.read() + enc2.read();
    
    int P, D;
    P = error;
    D = error - prev_error_turn;
    prev_error_turn = error;
    int error_correction = int(P*Kp_turn + D*Kd_turn);


    if(error_correction > 5)
      error_correction = 5;

    //return error_correction%10;
    return error_correction;
}

void L_backward(int speed){
    analogWrite(L_Motor_1, speed);
    analogWrite(L_Motor_2, 0);
}

void L_forward(int speed){
    analogWrite(L_Motor_1, 0);
    analogWrite(L_Motor_2, speed);
}

void L_stop(){
    analogWrite(L_Motor_1, 0);
    analogWrite(L_Motor_2, 0);
}

void R_backward(int speed){
    analogWrite(R_Motor_1, speed);
    analogWrite(R_Motor_2, 0);
}

void R_forward(int speed){
    analogWrite(R_Motor_1, 0);
    analogWrite(R_Motor_2, speed);
}

void R_stop(){
    analogWrite(R_Motor_1, 0);
    analogWrite(R_Motor_2, 0);
}

void drive_forward(int speed, uint16_t *line_data){   
    int center_correction = center_error(line_data);
    int enc_correction = encoder_error();

    int L_speed = speed - enc_correction - center_correction;
    int R_speed = speed;
    
    L_forward(L_speed);
    R_forward(R_speed);
}

void drive_backward(int speed){
    L_backward(speed);
    R_backward(speed);
}

void drive_stop(int speed){  
    R_stop();
    L_stop();
}


void drive_right(int speed, int *velocity_correction){
    int turn_correction = turn_error();

    L_forward(speed + *velocity_correction);
    R_backward(speed + turn_correction);  
}

void drive_left(int speed, int *velocity_correction){
    int turn_correction = turn_error();
    
    L_backward(speed + turn_correction);
    R_forward(speed + *velocity_correction); 
}

void turn_right(int speed, uint16_t *line_data, int *velocity_correction){
    //uint16_t local_line_data;   //named local to not be confused with the line_data used in the main function
    int slowdown;

    *velocity_correction = 0;
    prev_error_turn = 0;
    enc1.write(0);
    enc2.write(0);

    drive_right(speed, velocity_correction);
    delay(150); 
    
    while(1){       
        slowdown = 0;
        for(int i=0; i<6; i++){
          if((0b0010<<i) & *line_data)
            slowdown=i;
        }
        
        drive_right(speed - (slowdown*SLOW_COEFF), velocity_correction);  
        
        if((*line_data&0b0000001000000) &&  
           (*line_data&0b1111100011111) == 0 )
          break;
    }
    drive_stop(0);

    enc1.write(0);
    enc2.write(0);
}

void turn_left(int speed, uint16_t *line_data, int *velocity_correction){
    uint16_t local_line_data;   //named local to not be confused with the line_data used in the main function
    int slowdown;

    *velocity_correction = 0;
    prev_error_turn = 0;
    enc1.write(0);
    enc2.write(0);

    drive_left(speed, velocity_correction);
    delay(150);

    //read_line(&local_line_data);
    while(1){        
        slowdown = 0;
        for(int i=0; i<6; i++){
          if((0x1000>>i) & *line_data)
            slowdown=i;
        }
        
        drive_left(speed - (slowdown*SLOW_COEFF), velocity_correction);
          
        if((*line_data&0b0000001000000) &&  
           (*line_data&0b1111100011111) == 0   )
          break;
    }
    drive_stop(0);

    enc1.write(0);
    enc2.write(0);
}

void inch_forward(int speed, int angle, uint16_t *line_data, uint32_t *bt_buffer){
    enc1.write(0);
    enc2.write(0);

    int enc_correction = encoder_error();
    
    int L_speed = speed - enc_correction;
    int R_speed = speed;
    
    L_forward(L_speed);
    R_forward(R_speed);
    
    while(enc2.read() < angle){
        enc_correction = encoder_error();
        L_speed = speed + enc_correction;
        L_forward(L_speed);

        if((*line_data&0b1111000000000) == 0b1111000000000)
            *bt_buffer |= 0b100;
        if((*line_data&0b0000000001111) == 0b0000000001111)
            *bt_buffer |= 0b001; 
    }
    drive_stop(speed);

    enc1.write(0);
    enc2.write(0);
}

void inch_backward(int speed, int angle){
    enc1.write(0);
    enc2.write(0);

    int enc_correction = encoder_error();
    
    int L_speed = speed - enc_correction;
    int R_speed = speed;
    
    L_backward(L_speed);
    R_backward(R_speed);
    
    while(enc2.read() > -angle){
        enc_correction = encoder_error();
        L_speed = speed + enc_correction;
        L_backward(L_speed);
    }
    drive_stop(speed);

    enc1.write(0);
    enc2.write(0);
}
