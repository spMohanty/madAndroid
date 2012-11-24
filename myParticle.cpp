#include "myParticle.h"

myParticle::myParticle(myVector P, float r,myTerrain *t1, myGame *te) {
G = te;
T=t1;
coefficientOfRestitution = 0.9;

position = P;
radius = r;

//velocity.assign(2000,1923,1950);
velocity.normalise();

  /**
  color[0]=clr[0];//R
  color[1]=clr[1];//G
  color[2]=clr[2];//B
**/


// init state variables
  gettimeofday(&t, NULL);
  birthTime = t.tv_sec*1000000 + (t.tv_usec);
  lastRenderedTime = birthTime;

  g.assign(0,0,0.5);
  
  srand(birthTime);
  /**
  colorB=(float)(rand()%100)/100;
  colorR=(float)(rand()%100)/100;
  colorG=(float)(rand()%100)/100;
  **/
  colorR=0;
  colorB=0;
  colorG=1;
    
    velocityScaleFactor=1;
    accelerationScaleFactor = 1; 
     MAX_VELOCITY=350;
    MAX_ACCELERATION=20;
    noOfBounces=0; 
      isActivated = true;
true;
    
    noOfBounceBeforeExplosion = 1;
    explosionTotalTime=2000000;
    isExploding = false;
    isDead = false;
    isActivated = false;
// state variables initialisation complete
}
void myParticle::computeAction(){
  if(!isExploding)
  {
    //update velocity;
    velocity = velocity - g * (timeElapsed/10000000)*accelerationScaleFactor*MAX_ACCELERATION; 
//      velocity.normalise();
    //velocity updated

    //update position
    position = position + velocity *  (timeElapsed/1000000)*velocityScaleFactor*MAX_VELOCITY;
    //position updated

  manageCollissionWithTerrain();    
  manageCollissionWithWorldBoundaries();  
  manageCollissionWithOtherParticles();  

  pathTraced.push_back(position);
  }
  else
  {
    //:) do nothing :P
  }
}
void myParticle::manageCollissionWithTerrain(){
//Scan for all points in the range -r -> r for both (x and y)  and if the height at that distance is < radius of the sphere announce collision and manage

for(x=-radius;x<=radius;x++)
{
  for(y=-radius;y<=radius;y++)
  {
    X=position.x + x;
    Y = position.y + y;
    if(X>=0&&X<=(T->width)&&Y>=0&&Y<=(T->length))
    {   
      // check distance between center of sphere and the corresponding height of terrain
      temp.assign(X,Y,T->getHeight(X,Y));
      if((temp - position).mod()<=radius)
      {
        //announce collission and take action :) :P

        // reflection equation for incident vector along normal
        velocity = (velocity - (T->normals[(int)X][(int)Y]) * 2 * velocity.dot(T->normals[(int)X][(int)Y]) );

        temp = T->normals[(int)X][(int)Y] * velocity.dot(T->normals[(int)X][(int)Y]) ; // obtaining the component of the new velocity along the direction of normal
        velocity = temp * coefficientOfRestitution + (velocity - temp); //summing up the new velocity after using the coefficient of restitution along the normal component of the velocity

        // if the magnitude of the velocity vector is less than a threshold value, then we make it 0, and also annull the effect of gravity !!

/**
        if(abs(velocity.z)<=velocityThreshold)
        {
          g.assign(0,0,0);
          velocity.assign(0,0,0);
          position.assign(X,Y,T->getHeight(X,Y)+radius);
        }
   **/     
        noOfBounces += 1;
        
    if(noOfBounces==noOfBounceBeforeExplosion)
    {
      colorR=1;
      colorB=0;
      colorG=0;
      isActivated = true;
    }
    else if(noOfBounces>noOfBounceBeforeExplosion)
    {
      isExploding = true;
    }
        return; // One Collisson found, so we can safely assume there are no more collissions. 
      }
     }
    }
  }
}

void myParticle::manageCollissionWithOtherParticles(){
  // Collission with other spheres to be handled here :)
}
void myParticle::manageCollissionWithWorldBoundaries(){
    if(position.x+radius>=T->width)
    {
      velocity.x = -1*abs(velocity.x);
      position.x = T->width-radius;
      noOfBounces+=1;
    }
    if(position.x-radius<=0)
    {
      velocity.x = 1*abs(velocity.x);
      position.x = radius;
      noOfBounces+=1;
    }
    if(T->length <= position.y + radius)
    {
      velocity.y = -1*abs(velocity.y);
      position.y = T->length-radius;
      noOfBounces+=1;
    }
    if(position.y - radius <=0)
    {
      velocity.y = 1*abs(velocity.y);
      position.y = radius;
      noOfBounces+=1;
    }
    if(T->height <=  position.z+radius)
    {
      velocity.z = -1*abs(velocity.z);
      position.z = T->height-radius;
      noOfBounces+=1;
    }
    if(position.z - radius <= 0)
    {
      velocity.z = 1*abs(velocity.z);
      position.z = radius;
    }

    if(noOfBounces==noOfBounceBeforeExplosion)
    {
      colorR=1;
      colorB=0;
      colorG=0;
    }
    else if(noOfBounces>noOfBounceBeforeExplosion)
    {
      /**
      isExploding = true;
      gettimeofday(&t, NULL);
      explosionStartTime = (t.tv_sec*1000000 + (t.tv_usec)); // microSeconds
      **/
      startExplosion();
    }

}
void myParticle::startExplosion(){
      isExploding = true;
      gettimeofday(&t, NULL);
      explosionStartTime = (t.tv_sec*1000000 + (t.tv_usec)); // microSeconds
}
void myParticle::render(){

//cout<<velocity.mod()<<"    "<<velocity<<position<<endl; 
  gettimeofday(&t, NULL);
  timeElapsed = (t.tv_sec*1000000 + (t.tv_usec)) - lastRenderedTime; // microSeconds
  
  //cout<<timeElapsed<<endl;
    //cout<<noOfBounces<<"  "<<noOfBounceBeforeExplosion<<endl;
  //compute Necessary Action
  if(!G->isPaused)
  {
    computeAction();
  }
  //necessary Action Computed

  // draw the sphere
  glPushMatrix();  
/**
//  glColor3f(color[0],color[1],color[2]);
  GLfloat col0[] = { 1.0, 1.0, 1.0, 1.0 }; // RGBA
//  glMaterialfv(GL_FRONT, GL_SPECULAR, col0);
  glMaterialf(GL_FRONT, GL_SHININESS, 10.0);
  **/

  glTranslatef(position.x, position.y, position.z);
  
  if(isExploding)
  {
    //code for exploding ball
    
    gettimeofday(&t, NULL);
    timeElapsed = (t.tv_sec*1000000 + (t.tv_usec)) - explosionStartTime; // microSeconds

    // Explosion expands for 70% of time and then starts contracting

    float percentComplete = timeElapsed/explosionTotalTime;
       //cout<<"Percent Complete : "<<percentComplete<<endl;
    
    float div =0.8;
    if(percentComplete<div)
    {
        // draw increasing translucent sphere of %age of %cent complete
      glBlendFunc(GL_SRC_ALPHA,GL_ONE);
      glEnable(GL_BLEND);
      glColor4f(255,140,0,0.8);
      glutSolidSphere(((percentComplete)/(div))*MAX_EXPLOSION_RADIUS,15,15);
      glDisable(GL_BLEND);
    }
    else if(percentComplete<1)
    {
      //draw decreasing translucent sphere of %age of %left
      glBlendFunc(GL_SRC_ALPHA,GL_ONE);
      glEnable(GL_BLEND);
    //  glDisable(GL_DEPTH_TEST);
      glColor4f(255,140,0,0.8);
      glutSolidSphere((1-(percentComplete-div)/(1-div))*MAX_EXPLOSION_RADIUS,15,15);
     // glEnable(GL_DEPTH_TEST);
      glDisable(GL_BLEND);
    }
    else
    {
      isDead = true;
    }
  }
  else
  {
  glColor3f(colorR,colorG,colorB);
  glutSolidSphere(radius,10,10);
  }
  glPopMatrix();
/**
  // render path traced
    int ii=0;
    for(ii=0;ii<pathTraced.size();ii++)
    {
      if(ii==0)
      {
        glColor3f(0,1,0);
      }
      else if(ii==pathTraced.size()-1)
      {
        glColor3f(1,0,0);
      }
      else
      {
         glColor3f(0,0,1);
      }
     glPushMatrix();
       glTranslatef(pathTraced[ii].x,pathTraced[ii].y,pathTraced[ii].z);
       glutSolidSphere(0.5,5,5);
    glPopMatrix();
    }

  // path rendered
**/
 glColor3f(1,1,1);
  //sphere drawn

  lastRenderedTime = t.tv_sec*1000000 + (t.tv_usec);
}
