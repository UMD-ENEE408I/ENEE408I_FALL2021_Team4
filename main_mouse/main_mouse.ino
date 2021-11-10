static uint32_t bluetooth_buffer;
static uint16_t line_data;
static uint8_t  current_state;

#include <ArduinoBLE.h>

#include "mouse_sensor.h"         //includes functions for line sensor
#include "mouse_movement.h"       //includes function for mouse movement
#include "mouse_sound.h"          //includes all audio functions and tones
#include "main_definitions.h"     //RTOS and other stuff

#include "template_functions.h"   //OPTIONAL. includes functions to print out binary

//RTOS stuff
#include <mbed.h>
using namespace mbed;
using namespace rtos;
//Threads. Create a new one for each new function you want to run in parallel
Thread t1;

#define DEFAULT_SPEED 32  //speed of the car
#define TURN_SPEED    38
char str_buf[128];

/*
BLEService motorService("080a43de-6b5e-4004-9c9b-4ac9f67b99be");
// create switch characteristic and allow remote device to read and write
BLEUnsignedIntCharacteristic motorCharacteristic("080a43de-6b5e-4004-9c9b-4ac9f67b99be", BLERead | BLEWrite);
*/

void setup() {
  Serial.begin(115200);

  bluetooth_buffer = 0;   //preset buffer value
  current_state    = 0;
 /* 
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  // Set the connection interval to be as fast as possible (about 40 Hz)
  BLE.setConnectionInterval(0x0006, 0x0006);

  BLE.setLocalName("Emilys Mouse");
  BLE.setAdvertisedService(motorService);
  motorService.addCharacteristic(motorCharacteristic);
  BLE.addService(motorService);

  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  // assign event handlers for characteristic
  //what does this do?
  //motorCharacteristic.setEventHandler(BLEWritten, motorCharacteristicWritten);
  

  BLE.advertise();
  Serial.println("Waiting for connection");
 */

  
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
}

void loop() {
//  E.encoder1 = enc1.read();
//  E.encoder2 = enc2.read(); 
//  motorCharacteristic.writeValue(((int32_t) E.encoder1 << 16) |  ((int32_t) E.encoder2 & 0x0000ffff));//E, sizeof(E));//(
//  BLE.poll();

  //read_line(&line_data);
  printBinaryN(line_data, 13);
  

  uint16_t temp_data = 0;
  temp_data = line_data;
  
  if((temp_data&0b1111000000000) == 0b1111000000000){
          inch_forward(DEFAULT_SPEED, 90);
          turn_left(TURN_SPEED, &line_data);  
          delay(100);
      }
      else if((temp_data&0b0000000001111) == 0b0000000001111){
          inch_forward(DEFAULT_SPEED, 90);
          turn_right(TURN_SPEED, &line_data);
          delay(100);
      }
      else{
          drive_forward(DEFAULT_SPEED, &line_data);
      }

/*
  uint16_t temp_data = 0;  
  switch(current_state){
      case IDLE_STATE:
           drive_stop(0);
              //Add code to recieve bluetooth instruction here
           break;

      case F_STATE:
              drive_forward(DEFAULT_SPEED, &line_data);

              temp_data = line_data;
              if((temp_data&0b1111000001111) == 0b1111000001111){   //Both turns detected
                  drive_stop(0);
                  bluetooth_buffer |= 0b101;
                  
                  read_line(&temp_data);
                  inch_forward(DEFAULT_SPEED, 90);
                  if((temp_data&0b1111111111111) == 0b1111111111111){  //Finish line
                      bluetooth_buffer |= 0b1111;
                  }
                  else if(temp_data & 0b0000011100000)   //Middle path
                      bluetooth_buffer |= 0b010;

                  current_state = IDLE_STATE;
              }
              else if((temp_data&0b0000000001111) == 0b0000000001111){  //Right turn
                  drive_stop(0);
                  bluetooth_buffer |= 0b001;
                  
                  read_line(&temp_data);
                  inch_forward(DEFAULT_SPEED, 90);
                  if(temp_data & 0b0000011100000)   //Middle path
                      bluetooth_buffer |= 0b010;

                  current_state = IDLE_STATE;
              }
              else if((temp_data&0b1111000000000) == 0b1111000000000){
                  drive_stop(0);
                  bluetooth_buffer |= 0b100;
                  
                  read_line(&temp_data);
                  inch_forward(DEFAULT_SPEED, 90);
                  if(temp_data & 0b0000011100000)   //Middle path
                      bluetooth_buffer |= 0b010;

                  current_state = IDLE_STATE;
              }
              else if(temp_data == 0){      //Dead end.
                  drive_stop(0);
                  bluetooth_buffer &= 0xFFFFFFF0
                  
                  current_state = IDLE_STATE;
              }   
           break;
           
      case L_STATE:
           break;

      case R_STATE:
           break;

      case U_STATE:
           break;

      case END_STATE:
           break;
  };
  */

  
  
  
}

void t1_func(){
    while(1){
        read_line(&line_data);
    }
}

/*
void blePeripheralConnectHandler(BLEDevice central) {
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
}

void motorCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  Serial.print("motorCharacteristicWritten: ");
  unsigned long v = motorCharacteristic.value();
  //Serial.print(v);
  //short left = (short)((v>>0) & 0x0000FFFF); // Unpack 16 bit signed value (assume short is 16 bit)
  //short right = (short)((v>>16) & 0x0000FFFF); // Unpack 16 bit signed value
  //Serial.print(left);
  //Serial.print(" ");
  //Serial.println(right);
}
*/
/*~~~NOTES~~~
    Centered line value = 13'B0000001000000   //center point
    Centered line value = 13'B0000011100000   //center wide range
    
    Centered line value = 13'B1111000000000   //left point
*/


//DRIVING STRIGHT and TURN test code
/*
 if((line_data&0b1111000000000) == 0b1111000000000){
          inch_forward(DEFAULT_SPEED, 90);
          turn_left(TURN_SPEED, &line_data);  
          delay(100);
      }
      else if((line_data&0b0000000001111) == 0b0000000001111){
          inch_forward(DEFAULT_SPEED, 90);
          turn_right(TURN_SPEED, &line_data);
          delay(100);
      }
      else{
          drive_forward(DEFAULT_SPEED, &line_data);
      }
 */
