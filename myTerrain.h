#ifndef TERRAIN
#define TERRAIN

#include<iostream>
#include<stdio.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<string.h>


#include "myVector.h"

using namespace std;

class myTerrain {
  public:
    unsigned char heights[1300][1300];
    int width;//x
    int length;// y
    int height; // z
    int heightFactor; // multiplied with the height obtained from RAW data
    int textureWidth;
    int textureLength;
    myVector normals[1300][1300];
    // Constructor
    myTerrain(const char *fileName, int tWidth, int tLength);
          
    unsigned char *data;

    void render(); // renderring function for  our class
    int getHeight(int x, int y);
    GLuint loadRAWTexture();
    void loadData(const char *fileName, int width, int height);
    void computeNormals();
};

#endif // TERRAIN
