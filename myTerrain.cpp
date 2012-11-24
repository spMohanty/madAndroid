// myTerrain.cpp starts here

#include "myTerrain.h"
#include "loadRAW.h"

myTerrain::myTerrain(const char *fileName, int tWidth, int tLength){
  heightFactor = 1;
  height = 256 + 256*heightFactor;

  width = tWidth;
  length = tLength;
  RAW r(fileName,width,length);

  int count=0; 
  int x,y;

  for(x=0;x<width;x++)
    for(y=0;y<length;y++)
    {
      heights[x][y]=r.grayScale[x][y]*heightFactor;
    }
  computeNormals();   
  textureWidth=64;
  textureLength=64;
 loadData("rawFiles/floorTexture.raw",textureWidth,textureLength);
}

int myTerrain::getHeight(int x,int y){
  return (int)heights[x][y] ;
}

void myTerrain::render(){


//  glColor3f(0.3f,0.9f,0.0f);
 GLuint texture = loadRAWTexture();
//  GLuint texture = loadRAWTexture("skyImages/rawSky/left.raw",textureWidth,textureHeight);

  glEnable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, texture );

  myVector N;
  for(int y=0;y<length;y++)
  {
    glBegin(GL_TRIANGLE_STRIP);
    for(int x=0;x<width;x++)
    {
      //glTexCoord2f((float)(x%textureWidth)/width, (float)(y%textureLength) / length);
      glTexCoord2f((float)x/(float)textureWidth, (float)y/(float)textureLength);
      glNormal3f(normals[x][y].x,normals[x][y].y,normals[x][y].z);
      glVertex3f(x, y, getHeight(x,y)); 
      //glTexCoord2f((float)(x%textureWidth)/width, (float)((y+1)%textureLength) / length);
      glTexCoord2f((float)x/(float)textureWidth, (float)(y+1)/(float)textureLength);
      glNormal3f(normals[x][y+1].x,normals[x][y+1].y,normals[x][y+1].z);
      glVertex3f(x, y+1, getHeight(x,y+1)); 

      // N=normals[x+1][y+1];
      //    glNormal3f(N.x,N.y,N.z);
      //      glNormal3f(normals[x+1][y+1].x,normals[x+1][y+1].y,normals[x+1][y+1].z);
      //     glVertex3f(x + 1, y+1, getHeight(x+1,y+1));
      //glTexCoord2f((float)(x+1)/width, (float)(y) / length);
      //  N=normals[x+1][y];
      //  glNormal3f(N.x,N.y,N.z);
      // glVertex3f(x+ 1, y, getHeight(x+1,y)); 

    
    }
    glEnd();
  }
/**
  // Debugging
  // Draw normals to ground at specified positions
  myVector p1,p2,p3;
  
  glColor3f(1.0f,0.0f,0.0f);
  for(int y=0;y<length;y++)
    for(int x=;x<width;x++)
    {
      if(1)
      {
        p1.assign(x,y,getHeight(x,y));
        p2 = p1 + normals[x][y] * 10 ;
        p3=normals[x][y];
        glBegin(GL_LINES); 

        glVertex3f(p1.x, p1.y,p1.z);
        glVertex3f(p2.x, p2.y,p2.z);
        glEnd();
      }
    }
    **/
}

void myTerrain::computeNormals(){
  myVector sum;
  myVector out;
  myVector in;
  myVector left;
  myVector right;


  for(int y = 0; y < length; y++) {
    for(int x = 0; x < width ; x++) {

      sum.assign(0.0f,0.0f,0.0f);
      in.assign(0.0f,0.0f,0.0f);
      out.assign(0.0f,0.0f,0.0f);
      left.assign(0.0f,0.0f,0.0f);
      right.assign(0.0f,0.0f,0.0f);

      if (y > 0) {
        out.assign(0.0f,-1.0f, heights[x][y - 1] - heights[x][y]-'\0');
      }
      if (y < length - 1) {
        in.assign(0.0f, 1.0f, heights[x][y + 1] - heights[x][y]-'\0');
      }
      if (x > 0) {
        left.assign(-1.0f, 0.0f, heights[x - 1][y] - heights[x][y]-'\0');
      }
      if (x < width - 1) {
        right.assign(1.0f, 0.0f, heights[x + 1][y] - heights[x][y]-'\0');
      }

      if (x > 0 && y > 0) {
        sum = sum + out.crossProduct(left).normalise();
      }
      if (x > 0 && y < length - 1) {
        sum = sum + left.crossProduct(in).normalise();
      }
      if (x < width - 1 && y < length - 1) {
        sum = sum + in.crossProduct(right).normalise();
      }
      if (x < width - 1 && y > 0) {
        sum = sum + right.crossProduct(out).normalise();
      }

      normals[x][y] = sum.normalise() * -1;
    }
  }

  const float FALLOUT_RATIO = 0.5f;
  for(int y = 0; y < length; y++) {
    for(int x = 0; x < width; x++) {
      sum = normals[x][y];

      if (x > 0) {
        sum = sum + normals[x - 1][y] * FALLOUT_RATIO;
      }
      if (x < width - 1) {
        sum = sum + normals[x + 1][y] * FALLOUT_RATIO;
      }
      if (y > 0) {
        sum = sum + normals[x][y - 1] * FALLOUT_RATIO;
      }
      if (y < length -1) {
        sum = sum + normals[x][y + 1];
      }

      if (sum.mod() == 0) {
        sum.assign(0.0f, 0.0f, 1.0f);
      }
      normals[x][y] = sum.normalise();
    }
  }
  for(int x=0;x<width;x++)
   {
   normals[x][0].assign(0,0,1);
   normals[0][x].assign(0,0,1);
   }
}
GLuint myTerrain::loadRAWTexture()
{  
  GLuint texture;
  glGenTextures(1,&texture);

  glBindTexture(GL_TEXTURE_2D, texture);
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

/**
  unsigned char * data;
  int w=width;
  int h=height;

  data = (unsigned char*)malloc(w*h*3);
  FILE *fp;
  fp = fopen(fileName,"rb");
  fread(data,w*h*3,1,fp);
  fclose(fp);
**/
  gluBuild2DMipmaps( GL_TEXTURE_2D, 3, textureWidth, textureLength,GL_RGB, GL_UNSIGNED_BYTE, data );
//  free(data);

//  cout<<"SPM :"<<texture;
  return texture;
}
void myTerrain::loadData(const char *fileName, int width, int height)
{
  int w=width;
  int h=height;

  data = (unsigned char*)malloc(w*h*3);
  FILE *fp;
  fp = fopen(fileName,"rb");
  fread(data,w*h*3,1,fp);
  fclose(fp);
}

/**
// Testing !!
main(){
  myTerrain T("simpleHill.raw",512,512);
  cout<<sizeof(T)<<endl;

  T.loadRAWTexture("grass.raw",64,64);
}
**/
