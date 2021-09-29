#include <ArduinoBLE.h>

#define MOUSE1_UUID "8d527f4b-29f7-4946-b03f-2cd7ee85bcce"

BLEService motorService(MOUSE1_UUID);
BLEUnsignedIntCharacteristic MOUSE1(MOUSE1_UUID, BLERead | BLEWrite);

void setup() {
  Serial.begin(115200);


  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  // Set the connection interval to be as fast as possible (about 40 Hz)
  BLE.setConnectionInterval(0x0006, 0x0006);

  BLE.setLocalName("Tin_Mouse");
  BLE.setAdvertisedService(motorService);
  motorService.addCharacteristic(MOUSE1);
  BLE.addService(motorService);

  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  // assign event handlers for characteristic
  MOUSE1.setEventHandler(BLEWritten, mouse1FunctionCall);
  MOUSE1.setValue(0);

  BLE.advertise();
  Serial.println("Waiting for connection");
}

void loop() {
  BLE.poll();
}

void blePeripheralConnectHandler(BLEDevice central) {
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
}

void mouse1FunctionCall(BLEDevice central, BLECharacteristic characteristic) {
  Serial.print("mouse1FunctionCall: ");
  
  //serial_buffer will hold that value that will be sent and recieved through BlueTooth
  uint8_t serial_buffer = MOUSE1.value();

  Serial.println(serial_buffer);
}
