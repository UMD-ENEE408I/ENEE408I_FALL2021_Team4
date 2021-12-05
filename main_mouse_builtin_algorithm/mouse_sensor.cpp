#include "Arduino.h"
#include "mouse_sensor.h"

Adafruit_MCP3008 adc1;
Adafruit_MCP3008 adc2;

//value below threshold are considered white lines
int adc_threshhold[13];

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

int detectIntersection(uint16_t *line_data){
    if((*line_data&0b0000000001111) == 0b0000000001111 || 
       (*line_data&0b1111000000000) == 0b1111000000000     )
        return 1;
    else
        return 0;
}

int spinCalibrate(int speed, int *velocity_correction, uint16_t *line_data, int *arr){
    /*arr[12]= adc1.readADC(0);  arr[11]= adc2.readADC(0);
    arr[10]= adc1.readADC(1);  arr[9] = adc2.readADC(1);
    arr[8] = adc1.readADC(2);  arr[7] = adc2.readADC(2);
    arr[6] = adc1.readADC(3);  arr[5] = adc2.readADC(3);
    arr[4] = adc1.readADC(4);  arr[3] = adc2.readADC(4);
    arr[2] = adc1.readADC(5);  arr[1] = adc2.readADC(5); 
    arr[0] = adc1.readADC(6); 

    //drive_left(int speed, int *velocity_correction);

    arr[0] = (adc1.readADC(6) < arr[0]) ? adc1.readADC(6) : arr[0];
    */
    //NONFUNCTIONAL
}

/*---------------------------GYRO CODES-------------------------------------------*/


float bias = 0;
float gx, gy, gz;
float prev_time = 0;
void setupGyro(){
    if (!IMU.begin()) {
        //Serial.println("Failed to initialize IMU!");
        while (1);
    }
    
    float counter=0;
    float reg;
    while(1){
      if (IMU.gyroscopeAvailable()) {
          IMU.readGyroscope(gx, gy, gz);
          
          if (counter < 100){
            counter++;
            continue;
          }
          counter++;
          reg = gx+reg;
      }
    
      if (counter==1000){
        break;
      }
    }
    bias = reg  / (counter-100);  
}

void calculateGyro(float *rps, float *current_angle){
    if (IMU.gyroscopeAvailable()) { // might be some minimum polling time, the gyroscope might lose availability after taking a sample   
        IMU.readGyroscope(gx, gy, gz);
         
        float current_time = (float) micros()*(1.0/1000000.0);
        float dt = current_time - prev_time;
        prev_time = current_time;
        
        float delta_angle= (gz-bias)*dt*9.0/8.0;
        *current_angle = *current_angle + delta_angle;       
        *rps = gz - bias;
    }
}
