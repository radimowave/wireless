#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
BLEDescriptor* pDescriptor = NULL;

const int analogPin = 36; // Analog pin connected to your sensor

bool deviceConnected = false;


#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


/***********ServerCallback********************/
class ServerCallback :public BLEServerCallbacks {
  void onConnect(BLEServer* pServer){
    deviceConnected = true;
    Serial.println("Connected to MetaRadiator");
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("Disconnected from MetaRadiator");
  }
};

void setup() {
  // Create the BLE Device
  BLEDevice::init("meta-ESP32-KKU");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallback());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);
 



   // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                      //BLECharacteristic::VALUE_LENGTH_STATIC       //ใส่เพิ่มเข้ามา
                    );

  // Create a BLE Descriptor
  pDescriptor = new BLEDescriptor("2902");
  pCharacteristic->addDescriptor(pDescriptor);

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();

  Serial.begin(115200);
  Serial.println("BLE device active, waiting for connections...");
}

void loop() {
  if (deviceConnected) {
    int sensorValue = analogRead(analogPin);
    Serial.println(sensorValue/20);

    // Send the sensor data as a notification
    pCharacteristic->setValue((uint8_t*)&sensorValue, sizeof(int));
    pCharacteristic->notify();
  }

  // Add your own logic here

  delay(1000); // Change the delay as per your desired update rate
}



