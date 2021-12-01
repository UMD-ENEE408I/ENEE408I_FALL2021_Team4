static uint32_t bluetooth_buffer;
static uint16_t line_data;
static uint8_t  current_state;
static uint8_t  turn_flag;            //=1 whenever a turn is made. will be turned off in t2_func()
static int      velocity_correction;

#include <ArduinoBLE.h>
#include "mouse_sensor.h"         //includes functions for line sensor
#include "mouse_movement.h"       //includes function for mouse movement
#include "mouse_sound.h"          //includes all audio functions and tones
#include "main_definitions.h"     //RTOS and other stuff
#include "mouse_bluetooth.h"

#include "template_functions.h"   //OPTIONAL. includes functions to print out binary

#define DEFAULT_SPEED   32  //speed of the car
#define TURN_SPEED      34
#define MAX_CORRECTION  12  //max correction value for velocity
char str_buf[128];


void setup() {
  Serial.begin(115200);
//  if (DEBUG){
//    while(!Serial);
//  }

  setupBLE();

  bluetooth_buffer = 0;   //preset buffer value
  current_state    = 0;
  turn_flag       = 0;   //initially use right wheel as constant velocity
  
  velocity_correction = 0;
  
  //reflective sensors
  adc1.begin(ADC_1_CS);  
  adc2.begin(ADC_2_CS);

  //wheels motor
  pinMode(L_Motor_1, OUTPUT);
  pinMode(L_Motor_2, OUTPUT);
  pinMode(R_Motor_1, OUTPUT);
  pinMode(R_Motor_2, OUTPUT);

  //encoder
  enc1.write(0);  //reset encoder to 0
  enc2.write(0);  //reset encoder to 0

  //Initiate sensor
  PLAY_SOUND(calibrate_begin_sound, calibrate_begin_delay);
  calibrate_adc(adc_threshhold); 
  PLAY_SOUND(calibrate_end_sound, calibrate_end_delay);

  //Initiate rtos functions
  t1.start(t1_func);
  t2.start(t2_func);
}
 
void loop() {
  //read_line(&line_data);
  //printBinaryN(line_data, 13);
    uint8_t CURRENT_STATE = 0;
    uint16_t data_buffer = 0;

    // listen for BLE centrals to connect:
    BLEDevice central = BLE.central();

    // if a central is connected to peripheral:
    if (central) {
        //Serial.print("Connected to central: ");
        //Serial.println(central.address());

        // while the central is still connected to peripheral:
        while (central.connected()) {
            // if the remote device wrote to the characteristic,
            if (instrCharacteristic.written()) {
                CURRENT_STATE = 0b1111 & instrCharacteristic.value();
                //printBinary(instrCharacteristic.value());
                Serial.println(CURRENT_STATE);
            } 
            mouseDataCharacteristic.writeValue(bluetooth_buffer);

            //States of the vehicle
            switch(CURRENT_STATE){
                case IDLE_STATE:
                    bluetooth_buffer |= 0x80000000;   //Turn on bit that indicates a node hit
                    mouseDataCharacteristic.writeValue(bluetooth_buffer);
                    drive_stop(0);
                    break;
                case F_STATE:
                    bluetooth_buffer &= 0;    //reset buffer value  
                    //bluetooth_buffer |= (line_data<<4);   //storing line data in the distance spot
                    mouseDataCharacteristic.writeValue(bluetooth_buffer); 
                                
                    while(!detectIntersection(&line_data)){   //continuously look for intersection
                        drive_forward(DEFAULT_SPEED+velocity_correction, &line_data);
                    }
                    drive_stop(0);
                    
                    bluetooth_buffer |= uint16_t(enc2.read()<<4); //copy down encoder value

                    if((line_data&0b1111000000000) == 0b1111000000000)
                        bluetooth_buffer |= 0b100;
                    if((line_data&0b0000000001111) == 0b0000000001111)
                        bluetooth_buffer |= 0b001;                 
                    
                    inch_forward(DEFAULT_SPEED + velocity_correction, 140, &line_data, &bluetooth_buffer);
                    if(line_data&0b0000111110000)   //front path detected
                        bluetooth_buffer |= 0b010;
                    if((line_data&0b0000111110000) == 0b0000111110000)   //end point detected
                        bluetooth_buffer |= 0b1000;
                        
                    CURRENT_STATE = IDLE_STATE;
                    break;
                 case L_STATE:
                    bluetooth_buffer &= 0;    //reset buffer value  
                    mouseDataCharacteristic.writeValue(bluetooth_buffer); 
                    
                    //inch_forward(DEFAULT_SPEED + velocity_correction_L, 90);
                    turn_left(TURN_SPEED, &line_data, &velocity_correction);  
                    turn_flag = 1;
                    
                    CURRENT_STATE = F_STATE;
                    break;
                 case R_STATE:
                    bluetooth_buffer &= 0;    //reset buffer value  
                    mouseDataCharacteristic.writeValue(bluetooth_buffer); 
                    
                    //inch_forward(DEFAULT_SPEED + velocity_correction_L, 90);
                    turn_right(TURN_SPEED, &line_data, &velocity_correction);
                    turn_flag = 1;
                    
                    CURRENT_STATE = F_STATE;
                    break;
                 case U_STATE:
                    bluetooth_buffer &= 0;    //reset buffer value  
                    mouseDataCharacteristic.writeValue(bluetooth_buffer);

                    turn_left(TURN_SPEED, &line_data, &velocity_correction); 
                    turn_flag = 1;

                    CURRENT_STATE = F_STATE;
                    break;
                 case END_STATE:
                    CURRENT_STATE = IDLE_STATE;
                    break;
                 case CAL_STATE:
                    bluetooth_buffer &= 0;    //reset buffer value  
                    mouseDataCharacteristic.writeValue(bluetooth_buffer);
                    
                    PLAY_SOUND(calibrate_begin_sound, calibrate_begin_delay);
                    calibrate_adc(adc_threshhold); 
                    PLAY_SOUND(calibrate_end_sound, calibrate_end_delay);
                    
                    CURRENT_STATE = IDLE_STATE;
                    break;
                 
            }

        }

        // when the central disconnects, print it out:
        //Serial.print(F("Disconnected from central: "));
        //Serial.println(central.address());
    }
  
  
  
}

//THREAD1: Continuosly updates the line_data
void t1_func(){
    while(1){
        read_line(&line_data);
    }
}

//THREAD2: Continuously regulates the velocity of the wheel by adjusting the voltage based on rotation per 50ms
void t2_func(){
    //Setting up initial value
    int current_rotation = abs(enc2.read());
    int prev_rotation    = enc2.read();
    int rotation_per_ms  = 0;
    
    int error      = 0;
    int prev_error = 0;
    while(1){
        //If turn_state was a left turn, use the right wheel as the constant velocity wheel
        wait_us(10000); //10ms  (10000us)     

        if(turn_flag == 1){
            error      = 0;
            prev_error = 0;
            turn_flag  = 0;
        }
    
        current_rotation = abs(enc2.read());
        
        rotation_per_ms = current_rotation - prev_rotation;
        prev_rotation   = current_rotation;

        error = MAX_CORRECTION - rotation_per_ms;
        prev_error = error;
        int temp = int(error*1.2 + (error-prev_error)*0.3);

        velocity_correction = (temp > MAX_CORRECTION) ? MAX_CORRECTION : temp;
    }

}

/*~~~NOTES~~~
    Centered line value = 13'B0000001000000   //center point
    Centered line value = 13'B0000011100000   //center wide range
    
    Centered line value = 13'B1111000000000   //left point
*/
