#pragma once
#include <Arduino.h>

class Snake {
public:
    int x[300];
    int y[300];
    int length;
    int dir;  

    void reset(int len, int block);
    void update(int block);
    bool hitsItself();
};

