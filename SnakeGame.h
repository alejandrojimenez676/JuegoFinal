
#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>

class SnakeGame {
private:
    TFT_eSPI tft;

    static const int BLOCK = 15;
    static const int MAX_X = 465;
    static const int MAX_Y = 305;
    static const int MIN_Y = 60;
    static const int AD_PIN = 33;

    int dir;
    int snakeX[300];
    int snakeY[300];
    int snakeLen;
    int foodX, foodY;
    bool gameOver;

    unsigned long lastTime;
    const int speedGame = 130;

public:

    SnakeGame() : tft(TFT_eSPI()) {
        dir = 1;
        gameOver = false;
        lastTime = 0;
    }

    void begin() {
        pinMode(AD_PIN, INPUT);

        tft.init();
        tft.setRotation(1);
        tft.fillScreen(TFT_BLACK);

        randomSeed(analogRead(0));

        reiniciarJuego();
    }


    void crearComida() {
        foodX = random(0, MAX_X / BLOCK) * BLOCK;
        foodY = random(MIN_Y / BLOCK, MAX_Y / BLOCK) * BLOCK;
    }

    // ---------- Reiniciar ----------
    void reiniciarJuego() {
        snakeLen = 5;
        dir = 1;
        gameOver = false;

        for (int i = 0; i < snakeLen; i++) {
            snakeX[i] = 150 - i * BLOCK;
            snakeY[i] = 150;
        }

        crearComida();
        tft.fillScreen(TFT_BLACK);
    }

    // ---------- Dibujar UI ----------
    void dibujarUI() {
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.setTextSize(2);
        tft.drawString("SNAKE GAME", 160, 5);
        tft.drawString("PUNTAJE:", 10, 35);
    }

    // ---------- Control ----------
    void leerControles() {
        int adc = analogRead(AD_PIN);

        // Reinicio
        if (gameOver && adc > 3200) {
            reiniciarJuego();
            return;
        }
        if (gameOver) return;

        // Direcciones con tu EXACTA configuración
        if (adc < 300 && dir != 1)       dir = 3; // izquierda
        else if (adc < 900 && dir != 2)  dir = 0; // arriba
        else if (adc < 1600 && dir != 0) dir = 2; // abajo
        else if (adc < 2600 && dir != 3) dir = 1; // derecha
    }

    // ---------- Lógica del juego ----------
    void actualizar() {
        if (millis() - lastTime < speedGame) return;
        lastTime = millis();

        // mover cuerpo
        for (int i = snakeLen - 1; i > 0; i--) {
            snakeX[i] = snakeX[i - 1];
            snakeY[i] = snakeY[i - 1];
        }

        // mover cabeza
        if (dir == 0) snakeY[0] -= BLOCK;
        if (dir == 1) snakeX[0] += BLOCK;
        if (dir == 2) snakeY[0] += BLOCK;
        if (dir == 3) snakeX[0] -= BLOCK;

        // colisión con paredes
        if (snakeX[0] < 0 || snakeX[0] > MAX_X ||
            snakeY[0] < MIN_Y || snakeY[0] > MAX_Y) {
            gameOver = true;
        }

        // colisión consigo misma
        for (int i = 1; i < snakeLen; i++) {
            if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
                gameOver = true;
            }
        }

        // comida
        if (snakeX[0] == foodX && snakeY[0] == foodY) {
            snakeLen++;
            crearComida();
        }
    }

    // ---------- Dibujar ----------
    void dibujar() {
        tft.fillScreen(TFT_BLACK);
        dibujarUI();

        // puntaje
        tft.setCursor(130, 35);
        tft.print(snakeLen - 5);

        // comida
        tft.fillRect(foodX, foodY, BLOCK, BLOCK, TFT_RED);

        // serpiente
        for (int i = 0; i < snakeLen; i++) {
            tft.fillRect(snakeX[i], snakeY[i], BLOCK, BLOCK, TFT_GREEN);
        }

        if (gameOver) {
            tft.setTextColor(TFT_RED);
            tft.setTextSize(3);
            tft.drawString("GAME OVER", 110, 140);

            tft.setTextSize(2);
            tft.drawString("DISPARO = REINICIAR", 80, 180);
        }
    }

    // ---------- Función principal ----------
    void run() {
        leerControles();
        if (!gameOver) actualizar();
        dibujar();
    }
};
