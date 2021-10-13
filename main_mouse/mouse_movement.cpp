#include "Arduino.h"
#include "mouse_movement.h"

//Encoder enc1(M1_ENC_A, M1_ENC_B); //left wheel
Encoder enc2(M2_ENC_A, M2_ENC_B); //right wheel

const float Kp = 2.75;    //proportional gain   current optimal value=2.75
const float Ki = 0;       //integral gain       not needed
const float Kd = 0.8;     //derivative gain     current optimal value = 0.8
const int   left_wheel_coeff = 3;   //left wheel speed is different from right wheel. increase this value to increase
                                    //the default speed of the left wheel
const int   right_wheel_coeff = 2;
 
int prev_error;

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
    
    int error = L_error - R_error;
    int P, I, D;
    
    P = error;
    I = I + error;
    D = error - prev_error;
    prev_error = error;
    int error_correction = int(P*Kp + I*Ki + D*Kd); 

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
    int error_correction = center_error(line_data);
    int L_speed = speed + left_wheel_coeff - error_correction;
    int R_speed = speed;
  
    /*char buf[50];
    sprintf(buf, "Left: %i\tRight: %i\terror_correction: %i\n", L_speed, R_speed, error_correction);
    Serial.print(buf);*/
    L_forward(L_speed);
    R_forward(R_speed);
}

void drive_backward(int speed){
    L_backward(speed+left_wheel_coeff);
    R_backward(speed);
}

void drive_stop(int speed){  
    while(speed > 0){
        speed --;
        if(speed<0) speed = 0;
     
        drive_forward(speed, NULL);
    }
    R_stop();
    L_stop();
}

void drive_right(int speed){
    L_forward(speed + left_wheel_coeff);
    R_backward(speed + right_wheel_coeff);
}

void drive_left(int speed){
    L_backward(speed + left_wheel_coeff);
    R_forward(speed + right_wheel_coeff);
}

//void inch_forward(int speed, int duration){
//    L_forward(speed + 6);
//    R_forward(speed);
//    delay(duration);
//    drive_stop(speed);
//}
//
//void inch_backward(int speed, int duration){
//    L_backward(speed + 4);
//    R_backward(speed);
//    delay(duration);
//    drive_stop(speed);
//}

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
    
    L_backward(speed + 4);
    R_backward(speed);
    
    while(enc2.read() > -angle){
        //do nothing. the wheel's already rotating
    }
    drive_stop(speed);
}
