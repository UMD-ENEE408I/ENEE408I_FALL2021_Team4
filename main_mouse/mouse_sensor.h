#ifndef MOUSE_SENSORS
#define MOUSE_SENSORS

//Reflective ADC Sensor-------------------------------------------------------------------------------------------------
#include <Adafruit_MCP3008.h>   //for reflective sensors
#define ADC_1_CS A3
#define ADC_2_CS A2

#define ADC_ERROR_MARGIN 55  // threshold = white line value + error margin

extern Adafruit_MCP3008 adc1;
extern Adafruit_MCP3008 adc2;
extern int adc_threshhold[13];

void read_line(uint16_t *line_data);  //takes in a 16-bit buffer and stores sensor data of the line into the line_data.
                                      //ex: 
                                      //    uint16_t line_buffer;
                                      //    read_line(&line_buffer);    //the data reading will be stored in line_buffer
void calibrate_adc(int *arr);  
void printThreshold();


#endif                                      
