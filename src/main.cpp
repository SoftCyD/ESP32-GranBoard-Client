/*
 * ESP32 GranBoard Client 
 * 
 * - 15/05/2020 - Soft.Cyd@gmail.com - First version
 * - 13/12/2020 - Soft.Cyd@gmail.com - Update with scan and without address
 * 
 */

// Some links
// https://esp32.com/viewtopic.php?t=4181

#include <Arduino.h>
#include "BLEDevice.h"
#include "boardled.h"
#include "rgb.h"

 //declare reset function at address 0
void (*resetFunc)(void) = 0;

// remote service
static BLEUUID serviceUUID("442f1570-8a00-9a28-cbe1-e1d4212d53eb");
// characteristic of remote service
static BLEUUID readUUID("442f1571-8a00-9a28-cbe1-e1d4212d53eb");
static BLEUUID writeUUID("442f1572-8a00-9a28-cbe1-e1d4212d53eb");
static BLERemoteCharacteristic *pReadCharacteristic;
static BLERemoteCharacteristic *pWriteCharacteristic;

static BLEAddress *pGranBoardAddress;
static boolean doConnect = false;
static boolean connected = false;

String ledTarget = "";
bool ledOut = false;
bool ledBull = false;
bool ledButton = false;

BoardLed *granboard;

class MyClientCallback : public BLEClientCallbacks
{
  void onConnect(BLEClient *pclient)
  {
    connected = true;
  }

  void onDisconnect(BLEClient *pclient)
  {
    connected = false;
    Serial.println("<KO>");
    resetFunc(); //call reset
    return;
  }
};

static void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify)
{
  if (length > 0)
  {
    pData[length] = 0;
    String rcvNotif = String((char *)pData);

    if (pData[length - 1] == '@')
    {
      String mult_value = granboard->convertNotifToValue(rcvNotif);

      if (mult_value != "")
      {
        // Send to Serial to Be Used by Other App
        Serial.println("<" + mult_value + ">");

        // Out detected
        if (mult_value == "OUT")
        {
          ledOut = true;
          return;
        }

        // Button pressed
        if (mult_value == "BTN") {
          ledButton = true;
          return;
        }

        // Bull's Eye Hitted
        if (mult_value.endsWith("B"))
        {
          ledBull = true;
          return;
        }

        // else show target hitted
        ledTarget = mult_value;
        return;
      }
    }
  }
}

bool connectToServer(BLEAddress pAddress)
{
    BLEClient*  pClient  = BLEDevice::createClient();
    pClient->connect(pAddress, BLE_ADDR_TYPE_RANDOM);
  pClient->setClientCallbacks(new MyClientCallback());

  BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr)
  {
    pClient->disconnect();
    return false;
  }

  pReadCharacteristic = pRemoteService->getCharacteristic(readUUID);
  if (pReadCharacteristic == nullptr)
  {
    pClient->disconnect();
    return false;
  }

  if (pReadCharacteristic->canNotify())
    pReadCharacteristic->registerForNotify(notifyCallback);

  pWriteCharacteristic = pRemoteService->getCharacteristic(writeUUID);
  if (pWriteCharacteristic == nullptr)
  {
    pClient->disconnect();
    return false;
  }

  granboard = new BoardLed(pWriteCharacteristic);

  connected = true;
  return true;
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {

  void onResult(BLEAdvertisedDevice advertisedDevice) {

    if (advertisedDevice.haveServiceUUID() && advertisedDevice.getServiceUUID().equals(serviceUUID)) {
      advertisedDevice.getScan()->stop();
      pGranBoardAddress = new BLEAddress(advertisedDevice.getAddress());
      doConnect = true;
    }    
  }
};


void setup() {
  Serial.begin(115200);
  BLEDevice::init("");

  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(30);
} 


void loop() {
  if (doConnect == true) {
    if (connectToServer(*pGranBoardAddress)) {
      granboard->lightRing(RGB::AQUA);
      delay(2000);
      granboard->clear();
      connected = true;
    } else {
      resetFunc();
    }
    doConnect = false;
  }

  if (connected) {
    // all actions when conected
    if (ledTarget != "")
    {
      granboard->showHitValue(ledTarget, RGB::BLUE);
      ledTarget = "";
    }
    if (ledBull == true)
    {
      granboard->blinkBull(RGB::BLUE); 
      ledBull = false;
    }
    if (ledOut == true)
    {
      granboard->blinkOut(RGB::PURPLE);
      ledOut = false;
    }
    if (ledButton == true) {
      granboard->pressButton(RGB::MAROON, RGB::GREEN);
      ledButton = false;
    }
  
  }
  delay(5);
} 