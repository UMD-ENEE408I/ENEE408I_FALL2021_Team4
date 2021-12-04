#include "Arduino.h"
#include "mouse_movement.h"
#include "mouse_sensor.h"
#include <PID_v1.h>

double lineSetpoint, lineError, lineCorrection;  //input and output
PID motorPID(&lineError, &lineCorrection, &lineSetpoint, 1.5, 0.15, 0.23, DIRECT); //setPint, Kd=1.5, Ki, Kd=0.25

double rotationSetpoint;
double rotationError, rotationCorrection; //controls the two wheel to have the same rotation angle
PID rotationPID(&rotationError, &rotationCorrection, &rotationSetpoint, 0.2, 0.001, 0.012, DIRECT); //Kd=0.2, Ki, Kd=0.01

double angleSetpoint;
double leftTurnError, leftTurnCorrection; //keeps the wheel turning at constant velocity
PID leftTurnPID(&leftTurnError, &leftTurnCorrection, &angleSetpoint, 1, 0, 0, DIRECT); //Kd=, Ki=, Kd=

double rightTurnError, rightTurnCorrection; //keeps the wheel turning at constant velocity
PID rightTurnPID(&rightTurnError, &rightTurnCorrection, &angleSetpoint, 0.18, 0.01, 0.0004, DIRECT); //Kd=0.15, Ki=0.01, Kd=0.0005


//FOR THE GYRO
float gyroAngle = 0;
float gyroRPS   = 0;    //angles(deg) per seconds

Encoder enc1(M1_ENC_A, M1_ENC_B); //left wheel
Encoder enc2(M2_ENC_A, M2_ENC_B); //right wheel


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


//ADDED
void setupPID(){
    motorPID.SetOutputLimits(-255,255);
    motorPID.SetMode(AUTOMATIC);
    lineSetpoint = 0;
    
    rightTurnPID.SetOutputLimits(-255,255);
    rightTurnPID.SetMode(AUTOMATIC);
    angleSetpoint = 0;

    rotationPID.SetOutputLimits(-10,10);
    rotationPID.SetMode(AUTOMATIC);  
    rotationSetpoint = 0;
}

void calculateSensorError(uint16_t *line_data){

    switch(*line_data){
        case 0b1000000000000:
            lineError = -7;
            break;
        case 0b1100000000000:
            lineError = -6;
            break;    
        case 0b0110000000000:
            lineError = -5;
            break;
        case 0b0011000000000:
            lineError = -4;
            break;
        case 0b0001100000000:
            lineError = -3;
            break;
        case 0b0000110000000:
            lineError = -2;
            break;
        case 0b0000011000000:
            lineError = -1;
            break;
        case 0b0000001000000:
            lineError = 0;
            break;
        case 0b0000001100000:
            lineError = 1;
            break;
        case 0b0000000110000:
            lineError = 2;
            break;
        case 0b0000000011000:
            lineError = 3;
            break;
        case 0b0000000001100:
            lineError = 4;
            break;
        case 0b0000000000110:
            lineError = 5;
            break;
        case 0b0000000000011:
            lineError = 6;
            break;
        case 0b0000000000001:
            lineError = 7;
            break;   
        default:
            lineError = 0;
    }

    for(int i=0; i<13; i++){
        if(*line_data == (0x1000>>i))
            lineError = i-6;
    }
    
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
    calculateSensorError(line_data);  
    motorPID.Compute();

    int L_speed = constrain(speed - lineCorrection, 0, 255);
    int R_speed = constrain(speed + lineCorrection, 0, 255);
    
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


void drive_right(int speed){
    calculateGyro(&gyroRPS, &gyroAngle);

    //Serial.println(abs(gyroRPS));
    //PID to maintain constant linear velocity
    rightTurnError = 100.0 - abs(gyroRPS); //desired "rps"
    rightTurnPID.Compute();
    
    //PID to keep right wheel following same rotation as left wheel
    rotationError = enc1.read() + enc2.read();
    rotationPID.Compute();
   
    int L_speed = constrain(speed - rightTurnCorrection + rotationCorrection, 0, 255);
    int R_speed = constrain(speed - rightTurnCorrection - rotationCorrection, 0, 255);
    
    L_forward(L_speed);
    R_backward(R_speed); 
}

void drive_left(int speed){

    L_backward(speed);
    R_forward(speed); 
}

void turn_right(int speed, uint16_t *line_data, float targetAngle){
    //calculateGyro(&gyroRPS, &gyroAngle);
    //rightTurnPID.Compute();
/*    gyroRPS   = 0;
    gyroAngle = 0;
    enc1.write(0);
    enc2.write(0);
    
    while(abs(gyroAngle) < targetAngle){
        drive_right(speed);
    }
    
    //drive_stop(0);
    */
    enc1.write(0);
    enc2.write(0);
}

void turn_left(int speed, uint16_t *line_data){

    drive_stop(0);

    enc1.write(0);
    enc2.write(0);
}

void inch_forward(int speed, int angle, uint16_t *line_data, uint32_t *bt_buffer){
    enc1.write(0);
    enc2.write(0);

    drive_stop(speed);

    enc1.write(0);
    enc2.write(0);
}

void inch_backward(int speed, int angle){

    drive_stop(speed);

    enc1.write(0);
    enc2.write(0);
}
