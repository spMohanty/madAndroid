#include "myGame.h"


myGame::myGame(){
  isPaused = false;
  canShoot = true;
  showScores = true;

  playerLife = 100;
  playerMAXLife = 100;

  enemyLife = 100;
  enemyMAXLife = 100;


  isGameOver = false;
  Score = 0;
}

void myGame::render(){
  if(playerLife<=0)
   {
     isPaused = true;
     canShoot = false;
    
     isGameOver = true;
     winner = false;
   }
  if(enemyLife<=0)
   {
     isPaused = true;
     canShoot = false;
     isGameOver = true;
     winner = true;
   }
}
