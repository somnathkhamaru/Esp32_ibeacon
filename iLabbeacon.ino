
#include "sys/time.h"
#include "BLEDevice.h"
#include "BLEServer.h"
#include "BLEUtils.h"
#include "BLEBeacon.h"
#include "esp_sleep.h"

#define GPIO_DEEP_SLEEP_DURATION    4  // sleep x seconds and then wake up

BLEAdvertising *pAdvertising;
struct timeval now;


#define BEACON_UUID           "b4b5a7af-9be7-09bc-944d-e8d4204b7a1a" 
void setBeacon() {

  BLEBeacon oBeacon = BLEBeacon();
  oBeacon.setManufacturerId(0x4C00); // fake Apple 0x004C LSB (ENDIAN_CHANGE_U16!)
  oBeacon.setProximityUUID(BLEUUID(BEACON_UUID));
  oBeacon.setMajor(0xFFFF0000>> 16);
  oBeacon.setMinor(0xFFFF);
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  BLEAdvertisementData oScanResponseData = BLEAdvertisementData();
  
  oAdvertisementData.setFlags(0x04); // BR_EDR_NOT_SUPPORTED 0x04
  
  std::string strServiceData = "";
  
  strServiceData += (char)26;     // Len
  strServiceData += (char)0xFF;   // Type
  strServiceData += oBeacon.getData(); 
  oAdvertisementData.addData(strServiceData);
  
  pAdvertising->setAdvertisementData(oAdvertisementData);
  pAdvertising->setScanResponseData(oScanResponseData);

}

void setup() {

  Serial.begin(115200);

  Serial.printf("start ESP32 %d\n");
  
  // Create the BLE Device
  BLEDevice::init("");

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();

  pAdvertising = pServer->getAdvertising();
  
  setBeacon();
   // Start advertising
  pAdvertising->start();
  Serial.println("Advertizing started...");
//  delay(100);
//  pAdvertising->stop();
//  Serial.printf("enter deep sleep\n");
//  esp_deep_sleep(1000000LL * GPIO_DEEP_SLEEP_DURATION);
//  Serial.printf("in deep sleep\n");
}

void loop() {
   
}

