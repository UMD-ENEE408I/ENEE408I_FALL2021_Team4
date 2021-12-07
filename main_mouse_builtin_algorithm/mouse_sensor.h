#ifndef MOUSE_SENSORS
#define MOUSE_SENSORS

#include <Arduino_LSM9DS1.h>

//Reflective ADC Sensor-------------------------------------------------------------------------------------------------
#include <Adafruit_MCP3008.h>   //for reflective sensors
#define ADC_1_CS A3
#define ADC_2_CS A2

#define ADC_ERROR_MARGIN 60  // threshold = white line value + error margin

extern Adafruit_MCP3008 adc1;
extern Adafruit_MCP3008 adc2;
extern int adc_threshhold[13];

void read_line(uint16_t *line_data);  //takes in a 16-bit buffer and stores sensor data of the line into the line_data.
                                      //ex: 
                                      //    uint16_t line_buffer;
                                      //    read_line(&line_buffer);    //the data reading will be stored in line_buffer
void calibrate_adc(int *arr);  
void printThreshold();

int detectIntersection(uint16_t *line_data);  //returns 1 if it sees a left or right intersection. return 0 otherwise.

int spinCalibrate(int speed, int *velocity_correction, uint16_t *line_data, int *arr);

/*GYRO CODES------------------------------*/
void setupGyro();
void calculateGyro(float *rps, float *current_angle);
#endif                                      
