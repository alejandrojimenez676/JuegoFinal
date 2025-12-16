#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Snake.h"
#include "Food.h"
#include "Input.h"

#define SCREEN_W 480
#define SCREEN_H 320

#define UI_HEIGHT 50

#define BLOCK 15

#define PLAY_X 0
#define PLAY_Y UI_HEIGHT
#define PLAY_W SCREEN_W
#define PLAY_H (SCREEN_H - UI_HEIGHT)

class Game {
public:
    Game(TFT_eSPI &tftRef);
    void begin();
    void run();

private:
    TFT_eSPI &tft;
    Snake snake;
    Food food;
    Input input;

    bool gameOver = false;
    bool gameStarted = false;

    unsigned long lastTime = 0;

    int speedGame = 130;
    int speedMin = 50;                    
    unsigned long lastSpeedIncrease = 0;  
    int speedIncrementInterval = 10000;   

    const int buzzerPin = 32;  
    unsigned long soundStartTime = 0;
    unsigned long soundDuration = 0;
    int soundFrequency = 0;

    void playStartSound();
    void playEatSound();
    void startSound(int freq, unsigned long duration);
    void updateSound();

    const int themeLength = 20;
    int themeNotes[20] = {
        523, 587, 659, 698, 784, 880, 988, 1047, 988, 880,
        784, 698, 659, 587, 523, 659, 784, 880, 784, 659
    };
    int themeDurations[20] = {
        500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
        500, 500, 500, 500, 500, 500, 500, 500, 500, 500
    };
    int currentThemeIndex = 0;
    unsigned long lastNoteTime = 0;
    void updateTheme();


    void reset();
    void update();
    void drawUI();
    void drawBorder();
    void drawGameOver();
    void drawStartScreen();

    int readStableADC(int pin);
    bool isButtonSW5(int adc);
};
