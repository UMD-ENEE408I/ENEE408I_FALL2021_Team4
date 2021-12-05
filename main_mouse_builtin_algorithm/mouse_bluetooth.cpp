#include "Arduino.h"
#include "mouse_bluetooth.h"

// These UUIDs have been randomly generated. - they must match between the Central and Peripheral devices
// Any changes you make here must be suitably made in the Python program as well
BLEService nanoService("13012F00-F8C3-4F4A-A8F4-15CD926DA146"); // BLE Service

// BLE Characteristics - custom 128-bit UUID, read and writable by central device
BLEByteCharacteristic instrCharacteristic("13012F01-F8C3-4F4A-A8F4-15CD926DA146", BLERead | BLEWrite);
BLEUnsignedIntCharacteristic mouseDataCharacteristic("13012F02-F8C3-4F4A-A8F4-15CD926DA146", BLERead | BLEWrite);

void setupBLE(){
    // begin initialization
    if (!BLE.begin()) {
        Serial.println("Starting BLE failed!");
        while (1);
    }

    // set advertised local name and service UUID:
    BLE.setLocalName("Tin_Mouse");
    BLE.setAdvertisedService(nanoService);

    // add the characteristic to the service
    nanoService.addCharacteristic(instrCharacteristic);
    nanoService.addCharacteristic(mouseDataCharacteristic);
    
    // add service
    BLE.addService(nanoService);

    // set the initial value for the characeristic:
    instrCharacteristic.writeValue(0);
    mouseDataCharacteristic.writeValue(0);

    // start advertising
    BLE.advertise();
    delay(100);
    Serial.println("Arduino Nano BLE LED Peripheral Service Started");  
}
