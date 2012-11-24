#ifndef CLICK_TIMER
#define CLICK_TIMER

#include<iostream>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<sys/time.h>

#include "myCamera.h"
#include "myClickTimer.h"
#include "myVector.h"

#define mySystemSize 100

using namespace std;

#define totTime 1000000

class myClickTimer{
  public:
    struct timeval t;

    double startTime;
    double curTime;
    double totalTime;
    double timeElapsed;  
    bool isStopped;
    myCamera *C;
    
    //temp varibles
    float angle;
    //temp variables end
    myClickTimer(myCamera *c);
    float getFractionTimePassed();
    void reset();
    void render();
    myVector getScaledCoordinates(float x,float y);
};




#endif //CLICK_TIMER
