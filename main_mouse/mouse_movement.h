#ifndef MOUSE_MOVEMENT
#define MOUSE_MOVEMENT

#include <Encoder.h>
//Pin numbers for encoder
//#define M1_ENC_A 6
//#define M1_ENC_B 7
#define M2_ENC_A 9
#define M2_ENC_B 8

//extern Encoder enc1;
//extern Encoder enc2;

//Pin numbers for H-bridge motor control
#define L_Motor_1 2
#define L_Motor_2 3
#define R_Motor_1 5
#define R_Motor_2 4

int center_error(uint16_t *line_data);  //calculates the error difference and return the error correction value

void L_backward(int speed);
void L_forward(int speed);
void L_stop();

void R_backward(int speed);
void R_forward(int speed);
void R_stop();

void drive_forward(int speed, uint16_t *line_data);
void drive_backward(int speed);

void drive_stop(int speed);

void drive_right(int speed);
void drive_left(int speed);

void inch_forward(int speed, int duration);
void inch_backward(int speed, int duration);

//void inch_forward(int speed, int angle);
//void inch_backward(int speed, int angle);

#endif
