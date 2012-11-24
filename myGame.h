#ifndef GAME
#define GAME


#include<iostream>
#include<map>

class myGame {
  public:
    bool isPaused;
    bool canShoot;
    bool showScores;
    bool isGameOver;
    bool winner;
    
    float playerLife;
    float playerMAXLife;
    
    float enemyLife;
    float enemyMAXLife;
      
    int Score;
    myGame();
    void render();
};
#endif //GAME
