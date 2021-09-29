#include "Arduino.h"
#include "mouse_sensor.h"

Adafruit_MCP3008 adc1;
Adafruit_MCP3008 adc2;

//value below threshold are considered white lines
int adc_threshhold[13];
char str_buf[128];

void read_line(uint16_t *line_data){
    int adc_buf[13];
    //reading in sensor values
    adc_buf[12] = adc1.readADC(0);  adc_buf[11] = adc2.readADC(0);
    adc_buf[10] = adc1.readADC(1);  adc_buf[9]  = adc2.readADC(1);
    adc_buf[8]  = adc1.readADC(2);  adc_buf[7]  = adc2.readADC(2);
    adc_buf[6]  = adc1.readADC(3);  adc_buf[5]  = adc2.readADC(3);
    adc_buf[4]  = adc1.readADC(4);  adc_buf[3]  = adc2.readADC(4);
    adc_buf[2]  = adc1.readADC(5);  adc_buf[1]  = adc2.readADC(5); 
    adc_buf[0]  = adc1.readADC(6); 
    
//    sprintf(str_buf, "%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\n",
//                    adc_buf[0], adc_buf[1], adc_buf[2], adc_buf[3], adc_buf[4], adc_buf[5], adc_buf[6], 
//                    adc_buf[7], adc_buf[8], adc_buf[9], adc_buf[10], adc_buf[11], adc_buf[12]  );
//                    Serial.print(str_buf);      
    
    *line_data = 0;     //initialized as 16'b---0000000000000
    for(int i=0; i<13; i++){
        if(adc_buf[i] < adc_threshhold[i])
          *line_data |= (0x1000>>i);
    }
}

void calibrate_adc(int *arr){
    arr[12]= adc1.readADC(0)+ADC_ERROR_MARGIN;  arr[11]= adc2.readADC(0)+ADC_ERROR_MARGIN;
    arr[10]= adc1.readADC(1)+ADC_ERROR_MARGIN;  arr[9] = adc2.readADC(1)+ADC_ERROR_MARGIN;
    arr[8] = adc1.readADC(2)+ADC_ERROR_MARGIN;  arr[7] = adc2.readADC(2)+ADC_ERROR_MARGIN;
    arr[6] = adc1.readADC(3)+ADC_ERROR_MARGIN;  arr[5] = adc2.readADC(3)+ADC_ERROR_MARGIN;
    arr[4] = adc1.readADC(4)+ADC_ERROR_MARGIN;  arr[3] = adc2.readADC(4)+ADC_ERROR_MARGIN;
    arr[2] = adc1.readADC(5)+ADC_ERROR_MARGIN;  arr[1] = adc2.readADC(5)+ADC_ERROR_MARGIN; 
    arr[0] = adc1.readADC(6)+ADC_ERROR_MARGIN; 
}

void printThreshold(){
    for(int i=0; i<13; i++){
      Serial.print(adc_threshhold[i]); Serial.print(" ");
    } Serial.println();  
};
