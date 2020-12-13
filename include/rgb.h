#ifndef RGB_H_
#define RGB_H_

#include <Arduino.h>

class RGB
{
public:
    // Constructor
    RGB(uint8_t R, uint8_t G, uint8_t B);

    // properties
    uint8_t _red;
    uint8_t _green;
    uint8_t _blue;


    // constants colors
    static const RGB RED;
    static const RGB GREEN;
    static const RGB BLUE;
    static const RGB WHITE;
    static const RGB BLACK;
    static const RGB PURPLE;
    static const RGB MAROON;
    static const RGB LIME;
    static const RGB AQUA;
    static const RGB YELLOW;
    static const RGB MAGENTA;
    static const RGB NAVY;
    static const RGB OLIVE;
    static const RGB TEAL;
    static const RGB GREY;
 
};


#endif