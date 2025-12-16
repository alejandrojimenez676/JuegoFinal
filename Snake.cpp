#include "Snake.h"

void Snake::reset(int len, int block) {
    length = len;
    dir = 1;

    for (int i = 0; i < length; i++) {
        x[i] = 150 - i * block;
        y[i] = 150;
    }
}

void Snake::update(int block) {
    for (int i = length - 1; i > 0; i--) {
        x[i] = x[i - 1];
        y[i] = y[i - 1];
    }

    if (dir == 0) y[0] -= block;
    if (dir == 1) x[0] += block;
    if (dir == 2) y[0] += block;
    if (dir == 3) x[0] -= block;
}

bool Snake::hitsItself() {
    for (int i = 1; i < length; i++) {
        if (x[0] == x[i] && y[0] == y[i]) return true;
    }
    return false;
}
