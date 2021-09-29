#include "Arduino.h"
#include "mouse_sound.h"

int calibrate_begin_sound[] = {NOTE_F6, NOTE_A6, NOTE_D7};
int calibrate_begin_delay[] = {100, 100, 100};

int calibrate_end_sound[] = {NOTE_D7, NOTE_D7};
int calibrate_end_delay[] = {150, 200};

//IGNORE
void play_sound(int *arr_sound, int *arr_delay, int arr_size){
    for(int i=0; i<arr_size; i++){
        tone(BUZZ_PIN, arr_sound[i], arr_delay[i]);
        delay(arr_delay[i]);
    }
}
