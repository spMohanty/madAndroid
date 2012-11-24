#include "myScreen.h"

myScreen::myScreen(){
}
void myScreen::set2DScreen(){

  glPushMatrix();
  glColor3f(1,0,0);

  myVector axis(0,0,0);
  myVector temp(0,0,1);
  myVector rightDash;

  temp = C->position + C->front;
  glTranslatef(temp.x, temp.y,temp.z);

  temp.assign(0,0,1); 
  axis = (C->front * -1).crossProduct(temp).normalise();    
  float angle = acos(temp.dot(C->front)/C->front.mod());
  angle = (angle*180)/PI;

  rightDash = C->right.rotateVectorByDegrees(axis,-angle);
  glRotatef(angle,axis.x,axis.y,axis.z);

  temp.assign(1,0,0); 
  axis = (rightDash).crossProduct(temp).normalise();    
  angle = acos(temp.dot(rightDash)/rightDash.mod());
  angle = (angle*180)/PI;

  glRotatef(-angle,axis.x,axis.y,axis.z);
  glDisable(GL_DEPTH_TEST);
}
myVector myScreen::getScaledCoordinates(float x,float y){
    return myVector(x/mySystemSize*0.57,-1*y/mySystemSize*0.57,1);
}
void myScreen::unset2DScreen(){
  glEnable(GL_DEPTH_TEST);
  glPopMatrix();
}
void myScreen::render(float score,float level){
  set2DScreen();

  if(G->isPaused)
   {
  _observationModeScreen();
   }
  if(G->showScores)
  {
  _showScores(score,level);
  }
  
  unset2DScreen();
}
void myScreen::_showScores(float score,float level){

glPushMatrix();

 glBlendFunc(GL_SRC_ALPHA,GL_ONE);
 glEnable(GL_BLEND);
 
 // player Life
 float MAX_WIDTH = 275;
 float width ;
 width = ((float)G->playerLife/(float)G->playerMAXLife)*MAX_WIDTH;
 float height = 5;
 float dis = 140;
  

glColor3f(0,1,0);

//glutSolidSphere(0.001*width,10,10);

 glBegin(GL_TRIANGLE_STRIP);
 V=getScaledCoordinates(-dis,-dis);
 glVertex3f(V.x,V.y,V.z);
 V=getScaledCoordinates(-dis + width ,-dis);
 glVertex3f(V.x,V.y,V.z);
 V=getScaledCoordinates(-dis,-dis + height);
 glVertex3f(V.x,V.y,V.z);
 V=getScaledCoordinates(-dis + width,-dis+height);
 glVertex3f(V.x,V.y,V.z);
 glEnd();
 
 glPopMatrix();
 //enemyLife
 glPushMatrix();
  
 glColor3f(1,0,0);
 width = (G->enemyLife / G->enemyMAXLife)* MAX_WIDTH;
 height = 5;
 float extraHeight = 6;
  
 glBegin(GL_QUADS);
 V=getScaledCoordinates(-dis,-dis + extraHeight);
 glVertex3f(V.x,V.y,V.z);
 V=getScaledCoordinates(-dis +width ,-dis + extraHeight);
 glVertex3f(V.x,V.y,V.z);
 V=getScaledCoordinates(-dis + width,-dis+height + extraHeight);
 glVertex3f(V.x,V.y,V.z);
 V=getScaledCoordinates(-dis,-dis + height + extraHeight);
 glVertex3f(V.x,V.y,V.z);
 glEnd();

glDisable(GL_BLEND);
glPopMatrix(); 


string s;
stringstream ostr;
ostr << s << "Score : "<<score;
s = ostr.str();

printText(650,750,s,1,0,0);

stringstream ostr1;

ostr1 << "Level : "<<level;
s=ostr1.str();
printText(10,750,s,1,0,0);
}
void myScreen::_observationModeScreen(){

  glColor3f(0,0.6,0);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE);
  glEnable(GL_BLEND);
  glBegin(GL_QUADS);
  glVertex3f(1,1,1);
  glVertex3f(1,-1,1);
  glVertex3f(-1,-1,1);
  glVertex3f(-1,1,1);
  glEnd();
  glDisable(GL_BLEND);
  printText(10,780,"Observation Mode !!",1,0,0);
}
void myScreen::printText(int x,int y,string String,float colorR,float colorG, float colorB)
{
  //(x,y) is from the bottom left of the window
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, 800, 0, 800, -1.0f, 1.0f);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glPushAttrib(GL_DEPTH_TEST);
  glDisable(GL_DEPTH_TEST);
  glRasterPos2i(x,y);
  glColor3f(colorR,colorG,colorB);
  for (int i=0; i<String.size(); i++)
  {
     // glutBitmapCharacter(GLUT_BITMAP_9_BY_15, String[i]);
     glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, String[i]);
     //glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, String[i]);
  }
  glPopAttrib();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}
