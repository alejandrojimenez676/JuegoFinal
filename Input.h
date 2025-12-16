#pragma once
#include <Arduino.h>

class Input {
public:
    int read();
    int getDirection(int adc, int currentDir);
    bool restartButton(int adc);
};
