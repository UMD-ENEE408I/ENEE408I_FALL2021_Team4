#include <ArduinoBLE.h>
#include <Encoder.h>

const unsigned int M1_ENC_A = 6;
const unsigned int M1_ENC_B = 7;
const unsigned int M2_ENC_A = 8;
const unsigned int M2_ENC_B = 9;

Encoder enc1(M1_ENC_A, M1_ENC_B);
Encoder enc2(M2_ENC_B, M2_ENC_A);

struct encoder
  {
       int16_t encoder1;
       int16_t encoder2;
  };

encoder E;
BLEService motorService("080a43de-6b5e-4004-9c9b-4ac9f67b99be");


// create switch characteristic and allow remote device to read and write
BLEUnsignedIntCharacteristic motorCharacteristic("080a43de-6b5e-4004-9c9b-4ac9f67b99be", BLERead | BLEWrite);

void setup() {
  Serial.begin(115200);

  
  
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
}

void loop() {



  E.encoder1 = enc1.read();
  E.encoder2 = enc2.read(); 
  //motorCharacteristic.setValue(E.encoder1); //set the default value
  motorCharacteristic.writeValue(((int32_t) E.encoder1 << 16) |  ((int32_t) E.encoder2 & 0x0000ffff));//E, sizeof(E));//(
  BLE.poll();
  //motorCharacteristic.setValue(E.encoder1);
  //BLE.poll();

}

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
