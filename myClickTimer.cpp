#include "myClickTimer.h"

myClickTimer::myClickTimer(myCamera *c){
  C=c;
  reset();
  totalTime=totTime;
  isStopped = true;
}
void myClickTimer::render(){
  glPushMatrix();
  glColor3f(1,0,0);

  myVector axis(0,0,0);
  myVector temp(0,0,1);
  myVector rightDash;

  temp = C->position + C->front;
  glTranslatef(temp.x, temp.y,temp.z);
   
  temp.assign(0,0,1); 
  axis = (C->front * -1).crossProduct(temp).normalise();      
  angle = acos(temp.dot(C->front)/C->front.mod());
  angle = (angle*180)/PI;
 
  rightDash = C->right.rotateVectorByDegrees(axis,-angle);
  glRotatef(angle,axis.x,axis.y,axis.z);
  
  temp.assign(1,0,0); 
  axis = (rightDash).crossProduct(temp).normalise();      
  angle = acos(temp.dot(rightDash)/rightDash.mod());
  angle = (angle*180)/PI;
 
  glRotatef(-angle,axis.x,axis.y,axis.z);
  
  int x,y,Height,width;

  x=30;y=-30;width=5;Height=60;
 
  float frac = getFractionTimePassed();
  float  height = frac*Height + y;

if(!isStopped)
{
//draw my 2d world here 
  
  glBlendFunc(GL_SRC_ALPHA,GL_ONE);
  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);

  glBegin(GL_QUADS); 
  
  glColor4f(2.0f,0.0f,0.0f,1.0f);         // Full Brightness, 50% Alpha ( NEW )
  temp = getScaledCoordinates(x+width,y);  
  glVertex3f(temp.x,temp.y,temp.z);
  glColor4f(2.0f,0.0f,0.0f,1.0f);         // Full Brightness, 50% Alpha ( NEW )
  temp = getScaledCoordinates(x,y);
  glVertex3f(temp.x,temp.y,temp.z);
  glColor4f(2.0f*(1-frac),2.0f*frac,0.0f,1.0f);         // Full Brightness, 50% Alpha ( NEW )
  temp = getScaledCoordinates(x,height);
  glVertex3f(temp.x,temp.y,temp.z);
  glColor4f(2.0f*(1-frac),2.0f*frac,0.0f,1.0f);         // Full Brightness, 50% Alpha ( NEW )
  temp = getScaledCoordinates(x+5,height);
  glVertex3f(temp.x,temp.y,temp.z);

  glEnd();
  
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
//drawing 2d world complete
}
  glPopMatrix();
}
myVector myClickTimer::getScaledCoordinates(float x,float y){
  return myVector(x/mySystemSize*0.57,-1*y/mySystemSize*0.57,1);
}
void myClickTimer::reset(){
  gettimeofday(&t, NULL);
  startTime = t.tv_sec*1000000 + (t.tv_usec);
//  cout<<"Start : "<<startTime<<endl;
  isStopped = false;
}
float myClickTimer::getFractionTimePassed(){
  gettimeofday(&t, NULL);
  curTime = t.tv_sec*1000000 + (t.tv_usec);
  timeElapsed = curTime - startTime; 
  if(timeElapsed>=totalTime)
  { 
    return 1;
  }
  else
  {
  return ((int)timeElapsed%(int)totalTime)/totalTime;
  }
}

