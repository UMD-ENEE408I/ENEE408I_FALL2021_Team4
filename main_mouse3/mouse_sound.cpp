#include "Arduino.h"
#include "mouse_sound.h"
#include <mbed.h>


int calibrate_begin_sound[] = {NOTE_F6, NOTE_A6, NOTE_D7};
int calibrate_begin_delay[] = {100, 100, 100};

int calibrate_end_sound[] = {NOTE_D7, NOTE_D7};
int calibrate_end_delay[] = {150, 200};

int victory_sound[] = {523, 523, 523, 523, 415, 466, 523, 0, 466, 523};
int victory_delay[] = {133, 133, 133, 400, 400, 400, 133, 133, 133, 1200};

int mario_melody[] = {
  NOTE_E6, NOTE_E6, 0, NOTE_E6,
  0, NOTE_C6, NOTE_E6, 0,
  NOTE_G6, 0, 0,  0,
  NOTE_G5, 0, 0, 0,

  NOTE_C6, 0, 0, NOTE_G5,
  0, 0, NOTE_E5, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,
 
  NOTE_G5, NOTE_E6, NOTE_G6,
  NOTE_A7, 0, NOTE_F6, NOTE_G6,
  0, NOTE_E6, 0, NOTE_C6,
  NOTE_D6, NOTE_B6, 0, 0,
 
  NOTE_C6, 0, 0, NOTE_G5,
  0, 0, NOTE_E5, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,
 
  NOTE_G5, NOTE_E6, NOTE_G6,
  NOTE_A7, 0, NOTE_F6, NOTE_G6,
  0, NOTE_E6, 0, NOTE_C6,
  NOTE_D6, NOTE_B6, 0, 0,

  0, 0, NOTE_G6, NOTE_FS6, 
  NOTE_F6, NOTE_DS6, 0, NOTE_E6, 0,
  NOTE_GS5, NOTE_A6, NOTE_C6, 0,

  NOTE_A6, NOTE_C6, NOTE_D6,

  0, 0, NOTE_G6, NOTE_FS6, 
  NOTE_F6, NOTE_DS6, 0, NOTE_E6, 0,
  NOTE_C7, 0, NOTE_C7, NOTE_C7, 0, 0, 

  0, 0, NOTE_G6, NOTE_FS6, 
  NOTE_F6, NOTE_DS6, 0, NOTE_E6, 0,
  NOTE_GS5, NOTE_A6, NOTE_C6, 0,

  NOTE_A6, NOTE_C6, NOTE_D6,

  0, 0, NOTE_DS6, 0, 0, NOTE_D6, 0, 0, NOTE_C6, 0

};
//Mario main them tempo
int mario_tempo[] = {
  150, 150, 150, 150,
  150, 150, 150, 150,
  150, 150, 150, 150,
  150, 150, 150, 150,

  150, 150, 150, 150,
  150, 150, 150, 150,
  150, 150, 150, 150,
  150, 150, 150, 150,

  135, 135, 135,
  150, 150, 150, 150,
  150, 150, 150, 150,
  150, 150, 150, 150,

  150, 150, 150, 150,
  150, 150, 150, 150,
  150, 150, 150, 150,
  150, 150, 150, 150,

  135, 135, 135,
  150, 150, 150, 150,
  150, 150, 150, 150,
  150, 150, 150, 150,

  150, 150, 150, 150,
  150, 150, 150, 150,
  150, 150, 150, 150,
  150, 150, 150, 150,

  150, 150, 150, 150,
  150, 150, 150, 150,
  150, 150, 150, 150,
  150, 150, 150,

  150, 150, 150, 150,
  150, 150, 150, 150,
  150, 150, 150, 150,
  150, 150, 150, 150,

  150, 150, 150, 150,
  150, 150, 150, 150,
  150, 150
  
};


//IGNORE
void play_sound(int *arr_sound, int *arr_delay, int arr_size){
    for(int i=0; i<arr_size; i++){
        tone(BUZZ_PIN, arr_sound[i], arr_delay[i]);
        delay(arr_delay[i]);
    }
}
