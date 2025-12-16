#include "Game.h"
#include "Sprites.h"

// ---------- DEBOUNCE ----------
static unsigned long lastButtonTime = 0;
static const unsigned long buttonDelay = 250;

Game::Game(TFT_eSPI &tftRef) : tft(tftRef) {}

void Game::begin() {
    tft.init();
    tft.setRotation(1);   // 480x320
    pinMode(33, INPUT);
    pinMode(buzzerPin, OUTPUT);
    randomSeed(analogRead(0));

    drawStartScreen();
    playStartSound();
    gameStarted = false;
}

void Game::drawStartScreen() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_GREEN);
    tft.setTextSize(3);
    tft.drawString("SNAKE GAME", 140, 20);
    int x = (SCREEN_W - 80) / 2;
    int y = 80;
    tft.drawBitmap(x, y, epd_bitmap_snakeStart, 80, 80, TFT_GREEN);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.drawString("PRESIONE DISPARO", 130, 190);
}

void Game::drawUI() {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.drawString("SNAKE GAME", 160, 5);
    tft.drawString("PUNTAJE:", 10, 20);
}

void Game::drawBorder() {
    tft.drawRect(PLAY_X, PLAY_Y, PLAY_W, PLAY_H, TFT_WHITE);
}

void Game::reset() {
    tft.fillScreen(TFT_BLACK);
    drawUI();
    drawBorder();

    snake.reset(5, BLOCK);
    speedGame = 130;               // velocidad inicial
    lastSpeedIncrease = millis();

    food.create(PLAY_X + PLAY_W - BLOCK, PLAY_Y + PLAY_H - BLOCK, BLOCK, PLAY_Y + BLOCK, snake);

    gameOver = false;
    lastTime = millis();

    tft.pushImage(food.x, food.y, BLOCK, BLOCK, appleSprite);
    tft.pushImage(snake.x[0], snake.y[0], BLOCK, BLOCK, snakeHeadSprite);
}

void Game::update() {
    int adc = readStableADC(33);

    if (millis() - lastSpeedIncrease > speedIncrementInterval && speedGame > speedMin) {
        speedGame -= 2;  
        lastSpeedIncrease = millis();
    }

    if (millis() - lastTime < speedGame) return;
    lastTime = millis();

    snake.dir = input.getDirection(adc, snake.dir);

    int tailX = snake.x[snake.length - 1];
    int tailY = snake.y[snake.length - 1];

    snake.update(BLOCK);

    if (snake.x[0] < PLAY_X || snake.x[0] > PLAY_X + PLAY_W - BLOCK || snake.y[0] < PLAY_Y || snake.y[0] > PLAY_Y + PLAY_H - BLOCK || snake.hitsItself()) {
        gameOver = true;
        return;
    }

    if (snake.x[0] == food.x && snake.y[0] == food.y) {
        snake.x[snake.length] = tailX;
        snake.y[snake.length] = tailY;
        snake.length++;

        food.create(PLAY_X + PLAY_W - BLOCK, PLAY_Y + PLAY_H - BLOCK, BLOCK, PLAY_Y + BLOCK, snake);
        tft.pushImage(food.x, food.y, BLOCK, BLOCK, appleSprite);
        playEatSound();

        if (snake.length % 2 == 0 && speedGame > speedMin) speedGame -= 1; // más gradual
    } else {
        tft.fillRect(tailX, tailY, BLOCK, BLOCK, TFT_BLACK);
        drawBorder();
    }

    tft.pushImage(snake.x[0], snake.y[0], BLOCK, BLOCK, snakeHeadSprite);
    for (int i = 1; i < snake.length; i++) {
        tft.pushImage(snake.x[i], snake.y[i], BLOCK, BLOCK, snakeBodySprite);
    }

    tft.fillRect(130, 20, 40, 20, TFT_BLACK);
    tft.setCursor(130, 20);
    tft.print(snake.length - 5);
}

void Game::drawGameOver() {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setTextSize(3);
    tft.drawString("GAME OVER", 150, 140);
    tft.setTextSize(2);
    tft.drawString("DISPARO = REINICIAR", 110, 190);
}

void Game::run() {
    updateSound();
    updateTheme();

    int adc = readStableADC(33);

    if (!gameStarted) {
        if (isButtonSW5(adc)) {
            gameStarted = true;
            noTone(buzzerPin);
            reset();
        }
        return;
    }

    if (gameOver) {
        drawGameOver();
        if (isButtonSW5(adc)) reset();
        return;
    }

    update();
}

int Game::readStableADC(int pin) {
    int total = 0;
    for (int i = 0; i < 20; i++) {
        total += analogRead(pin);
        delay(2);
    }
    return total / 20;
}

bool Game::isButtonSW5(int adc) {
    static bool lastState = false;
    bool pressed = (adc > 2700 && adc < 3100);
    unsigned long now = millis();

    if (pressed != lastState && now - lastButtonTime > buttonDelay) {
        lastButtonTime = now;
        lastState = pressed;
        if (pressed) return true;
    }
    return false;
}

// ----------------- Sonido -----------------
void Game::startSound(int freq, unsigned long duration) {
    soundFrequency = freq;
    soundDuration = duration;
    soundStartTime = millis();
    tone(buzzerPin, freq);
}

void Game::updateSound() {
    if (soundDuration > 0 && millis() - soundStartTime >= soundDuration) {
        noTone(buzzerPin);
        soundDuration = 0;
    }
}

void Game::playStartSound() {
    startSound(1000, 200);
}

void Game::playEatSound() {
    startSound(1500, 100);
}

void Game::updateTheme() {
    if (!gameStarted && millis() - lastNoteTime >= themeDurations[currentThemeIndex]) {
        tone(buzzerPin, themeNotes[currentThemeIndex]);
        lastNoteTime = millis();
        currentThemeIndex++;
        if (currentThemeIndex >= themeLength) currentThemeIndex = 0;
    }
}
