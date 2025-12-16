#include "Food.h"
#include "Snake.h"   

void Food::create(int maxX, int maxY, int block, int minY, const Snake &snake) {
    bool valid = false;

    while (!valid) {
 
        x = random(0, maxX / block) * block;
        y = random(minY / block, maxY / block) * block;

        valid = true;

        
        for (int i = 0; i < snake.length; i++) {
            if (snake.x[i] == x && snake.y[i] == y) {
                valid = false;
                break;
            }
        }
    }
}
