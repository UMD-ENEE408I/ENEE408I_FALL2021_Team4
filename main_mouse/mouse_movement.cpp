#include "Arduino.h"
#include "mouse_movement.h"
#include "mouse_sensor.h"

Encoder enc1(M1_ENC_A, M1_ENC_B); //left wheel
Encoder enc2(M2_ENC_A, M2_ENC_B); //right wheel

const double Kp_speed = 0;   
const double Kd_speed = 0; 
int prev_error_speed;

const double Kp_center = 2.7;  //3
const double Ki_center = 0;
const double Kd_center = 0.8; 
int prev_error_center;

const double Kp_enc = 0.2; 
const double Ki_enc = 0;   
const double Kd_enc = 0.2; 
int prev_error_enc;

const double Kp_turn = 3; 
const double Ki_turn = 0;    
const double Kd_turn = 1.4;   //1.2
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

int speed_pid(long unsigned  prev_time, long unsigned prev_angle){  //regulate speed for right wheel.
   /* long unsigned elapse_time = millis() - prev_time;
    long unsigned    rotation = abs(enc2.read()) - prev_angle;

    int ms_per_mm = (long)elapse_time / rotation;
    
    int error = 10 - ms_per_mm;  //desired speed value is 10 ms/mm
    int P, D;

    //Serial.println(ms_per_mm);
    
    P = error;
    D = error - prev_error_center;
    prev_error_speed = error;
    
    int error_correction = int(P*Kp_speed + D*Kd_speed); 

    return error_correction;*/
    return 0;
}

int center_error(uint16_t *line_data){
    int L_error, R_error;
    L_error = R_error = 0;
    for(int i=0; i<5; i++){
        if((0x80<<i) & *line_data)
          L_error = i;
        if((0x20>>i) & *line_data)
          R_error = i;
    }
    
    int error = L_error - R_error;
    int P, I, D;
    
    P = error;
    I = error + prev_error_enc;
    D = error - prev_error_center;
    prev_error_center = error;
    int error_correction = int(P*Kp_center + I*Ki_center + D*Kd_center); 

    return error_correction;
}

int encoder_error(){
    int error = enc1.read() - enc2.read();

    int P, I, D;
    P = error;
    I = error + prev_error_enc;
    D = error - prev_error_enc;
    prev_error_enc = error;
    int error_correction = int(P*Kp_enc + I*Ki_enc + D*Kd_enc);

    return error_correction%10;
}

int turn_error(){
    int error = enc1.read() + enc2.read();
    
    int P, I, D;
    P = error;
    I = error + prev_error_enc;
    D = error - prev_error_enc;
    prev_error_enc = error;
    int error_correction = int(P*Kp_turn + I*Ki_turn + D*Kd_turn);

    if(error_correction > 5)
      error_correction = 5;

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
    
    /*
    char buf[50];
    sprintf(buf, "L: %i, R: %i\n",enc1.read(),enc2.read());
    Serial.print(buf);
    */
    
    L_forward(L_speed);
    R_forward(R_speed);
}

void drive_backward(int speed){
    L_backward(speed);
    R_backward(speed);
}

void drive_stop(int speed){  
    /*while(speed > 0){
        speed --;
        if(speed<0) speed = 0;
     
        drive_forward(speed, NULL);
    }
    */
    R_stop();
    L_stop();
}

void drive_right(int speed){
    int turn_correction = turn_error();

    /*//BROKE
    long unsigned prev_time  = millis();
    long unsigned prev_angle = abs(enc2.read());
    int speed_calibrate = speed_pid(prev_time, prev_angle); //Makes sure wheel turns at constant speed
   */
    
    L_forward(speed - turn_correction);
    R_backward(speed);  
}

void drive_left(int speed){
    int turn_correction = turn_error();
    
    L_backward(speed + turn_correction);
    R_forward(speed); 
}

void turn_right(int speed){
    uint16_t local_line_data;   //named local to not be confused with the line_data used in the main function
    
    while(1){       
        /*char buf[50];
        sprintf(buf, "L: %i, R: %i\n",enc1.read(),enc2.read());
        Serial.print(buf);
        */
        drive_right(speed);    
        read_line(&local_line_data);
        
        if((local_line_data&0b0000001000000) &&  
           (local_line_data&0b1111100011111) == 0   )
          break;
    }
    drive_stop(0);
}

void turn_left(int speed){
    uint16_t local_line_data;   //named local to not be confused with the line_data used in the main function
    while(1){        
        char buf[50];
        sprintf(buf, "L: %i, R: %i\n",enc1.read(),enc2.read());
        Serial.print(buf);  
        
        drive_left(speed);
        read_line(&local_line_data);
        
        if((local_line_data&0b0000001000000) &&  
           (local_line_data&0b1111100011111) == 0   )
          break;
    }
    drive_stop(0);
}

void inch_forward(int speed, int angle){
    enc2.write(0);
    
    L_forward(speed + 6);
    R_forward(speed+3);
    
    while(enc2.read() < angle){
        //do nothing. the wheel's already rotating
    }
    drive_stop(speed);
}

void inch_backward(int speed, int angle){
    enc2.write(0);
    
    L_backward(speed);
    R_backward(speed);
    
    while(enc2.read() > -angle){
        //do nothing. the wheel's already rotating
    }
    drive_stop(speed);
}
