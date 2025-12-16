#pragma once
#include <Arduino.h>


class Snake;

class Food {
public:
    int x, y;


    void create(int maxX, int maxY, int block, int minY, const Snake &snake);
};
