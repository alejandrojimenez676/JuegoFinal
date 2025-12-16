#include "Input.h"

int Input::read() {
    return analogRead(33);
}

int Input::getDirection(int adc, int currentDir) {

    if (adc < 300 && currentDir != 1) return 3;
    if (adc < 900 && currentDir != 2) return 0;
    if (adc < 1600 && currentDir != 0) return 2;
    if (adc < 2600 && currentDir != 3) return 1;

    return currentDir;
}

bool Input::restartButton(int adc) {
    return adc > 2600;
}
