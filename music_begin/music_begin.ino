#include "mouse_sound.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  PLAY_SOUND(begin_race_sound, begin_race_delay);
  delay(500);
  PLAY_SOUND(mario_melody, mario_tempo);
}

void loop() {
  // put your main code here, to run repeatedly:

}
