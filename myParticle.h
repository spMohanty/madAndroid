#ifndef PARTICLE
#define PARTICLE

#include<stdio.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<sys/time.h>
#include<vector>

#include "myVector.h"
#include "myTerrain.h"
#include "myGame.h"
#include "myCamera.h"

#define velocityThreshold 0.1
#define MAX_EXPLOSION_RADIUS 45
using namespace std;

class myParticle {
  public:
    int name;
    myVector position;
    myVector velocity;
    myVector g;
    myCamera *C;

    float velocityScaleFactor;
    float accelerationScaleFactor;
    float MAX_VELOCITY;
    float MAX_ACCELERATION;
     
    float radius;
    float coefficientOfRestitution;
    float colorR,colorG,colorB;
    bool isDead;
    bool isPaused;
    bool isActivated;
    vector<myVector> pathTraced;
    // state variables
    
    
    // Explosion parameters
    int noOfBounceBeforeExplosion;
    int noOfBounces;
    
    double explosionStartTime;
    double explosionTotalTime;
      //Explosion parameters end
    double birthTime;
    double lastRenderedTime;
    double timeElapsed;
    bool isExploding;
    // state variables end
    
    myGame *G;
    myTerrain *T;
    // temp variable 
    myVector temp;
    struct timeval t;
    int x,y;
    float X,Y;

    myParticle(myVector P, float r,myTerrain *t1 , myGame *te);
    void render();
    void computeAction();
    void manageCollissionWithTerrain();
    void manageCollissionWithWorldBoundaries();
    void manageCollissionWithOtherParticles();
    void startExplosion();
};

#endif // PARTICLE
