    #include <Arduino.h>

    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"

    #include <TFT_eSPI.h>
    #include "Game.h"


    TFT_eSPI tft = TFT_eSPI();
    Game game(tft);

    void TaskGame(void *pvParameters) {
        while (1) {
            game.run();
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }

    void TaskSound(void *pvParameters) {
        while (1) {
            vTaskDelay(20 / portTICK_PERIOD_MS);
        }
    }

    void TaskSystem(void *pvParameters) {
        while (1) {
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }

    void setup() {
        Serial.begin(115200);

        game.begin();

        xTaskCreate(
            TaskGame,
            "Game Task",
            4096,
            NULL,
            2,
            NULL
        );

        xTaskCreate(
            TaskSound,
            "Sound Task",
            2048,
            NULL,
            1,
            NULL
        );

        xTaskCreate(
            TaskSystem,
            "System Task",
            2048,
            NULL,
            1,
            NULL
        );

        
        vTaskStartScheduler();
    }


    void loop() {
        
    }
