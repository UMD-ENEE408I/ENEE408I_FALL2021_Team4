#include <ArduinoBLE.h>

#define DEBUG true

// These UUIDs have been randomly generated. - they must match between the Central and Peripheral devices
// Any changes you make here must be suitably made in the Python program as well
BLEService nanoService("13012F00-F8C3-4F4A-A8F4-15CD926DA146"); // BLE Service

// BLE Characteristics - custom 128-bit UUID, read and writable by central device
BLEByteCharacteristic instrCharacteristic("13012F01-F8C3-4F4A-A8F4-15CD926DA146", BLERead | BLEWrite);
BLEUnsignedIntCharacteristic mouseDataCharacteristic("13012F02-F8C3-4F4A-A8F4-15CD926DA146", BLERead | BLEWrite);

void setup() {
    Serial.begin(9600);
    
    if (DEBUG) {
      while(!Serial);
    }

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

void loop() {
    // listen for BLE centrals to connect:
    BLEDevice central = BLE.central();

    // if a central is connected to peripheral:
    if (central) {
        Serial.print("Connected to central: ");
        Serial.println(central.address());


        // while the central is still connected to peripheral:
        while (central.connected()) {
            // if the remote device wrote to the characteristic,
            if (instrCharacteristic.written()) {
                uint8_t CURRENT_STATE = 0b1111&instrCharacteristic.value();;
                //printBinary(instrCharacteristic.value());
                Serial.println(CURRENT_STATE);
                /*
                ADD STATES HERE
                */
            } 
        }


        // when the central disconnects, print it out:
        Serial.print(F("Disconnected from central: "));
        Serial.println(central.address());
    }
}


template<typename T>        //prints out all the binary bits of a variable
void printBinary(T value){
    int num_bits = sizeof(value)*8;
    for(int i=0; i<num_bits; i++){
        Serial.print((value & (1<<num_bits-i-1)) ? "1" : "0");
    } Serial.println();
}
