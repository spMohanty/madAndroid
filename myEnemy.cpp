#include "myEnemy.h"


myEnemy::myEnemy(myTerrain *t){
    life = 100;    
    isDead = false;
    pCount = 0;
    T=t;
    // set Camera to walk Mode
    C.mode=0;

    C.position.x = rand()%t->width + 1;
    C.position.y = rand()%t->length + 1;
    C.position.z = t->heights[(int)C.position.x][(int)C.position.y];
      
  /**
    C.position.y = 200;
    C.position.x = 200;
    
    C.position.x = 300;
    **/
    C.front.assign(1,0,0);
    C.up.assign(0,0,1);
    C.right.assign(1,0,0);

    C.worldWidth = T->width;
    C.worldLength = T->length;
    C.worldHeight = T->height;
    C.speed_traverse = 1;



    width = 10; // of Cylinder
    length = 10;
    bodyHeight = 7.5;
    radiusOfLeg=1.75;
    armRadius=1.5;
    eyeRadius = 0.5;

    height = 2*radiusOfLeg + bodyHeight + width/2;
}

void myEnemy::render(){
  
if(!G->isPaused)  
{
  computeAction();
}
  _drawEnemy();

  glColor3f(1,1,1);
  // rendering particles
  for(map<int, myParticle*>::iterator ii=particles.begin();ii!=particles.end();++ii)
  {
      if((*ii).second->isDead)
          {
                particles.erase((*ii).second->name);
                    free((*ii).second);
                      }
                        else
                            {
                                (*ii).second->render();
                                  }
  }
  // particles rendered


}
void myEnemy::computeAction(){
  V.assign(C.position.x,C.position.y,C.position.z);
  V.z+=height;
  C.front = (playerCamera->position - V).normalise();
  C.right =C.front.crossProduct(C.up).normalise();

  C.moveForward(0.8);
   
   srand(rand());
  if(rand()%100==6)
  {
  // then shoot under some condition
   
    //distance along X-Y direction 
    V = C.position;
    V.z = 0;

    //V.mod : distance
    
     //avg time before redraw : 22769 microseconds
     
     //max Velocity : C.MAX_VELOCITY;
     float t;
     myVector V(100,100,100);
     myVector S;

     S = playerCamera->position - C.position;
     
     V = S;
     V.normalise();
     V.z = 1;
        myParticle *P = new myParticle(C.position,3,T,G);
        P->name = pCount;
        P->velocity = V;
        P->velocityScaleFactor = 0.3;
        P->noOfBounceBeforeExplosion = 1;
        particles[pCount++]=P;

  }
}
void myEnemy::_drawEnemy(){

  glPushMatrix();
  // draw a cube at myPosition
  glTranslatef(C.position.x,C.position.y,C.position.z); // taking it to the position 

  glPushMatrix();
  
  glColor3f(0,1,0);
  
  glTranslatef(0,0,radiusOfLeg);
  //drawing legs :)
  glTranslatef(((width-4*radiusOfLeg)/2)+radiusOfLeg, 0, 0);
  glutSolidSphere(radiusOfLeg,10,10);
  glTranslatef(-2*(((width-4*radiusOfLeg)/2)+radiusOfLeg), 0, 0);
  glutSolidSphere(radiusOfLeg,10,10);
 
  glTranslatef(((width-4*radiusOfLeg)/2)+radiusOfLeg, 0, radiusOfLeg);
  // drawing cylinder body
  GLUquadricObj* cyl;
  cyl = gluNewQuadric();
  gluQuadricDrawStyle(cyl, GLU_FILL);
  gluCylinder(cyl, width/2, length/2, bodyHeight, 15, 15);
  free(cyl);

  // drawing head
  glTranslatef(0,0,bodyHeight);
  glutSolidSphere(width/2 - 0.05,15,15);

  glPushMatrix();

  // only for the eyes

  //orienting it at its position :)

    // current front : 0,1,0
    // trying to make "front" as its front
        //angle between Y vector and its front vector
      V.assign(0,1,0); //original Front
      angle = acos(V.dot(C.front)/(V.mod()*C.front.mod()))*180/PI;
      V = V.crossProduct(C.front).normalise(); // axis along which we rotate
      glRotatef(angle, V.x,V.y,V.z);
 

      V.assign(0,0,1); //original UP
      angle = acos(V.dot(C.up)/(V.mod()*C.up.mod()))*180/PI;
      V = V.crossProduct(C.up).normalise(); // axis along which we rotate
      glRotatef(angle, V.x,V.y,V.z);
  //orientation of the eyes done 
 // drawing eyes
  glColor3f(1,0,0);
  glPushMatrix();
  glRotatef(60,0,0,1);
  glPushMatrix();
  glRotatef(25,0,-1,0);
  glTranslatef(width/2,0,0);
  glutSolidSphere(eyeRadius,10,10);
  glPopMatrix();
  glRotatef(60,0,0,1);
  glRotatef(25,0,-1,0);
  glTranslatef(width/2,0,0);
  glutSolidSphere(eyeRadius,10,10);

  glPopMatrix();
  glPopMatrix();
  //eyes Done
  
  glColor3f(0,1,0);
  
  //rotate the arms along the front

  glTranslatef(0,0,-bodyHeight/2); 

  glPushMatrix();
    V.assign(0,1,0); //original Front
    angle = acos(V.dot(C.front)/(V.mod()*C.front.mod()))*180/PI;
    V = V.crossProduct(C.front).normalise(); // axis along which we rotate
    glRotatef(angle, V.x,V.y,V.z);

  glTranslatef(width/2+armRadius,0,0);
  glScalef(1,1,3);
  glutSolidSphere(armRadius,10,10);
  glTranslatef(-width-2*armRadius,0,0);
  glutSolidSphere(armRadius,10,10);
  glPopMatrix();
/**
  glTranslatef(0,0,height/2);
  glColor3f(0,1,0);
  glutSolidCube(20);
  **/
  glPopMatrix();
  glPopMatrix();
}
bool myEnemy::isColliding(myVector pos){
  myVector temp= pos - C.position  ;// relative location of point in the frame of reference of the enemy
 
  if(abs(temp.x)<=width/2 && abs(temp.y)<=length/2 && temp.z>=0 && pos.z<=height)
  {
    return true;
  }
  else
  {
    return false;
  }

}
bool myEnemy::inExplosionRange(myVector pos, float Range){
  
  myVector temp= pos - C.position  ;// relative location of point in the frame of reference of the enemy
  
if(abs(temp.x)<=width/2 + Range && abs(temp.y) <= length/2 + Range && temp.z <= height + Range)
  return true;
else
  return false;

}
