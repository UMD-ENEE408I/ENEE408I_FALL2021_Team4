#include "Arduino.h"
#include "mouse_sound.h"

int calibrate_begin_sound[] = {NOTE_F6, NOTE_A6, NOTE_D7};
int calibrate_begin_delay[] = {100, 100, 100};

int begin_race_sound[] = {NOTE_A5, 0, NOTE_A5, 0, NOTE_A5, 0, NOTE_A6};
int begin_race_delay[] = {800, 300, 800, 300,  800, 300, 800};

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

int calibrate_end_sound[] = {NOTE_D7, NOTE_D7};
int calibrate_end_delay[] = {150, 200};

//IGNORE
void play_sound(int *arr_sound, int *arr_delay, int arr_size){
    for(int i=0; i<arr_size; i++){
        tone(BUZZ_PIN, arr_sound[i], arr_delay[i]);
        delay(arr_delay[i]);
    }
}
