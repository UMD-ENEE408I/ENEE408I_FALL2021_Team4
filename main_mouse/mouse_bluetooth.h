#ifndef MAIN_BLUETOOTH   //This header file is just an extension of the main_mouse file.
#define MAIN_BLUETOOTH

#include <ArduinoBLE.h>

#define DEBUG true
// These UUIDs have been randomly generated. - they must match between the Central and Peripheral devices
// Any changes you make here must be suitably made in the Python program as well
extern BLEService nanoService; // BLE Service
extern BLEByteCharacteristic instrCharacteristic;
extern BLEUnsignedIntCharacteristic mouseDataCharacteristic;

void setupBLE();

#endif
