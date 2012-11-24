#ifndef ENEMY
#define ENEMY


#include<iostream>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<map>

#define PI 3.14159265


#include "myCamera.h"
#include "myTerrain.h"
#include "myParticle.h"
#include "myGame.h"


using namespace std;

class myEnemy{
  public:
    myCamera C;
    myCamera *playerCamera;
    myTerrain *T;
    myGame *G;
    float width;
    float length;
    float height;
    float radiusOfLeg;
    float armRadius;
    float eyeRadius;
    float bodyHeight;
    float life;

    int name;

    bool isDead;
    //temp vars
    myVector V;
    float angle;
    
    map<int, myParticle*> particles;

    map<int, myParticle*> *opponentParticles;
    int pCount;

    myEnemy(myTerrain *t);
    void render();
    void _drawEnemy();
    void computeAction();
//    void handleCollissionWithEnemy();
    bool isColliding(myVector pos);
    bool inExplosionRange(myVector pos,float Range);
};

#endif //ENEMY
