#ifndef SCREEN
#define SCREEN

#include<iostream>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<sstream>
#include<string>

#include "myVector.h"
#include "myCamera.h"
#include "myScreen.h"
#include "myGame.h"


#define mySystemSize 100

using namespace std;

class myScreen{
  public:
     myCamera *C;    
     myVector V;
     myGame *G;
     

     myScreen();
     void set2DScreen();
     void unset2DScreen();
     myVector getScaledCoordinates(float x, float y);
     void render(float score,float level);
     void _observationModeScreen();
     void printText(int x,int y,string String,float colorR,float colorG, float colorB);
     void _showScores(float score,float level);
};

#endif //SCREEN
