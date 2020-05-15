
/*
 *
 */
#ifndef BOARDLED_H_
#define BOARDLED_H_

#include <Arduino.h>
#include "BLERemoteCharacteristic.h"
#include "rgb.h"

class BoardLed
{

public:
  typedef enum
  {
    LED_RED = 1,
    LED_YELLOW = 2,
    LED_GREEN = 4,
    LED_MAGENTA = 5,
    LED_PURPLE = 6,
    LED_WHITE = 7
  } colorLED;

  typedef struct
  {
    int number;
    colorLED color;
  } LED;

  //RGB PURPLE(0x7D, 0x00, 0x7D);

  // Constructor
  BoardLed(BLERemoteCharacteristic *pWriteCharacteristic);

  // functions
  void showHitValue(String targetValue, RGB color);
  void lightLED(LED laLED);
  void clear();
  void lightRing(RGB ringColor);
  void blinkBull(RGB bullColor);
  void blinkOut(RGB color); // uint8_t red, uint8_t green, uint8_t blue);
  void pressButton(RGB colorUp, RGB colorDown);
  String convertNotifToValue(String rcvNotif);
private:
  BLERemoteCharacteristic *_pWriteCharacteristic;
  void sendDataLED(uint8_t *dataoutputled, size_t length);
};

#endif