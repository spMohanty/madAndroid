#include "myCamera.h"
//myCamera.cpp starts

myCamera::myCamera(){
  // Setting up the default camera alignment of OpenGL, located at (0,0,0) and facing towards -z direction
  drawGrid=false;
  showCursor=false;
  drawBoundBox=false;
  initialise();
}
void myCamera::initialise(){
  position.assign(0.0f, 0.0f, 15.0f);
  front.assign(1.0f, 1.0f, 0.0f);
  right.assign(1.0f, -1.0f, 0.0f);
  up.assign(0.0f, 0.0f, 1.0f);

  up.normalise();
  front.normalise();
  right.normalise();

  speed_traverse=1;
  speed_look=1;

  textureWidth=64;
  textureLength=64;
  loadData("rawFiles/wallTexture.raw",textureWidth,textureLength,0);
  loadData("rawFiles/roofTexture.raw",textureWidth,textureLength,1);
  
  pathTracedLastParticle = new vector<myVector>;
//  stabiliseCamera();
}

void myCamera::stabiliseCamera(){
  //Bring the camera to the X-Y plane with its top vector being positive !!
  
// initialise(); 
  
  myVector axis(0,0,0);
  myVector temp(0,0,1);
  axis = up.crossProduct(temp).normalise();
  float angle = acos( temp.dot(up)/up.mod() );
  angle = (angle*180)/PI;
  up = up.rotateVectorByDegrees(axis,angle).normalise();
  right = right.rotateVectorByDegrees(axis,angle).normalise();
  front = front.rotateVectorByDegrees(axis,angle).normalise();
}


void myCamera::moveForward(float units){
  

  if(mode==0)//Can Fly Any Where 
  {
      position = position + front * (speed_traverse*units);
  
  }
  if(mode==1)// Ghost Mode but has a bounded world
  {
    position = position + front * (speed_traverse*units);
    if(((position.x<0 || position.x>worldWidth) || (position.y<0 || position.y>worldLength) || (position.z<0 || position.z>worldHeight) ))
    {
      //cout<<position<<endl;
      position = position - front * (speed_traverse*units);
    }
  }
  if(mode==2)// Walk Mode
  {
    // Walk Mode Code
    myVector axis(0,0,0);
    myVector temp;

    temp = terrainNormals[(int)position.x][(int)position.y];
    axis = up.crossProduct(temp).normalise();

    float angle = acos( temp.dot(up)/up.mod() );
    angle = (angle*180)/PI;
    //up = up.rotateVectorByDegrees(axis,angle).normalise();
    //right = right.rotateVectorByDegrees(axis,angle).normalise();
    myVector frontDash = front.rotateVectorByDegrees(axis,angle).normalise();
    

    position = position + frontDash * (speed_traverse*units);
    if(((position.x<0 || position.x>worldWidth) || (position.y<0 || position.y>worldLength) || (position.z<0 || position.z>worldHeight) ))
    {
      position = position - frontDash * (speed_traverse*units);
    }
  }
}

void myCamera::moveBackward(float units){
  if(mode==0)//Can Fly Any Where 
  {
    position = position - front * (speed_traverse*units);
  
  }
  if(mode==1)// Ghost Mode but has a bounded world
  {
      position = position - front * (speed_traverse*units);
   if(((position.x<0 || position.x>worldWidth) || (position.y<0 || position.y>worldLength) || (position.z<0 || position.z>worldHeight) ))
    {
      position = position + front * (speed_traverse*units);
    }
  }
    // Walk Mode Code
  if(mode==2)// Walk Mode
  {
    // Walk Mode Code
    myVector axis(0,0,0);
    myVector temp;

    temp = terrainNormals[(int)position.x][(int)position.y];
    axis = up.crossProduct(temp).normalise();

    float angle = acos( temp.dot(up)/up.mod() );
    angle = (angle*180)/PI;
    //up = up.rotateVectorByDegrees(axis,angle).normalise();
    //right = right.rotateVectorByDegrees(axis,angle).normalise();
    myVector frontDash = front.rotateVectorByDegrees(axis,angle).normalise();
    

    position = position - frontDash * (speed_traverse*units);
    if(((position.x<0 || position.x>worldWidth) || (position.y<0 || position.y>worldLength) || (position.z<0 || position.z>worldHeight) ))
    {
      position = position + frontDash * (speed_traverse*units);
    }
  }

}

void myCamera::strafeLeft(float units){
  
  if(mode==0)//Can Fly Any Where 
  {
  position = position - right * (speed_traverse*units);
  
  }
  if(mode==1)// Ghost Mode but has a bounded world
  {
  position = position - right * (speed_traverse*units);
    if(((position.x<0 || position.x>worldWidth) || (position.y<0 || position.y>worldLength) || (position.z<0 || position.z>worldHeight) ))
    {
      position = position + right * (speed_traverse*units);
    }
  }
  if(mode==2)// Walk Mode
  {
    // Walk Mode Code
    myVector axis(0,0,0);
    myVector temp;

    temp = terrainNormals[(int)position.x][(int)position.y];
    axis = up.crossProduct(temp).normalise();

    float angle = acos( temp.dot(up)/up.mod() );
    angle = (angle*180)/PI;
    //up = up.rotateVectorByDegrees(axis,angle).normalise();
    myVector rightDash = right.rotateVectorByDegrees(axis,angle).normalise();
    
    position = position - rightDash * (speed_traverse*units);
    if(((position.x<0 || position.x>worldWidth) || (position.y<0 || position.y>worldLength) || (position.z<0 || position.z>worldHeight) ))
    {
      position = position + rightDash * (speed_traverse*units);
    }
  }

}

void myCamera::strafeRight(float units){
  if(mode==0)//Can Fly Any Where 
  {
  position = position + right * (speed_traverse*units);
    }
  if(mode==1)// Ghost Mode but has a bounded world
  {
  position = position + right * (speed_traverse*units);
    if(((position.x<0 || position.x>worldWidth) || (position.y<0 || position.y>worldLength) || (position.z<0 || position.z>worldHeight) ))
    {
      position = position - right * (speed_traverse*units);
    }
  }
  if(mode==2)// Walk Mode
  {
    // Walk Mode Code
    myVector axis(0,0,0);
    myVector temp;

    temp = terrainNormals[(int)position.x][(int)position.y];
    axis = up.crossProduct(temp).normalise();

    float angle = acos( temp.dot(up)/up.mod() );
    angle = (angle*180)/PI;
    //up = up.rotateVectorByDegrees(axis,angle).normalise();
    myVector rightDash = right.rotateVectorByDegrees(axis,angle).normalise();
    
    position = position + rightDash * (speed_traverse*units);
    if(((position.x<0 || position.x>worldWidth) || (position.y<0 || position.y>worldLength) || (position.z<0 || position.z>worldHeight) ))
    {
      position = position - rightDash * (speed_traverse*units);
    }
  }
}

void myCamera::moveUp(float units){
  position = position + up * (speed_traverse*units);
}

void myCamera::moveDown(float units){
  position = position - up * (speed_traverse*units);
}

void myCamera::lookUp(float units){
  myVector newFront,newUp;

  newFront = front.rotateVectorByDegrees(right,speed_look*units);
  
  newUp = right.crossProduct(newFront);
  newUp.normalise();

  front = newFront;
  up = newUp;


}

void myCamera::lookDown(float units){
  myVector newFront,newUp;

  newFront = front.rotateVectorByDegrees(right,-1*speed_look*units);
  
  newUp = right.crossProduct(newFront);
  newUp.normalise();

  front = newFront;
  up = newUp;
}

void myCamera::lookLeft(float units){
  myVector newFront,newRight;

  newFront = front.rotateVectorByDegrees(up,speed_look*units);
  
  newRight = newFront.crossProduct(up);
  newRight.normalise();

  front = newFront;
  right = newRight;
 
}

void myCamera::lookRight(float units){
  myVector newFront,newRight;

  newFront = front.rotateVectorByDegrees(up,-1*speed_look*units);
  
  newRight = newFront.crossProduct(up);
  newRight.normalise();

  front = newFront;
  right = newRight;
}
void myCamera::_drawGrid(){

  // Axis
  glColor3f(1.0f,0.0f,0.0f);
  glBegin(GL_LINES);
  glVertex3f(0.0f,0.0f,0.0f);
  glVertex3f(500.0f,0.0f,0.0f);
  glEnd();


  glColor3f(0.0f,1.0f,0.0f);
  glBegin(GL_LINES);
  glVertex3f(0.0f,0.0f,0.0f);
  glVertex3f(0.0f,500.0f,0.0f);
  glEnd();


  glColor3f(0.0f,0.0f,1.0f);
  glBegin(GL_LINES);
  glVertex3f(0.0f,0.0f,0.0f);
  glVertex3f(0.0f,0.0f,500.0f);
  glEnd();


    
}
void myCamera::_drawBoundBox(){
/**
int i,rows,columns;
rows=worldHeight;
columns=worldWidth;
int M=16;
glPushMatrix();
glColor3f(1,1,1);
glBegin(GL_LINES);
i=0;
int p=0;
while(p*M<worldWidth)
{
  i=p*M;
  
  glVertex3f(0,0, i);
  glVertex3f(0,columns, i);

  glVertex3f(0,i, 0);
  glVertex3f(0,i, rows);
  
  glVertex3f(0,0, i);
  glVertex3f(columns,0, i);

  glVertex3f(i, 0,0);
  glVertex3f(i,0, rows);
  
  glVertex3f(worldWidth,0, i);
  glVertex3f(worldWidth,columns, i);
  
  glVertex3f(worldLength,i, 0);
  glVertex3f(worldLength,i, rows);
  
  glVertex3f(0,worldLength, i);
  glVertex3f(columns,worldLength, i);
  
  glVertex3f(i, worldLength,0);
  glVertex3f(i,worldLength, rows);

  p+=1;
//------------------------

}
glEnd();
glPopMatrix();
**/
  glColor3f(1,1,1);
 GLuint texture = loadRAWTexture(data[0]);

   glEnable( GL_TEXTURE_2D );
   glBindTexture( GL_TEXTURE_2D, texture );

   glBegin(GL_QUADS);
   glTexCoord2f(0,0);
   glVertex3f(0,0,0);
    glTexCoord2f((float)worldLength/(float)textureWidth, 0);
   glVertex3f(0,worldLength,0);
   glTexCoord2f((float)worldLength/(float)textureWidth, (float)worldHeight/(float)textureLength);
   glVertex3f(0,worldLength,worldHeight);
    glTexCoord2f(0,(float)worldHeight/(float)textureLength);
   glVertex3f(0,0,worldHeight);
   glEnd();

  
   glBegin(GL_QUADS);
   glTexCoord2f(0,0);
   glVertex3f(0,0,0);
    glTexCoord2f((float)worldWidth/(float)textureWidth, 0);
   glVertex3f(worldWidth,0,0);
   glTexCoord2f((float)worldWidth/(float)textureWidth, (float)worldHeight/(float)textureLength);
   glVertex3f(worldWidth,0,worldHeight);
    glTexCoord2f(0,(float)worldHeight/(float)textureWidth);
   glVertex3f(0,0,worldHeight);
   glEnd();


   glBegin(GL_QUADS);
   glTexCoord2f(0,0);
   glVertex3f(worldWidth,0,0);
    glTexCoord2f((float)worldLength/(float)textureWidth, 0);
   glVertex3f(worldWidth,worldLength,0);
   glTexCoord2f((float)worldLength/(float)textureWidth, (float)worldHeight/(float)textureLength);
   glVertex3f(worldWidth,worldLength,worldHeight);
    glTexCoord2f(0,(float)worldHeight/(float)textureLength);
   glVertex3f(worldWidth,0,worldHeight);
   glEnd();

  
   glBegin(GL_QUADS);
   glTexCoord2f(0,0);
   glVertex3f(0,worldLength,0);
    glTexCoord2f((float)worldWidth/(float)textureWidth, 0);
   glVertex3f(worldWidth,worldLength,0);
   glTexCoord2f((float)worldWidth/(float)textureWidth, (float)worldHeight/(float)textureLength);
   glVertex3f(worldWidth,worldLength,worldHeight);
    glTexCoord2f(0,(float)worldHeight/(float)textureWidth);
   glVertex3f(0,worldLength,worldHeight);
   glEnd();

   texture = loadRAWTexture(data[1]);
  
   glBegin(GL_QUADS);
   glTexCoord2f(0,0);
   glVertex3f(0,0,worldHeight);
    glTexCoord2f(0,(float)worldLength/(float)textureLength);
   glVertex3f(0,worldLength,worldHeight);
   glTexCoord2f((float)worldWidth/(float)textureWidth, (float)worldLength/(float)textureLength);
   glVertex3f(worldWidth,worldLength,worldHeight);
    glTexCoord2f((float)worldWidth/(float)textureWidth,0);
   glVertex3f(worldWidth,0,worldHeight);
   glEnd();

}

void myCamera::render(){
    myVector temp;
      temp=position+front  ;
        gluLookAt(position.x,position.y,position.z, temp.x ,temp.y , temp.z ,up.x,up.y,up.z);
//glMaterialf(GL_FRONT, GL_SHININESS, 20.0);
    if(!showCursor){
     glutSetCursor(GLUT_CURSOR_DESTROY);
   }

    if(drawGrid)
    {
      _drawGrid();
    }
    glEnd();
    if(drawBoundBox)
    {
    _drawBoundBox();
    }
    
    //if in mode 1 : observation mode, also draw the following things :)
    if(mode==1)
    {
      // a greenish color filter infront of eye !!
      // path of last particle
      // render path traced
      myVector p1;
      int ii=0;
      for(ii=0;ii<pathTracedLastParticle->size();ii++)
      {
        if(ii==0)
        {
          glColor3f(0,1,0);
        }
        else if(ii==pathTracedLastParticle->size()-1)
        {
          glColor3f(1,0,0);
        }
        else
        {
          glColor3f(0,0,1);
        }
        p1=pathTracedLastParticle->at(ii);
        glPushMatrix();
        glTranslatef(p1.x,p1.y,p1.z);
        glutSolidSphere(0.5,5,5);
        glPopMatrix();
      }
          glColor3f(1,1,1);

      // path rendered
    }
    
}
void myCamera::loadData(const char *fileName, int width, int height,int location)
{
  int w=width;
  int h=height;

  data[location] = (unsigned char*)malloc(w*h*3);
  FILE *fp;
  fp = fopen(fileName,"rb");
  fread(data[location],w*h*3,1,fp);
  fclose(fp);
}

GLuint myCamera::loadRAWTexture(unsigned char *data1)
{
  GLuint texture;
  glGenTextures(1,&texture);

  glBindTexture(GL_TEXTURE_2D, texture);
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

  gluBuild2DMipmaps( GL_TEXTURE_2D, 3, textureWidth, textureLength,GL_RGB, GL_UNSIGNED_BYTE, data1);
  return texture;
}
void myCamera::toObservationMode(vector<myVector> pathTraced)
{
  //take backup of current state of camera
  upBackup = up;
  frontBackup = front;
  rightBackup = right;
  positionBackup = position;
  //backup taken
  mode = 1;
  // copy the contents of pathTraced to global pathTracedLastParticle pointer
  pathTracedLastParticle = new vector<myVector>;

  int ii;
  for(ii=0;ii<pathTraced.size();ii++)
  {
    pathTracedLastParticle->push_back(pathTraced[ii]);
  }
}
void myCamera::toWalkMode()
{
  // restore position ot backed up position
  up = upBackup;
  front = frontBackup;
  right = rightBackup;
  position = positionBackup;
  mode = 2;
 
}
//Testing purposes !!
/**
main(){
  
  myCamera C;
  cout<<C.front<<C.right<<C.up<<C.position<<"----"<<endl;
  
  C.lookRight(90);
  cout<<C.front<<C.right<<C.up<<C.position<<"----------"<<endl;;
  C.stabiliseCamera();

i*/
