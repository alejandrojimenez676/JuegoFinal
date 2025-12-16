#pragma once
#include <Arduino.h>

// -------- Colores TFT --------
#define BLACK 0x0000
#define WHITE 0xFFFF
#define RED   0xF800
#define GREEN 0x07E0
#define DARK_GREEN 0x03A0
#define YELLOW 0xFFE0
#define ORANGE 0xFD20
#define BLUE  0x001F

// Bloque de la serpiente
extern const uint16_t snakeBodySprite[15*15];  
extern const uint16_t snakeHeadSprite[15*15];  
extern const uint16_t appleSprite[15*15];  

// Bitmap de inicio
extern const unsigned char epd_bitmap_snakeStart[];
