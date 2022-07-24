

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define SERVICE_UUID "6e400001-b5a3-f393-e0a9-e50e24dcca9e"
#define CHARACTERISTIC_UUID_RX "6e400002-b5a3-f393-e0a9-e50e24dcca9e"
#define CHARACTERISTIC_UUID_TX "6e400003-b5a3-f393-e0a9-e50e24dcca9e"

static BLECharacteristic *pCharacteristicTX;
static BLECharacteristic *pCharacteristicRX;

static bool deviceConnected = false;

class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
  }
  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
    // pServer->startAdvertising();
  }
};

class MyCallbacks : public BLECharacteristicCallbacks
{

  void onWrite(BLECharacteristic *pCharacteristic)
  {
    uint8_t *pData;
    std::string value = pCharacteristic->getValue();
    int len = value.length();
    pData = pCharacteristic->getData();

    //        Serial.print("Write callback for characteristic ");
    //        Serial.print(pCharacteristic->getUUID().toString().c_str());
    //        Serial.print(" of data length ");
    //        Serial.println(len);
    //        Serial.print("RX  ");
    //        for (int i = 0; i < len; i++) {
    //          Serial.printf("%02X ", pData[i]);
    //        }
    //        Serial.println();

    if (pData[0] == 0xAB)
    {
      switch (pData[4])
      {
      case 0x93:
        set_time(pData[13], pData[12], pData[11], pData[10], pData[9], pData[7] * 256 + pData[8]);
        break;
      case 0x7C:
        hr24 = pData[6] == 0;
        break;
        case 0x77:
        autoScreen = pData[6] == 1;
        break;
      case 0x72:
        cMsg++;
        isNotify = true; // vibrate motor

        onTime = millis() - 7000; // turn on the screen
        screenOn = true;
        load_screen(3);

        sMsg = cMsg; // show last msg

        title[cMsg % 5] = "Message";

        msg[cMsg % 5][pData[2] - 5] = 0;
        for (int x = 0; x < len - 8; x++)
        {
          msg[cMsg % 5][x] = char(pData[x + 8]);
        }

        break;
      }
    }
    else
    {
      switch (pData[0])
      {
      case 0:
        for (int x = 0; x < len - 1; x++)
        {
          msg[cMsg % 5][x + 12] = char(pData[x + 1]);
        }
        break;
      case 1:
        for (int x = 0; x < len - 1; x++)
        {
          msg[cMsg % 5][x + 31] = char(pData[x + 1]);
        }
        break;
      case 2:
        for (int x = 0; x < len - 1; x++)
        {
          msg[cMsg % 5][x + 50] = char(pData[x + 1]);
        }
        break;
      case 3:
        for (int x = 0; x < len - 1; x++)
        {
          msg[cMsg % 5][x + 69] = char(pData[x + 1]);
        }
        break;
      case 4:
        for (int x = 0; x < len - 1; x++)
        {
          msg[cMsg % 5][x + 88] = char(pData[x + 1]);
        }
        break;
      case 5:
        for (int x = 0; x < len - 1; x++)
        {
          msg[cMsg % 5][x + 107] = char(pData[x + 1]);
        }
        break;
      }
    }
  }
};

void init_BLE()
{
  BLEDevice::init("DT78 ESP32");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristicTX = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);
  pCharacteristicRX = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR);
  pCharacteristicRX->setCallbacks(new MyCallbacks());
  pCharacteristicTX->setCallbacks(new MyCallbacks());
  pCharacteristicTX->addDescriptor(new BLE2902());
  pCharacteristicTX->setNotifyProperty(true);
  pService->start();

  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  macAddr = BLEDevice::getAddress().toString().c_str();

}