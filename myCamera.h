#ifndef CAMERA
#define CAMERA


#include<stdio.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<vector>

#include "myGame.h"
#include "myVector.h"

using namespace std;


class myCamera {
  public:
    myVector front,right,up; // direction vectors to know the alignment of the Camera
    myVector position; // positiong vector of the camera 
    myVector positionBackup,frontBackup,rightBackup,upBackup;
    bool drawGrid;
    bool drawBoundBox;
    bool showCursor;
    int mode; // 0=(Can FLy anywhere) ; 1=(Ghost Mode with Boundaries) ; 2=Walk Mode;
    int worldWidth,worldLength,worldHeight;
    myGame *G;
    myVector terrainNormals[1300][1300];
    // Camera custom attributes
    float speed_traverse; // the scale factor multiplied when moving in the world (affects the position vector of the camera)
    float speed_look; // the scale factor when looking around the world from a fixed position. (affects the alignment vectors of the camera)

    myCamera(); // Default Constructor
    void stabiliseCamera();// Member function to stabilise the camera
    void initialise();// Initialises the camera to the initial state
    
    void moveForward(float units); // Function to move forward 
    void moveBackward(float units); // Function to move backward
    void strafeLeft(float units); // Function to strafe left
    void strafeRight(float units); // Function to strafe right
    void moveUp(float units);
    void moveDown(float units);
    void render();
    void _drawGrid();
    void _drawBoundBox();

    void lookUp(float units);// Look up 
    void lookDown(float units);// Look Down
    void lookLeft(float units);// Look Left
    void lookRight(float units);// Look Left

    //for textures of BoundBox :)
    int textureWidth;
    int textureLength;
    GLuint loadRAWTexture(unsigned char *data1);
    void loadData(const char *fileName, int width, int height,int location);
    unsigned char *data[2]; // 0=> wall , 1=> roof
    void toObservationMode(vector<myVector> pathTraced);
    vector<myVector> *pathTracedLastParticle;
    void toWalkMode();
};

// myCamera.h ends
//-------------------------------

#endif // CAMERA
