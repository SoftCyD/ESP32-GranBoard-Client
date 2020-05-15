
#include "boardled.h"
#include <Arduino.h>
#include "BLERemoteCharacteristic.h"
#include "rgb.h"

PROGMEM String notifs_keys[84] = {"4.0@", "8.0@", "3.3@", "3.4@", "3.5@", "3.6@", "2.3@", "2.4@", "2.5@", "2.6@",
                                  "1.2@", "1.4@", "1.5@", "1.6@", "0.1@", "0.3@", "0.5@", "0.6@", "0.0@", "0.2@",
                                  "0.4@", "4.5@", "1.0@", "1.1@", "1.3@", "4.4@", "2.0@", "2.1@", "2.2@", "4.3@",
                                  "3.0@", "3.1@", "3.2@", "4.2@", "9.1@", "9.0@", "9.2@", "8.2@", "10.1@", "10.0@",
                                  "10.2@", "8.3@", "7.1@", "7.0@", "7.2@", "8.4@", "6.1@", "6.0@", "6.3@", "8.5@",
                                  "11.1@", "11.2@", "11.4@", "8.6@", "11.0@", "11.3@", "11.5@", "11.6@", "6.2@", "6.4@",
                                  "6.5@", "6.6@", "7.3@", "7.4@", "7.5@", "7.6@", "10.3@", "10.4@", "10.5@", "10.6@",
                                  "9.3@", "9.4@", "9.5@", "9.6@", "5.0@", "5.3@", "5.5@", "5.6@", "5.1@", "5.2@", "5.4@", "4.6@", "BTN@", "OUT@"};

PROGMEM String notifs_values[84] = {"DB", "SB", "S20", "T20", "S20", "D20", "S1", "T1", "S1", "D1",
                                    "S18", "T18", "S18", "D18", "S4", "T4", "S4", "D4", "S13", "T13",
                                    "S13", "D13", "S6", "T6", "S6", "D6", "S10", "T10", "S10", "D10",
                                    "S15", "T15", "S15", "D15", "S2", "T2", "S2", "D2", "S17", "T17",
                                    "S17", "D17", "S3", "T3", "S3", "D3", "S19", "T19", "S19", "D19",
                                    "S7", "T7", "S7", "D7", "S16", "T16", "S16", "D16", "S8", "T8",
                                    "S8", "D8", "S11", "T11", "S11", "D11", "S14", "T14", "S14", "D14",
                                    "S9", "T9", "S9", "D9", "S12", "T12", "S12", "D12", "S5", "T5", "S5", "D5", "BTN", "OUT"};

PROGMEM uint8_t ledpos[21] = {0,
                              28, 49, 55, 34, 22,
                              40, 1, 7, 16, 43,
                              10, 19, 37, 13, 46,
                              4, 52, 31, 58, 25};

/*
uint8_t data_anim_yellow[16] = {0x11, 0x7D, 0x00, 0x00, 0x7D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x05, 0x00, 0x00, 0x01};
*/



/*!
 *  @brief  Instantiates a new BoardLEd class
 *  @param  pWriteCharacteristic
 *          the BLE characteristic to write data
 */
BoardLed::BoardLed(BLERemoteCharacteristic *pWriteCharacteristic)
{
    _pWriteCharacteristic = pWriteCharacteristic;
}

/*!
 *  @brief  Switch On LED indicated by targetValue
 *  @param  targetValue
 *          The target : eg: S20 T19 DB 
 *  @param  ledColor
 *          The color of LED to be switched on
 */
void BoardLed::showHitValue(String targetValue, RGB ledColor)
{

    uint8_t dataled[16] = {0x01, ledColor._red, ledColor._green, ledColor._blue, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x14, 0x00, 0x00, 0x00};

    char mult = targetValue.charAt(0);

    int value = targetValue.substring(1).toInt();
    uint8_t cell = ledpos[value];
    dataled[10] = cell;

    if (mult == 'D')
        dataled[0] = 0x02;
    else if (mult == 'T')
        dataled[0] = 0x03;
    else
        dataled[0] = 0x01;

    sendDataLED(dataled, sizeof(dataled));
}

/*!
* @brief Send data to GranBoard
* @param dataoutputled
*        The array of data to send to GranBoard
* @param length
*        The size of the array
*/
void BoardLed::sendDataLED(uint8_t *dataoutputled, size_t length)
{
    _pWriteCharacteristic->writeValue(dataoutputled, length, false);
}

/*!
* @brief Switch on a led number (1 to 20)
* @param aLED
*        The struct with number and color of the led
*        NB: 
*        Struct LEDd is not well reverse-engineered yet
*        Not used in main.cpp
*/
void BoardLed::lightLED(LED aLED)
{
    uint8_t t_number[20] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    t_number[aLED.number - 1] = aLED.color;
    sendDataLED(t_number, sizeof(t_number));
}

/*!
* @brief Clear all leds on board
*/
void BoardLed::clear()
{
    uint8_t t_number[20] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    sendDataLED(t_number, sizeof(t_number));
}

/*!
* @brief Swith on all leds with the same color. Never switch off.
* @param ringColor
*        The RGB color of the ring
*/
void BoardLed::lightRing(RGB ringColor)
{
    uint8_t dataRing[] = {0x14, ringColor._red, ringColor._green, ringColor._blue, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x01};
    sendDataLED(dataRing, sizeof(dataRing));
}


/*!
* @brief Blink all board with the same color when Bull is hitted. Swith off after a delay
* @param ringColor
*        The RGB color of the ring
*/
void BoardLed::blinkBull(RGB bullColor)
{
    uint8_t dataBull[16] = {0x17, bullColor._red, bullColor._green, bullColor._blue, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x01};
    sendDataLED(dataBull, sizeof(dataBull));
}

/*!
* @brief Blink all board with the same color when OUT is hitted. Swith off after a delay
* @param ringColor
*        The RGB color of the ring
*/
void BoardLed::blinkOut(RGB color)
{
    uint8_t dataLedOut[16] = {0x17, color._red, color._green, color._blue, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x01};
    sendDataLED(dataLedOut, sizeof(dataLedOut));
}

/*!
* @brief Show led animation when Buton on board is pressed
* @param colorUp
*        The RGB color of the animation up
* @param colorDown
*        The RGB color of the animation down
*/
void BoardLed::pressButton(RGB colorUp, RGB colorDown)
{
    uint8_t dataButton[16] = {0x11, colorUp._red, colorUp._green, colorUp._blue, colorDown._red, colorDown._green, colorDown._blue, 0x00, 0x00, 0x00, 0x10, 0x00, 0x05, 0x00, 0x00, 0x00};
    sendDataLED(dataButton, sizeof(dataButton));
}

/*!
* @brief convert notification send by the board to a readable value (S1 D2 T3 SB DB ...)
* @param rcvNotif
*        the notification send by the board
* @return the value of the target hitted
*/
String BoardLed::convertNotifToValue(String rcvNotif)
{
    for (int i = 0; i < sizeof(notifs_keys); i++)
    {
        if (notifs_keys[i] == rcvNotif)
        {
            return notifs_values[i];
            break;
        }
    }
    return "";
}
