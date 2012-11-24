#ifndef MAIN
#define MAIN

#include<iostream>
#include<stdlib.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<map>
#include<vector>

#include "myCamera.h"
#include "myTerrain.h"
#include "myParticle.h"
#include "myClickTimer.h"
#include "myScreen.h"
#include "myEnemy.h"
#include "myGame.h"
#include "loadRAW.h"

using namespace std;

//state variables start
void increaseLevel();
bool interLevelState = false;

// state variables end

//Scene Variables Start
myScreen S;
myGame G;
myTerrain T("rawFiles/plane.raw",512,512);
myCamera C;
myClickTimer CT(&C);
int level = 1;

myVector t(100,100,100);
//myParticle P(t,3,&T);

//myParticle* particles[10000];
map<int, myParticle*> particles;
int pCount=0;

int bounceBeforeExplosion = 1;

map<int, myEnemy*> enemies;
int eCount=0;

int width=800;
int height=800;

int lastShotParticle;
// Scene Variables End

// Game Variables Start

// Game Variables End

void handleKeyPress(unsigned char key,
                    int x, int y) {
C.speed_traverse=4;

switch(key) {
    case 'a':
    case 'A':
      C.strafeLeft(1);
      break;
    case 'd':
    case 'D':
      C.strafeRight(1);
      break;
    case 'w':
    case 'W':
      C.moveForward(1);
      break;
    case 's':
    case 'S':
      C.moveBackward(1);
      break;
    case 'o':
    case 'O':
      if(!interLevelState);
      if(C.mode==2)
      {

          if(particles.find(lastShotParticle)!=particles.end())
          {
            C.toObservationMode(particles[lastShotParticle]->pathTraced);
          }
          else
          {
            vector<myVector> g; //empty vector 
            C.toObservationMode(g);
          }
          G.isPaused = true;
          G.canShoot = false;
      }
      else if(C.mode==1)
      {
        G.isPaused = false;
        G.canShoot = true;
        C.toWalkMode();
      }
      break;
    case 'm':
    case 'M':
      if(interLevelState)
        increaseLevel();
        break;
    case 27:
      exit(0);
      break;
  }
 }
int ww,wh,dx,dy;

void motion(int x, int y) {
  static bool wrap = false;
  if(!wrap) {
    ww = glutGet(GLUT_WINDOW_WIDTH);
    wh = glutGet(GLUT_WINDOW_HEIGHT);

    dx = x - ww / 2;
    dy = y - wh / 2;

      C.lookLeft(-0.15*dx);
      C.lookUp(-0.1*dy);

    // move mouse pointer back to the center of the window
    wrap = true;
    glutWarpPointer(ww / 2, wh / 2);
  } else {
    wrap = false;
  }
}
void myMouseClick(int button,int state, int x, int y){
//cout<<button<<state<<x<<y<<endl;
  
  if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
  {
    if(G.canShoot)
    {
    CT.reset();
    }
  }
  if(button==GLUT_LEFT_BUTTON &&state==GLUT_UP)
  {
    // Shooting script

    if(G.canShoot)
    {
//     cout<<pCount+1<<endl; // initialisation of ammo :) !!
     myParticle *p = new myParticle(C.position, 3, &T, &G);
     p->velocity = C.front;
     p->velocityScaleFactor = 0.2 + 0.8*CT.getFractionTimePassed();
     p->C = &C;
     p->name = pCount;
     lastShotParticle = pCount;
     p->noOfBounceBeforeExplosion = bounceBeforeExplosion;
     particles[pCount++]=p; 
     CT.isStopped = true;
    }
  }
  if(button==GLUT_MIDDLE_BUTTON && state==GLUT_DOWN)
  {
    C.stabiliseCamera();
  }
}

void initialiseEnemy(){
  // remove all previous enemies
  

    for(map<int, myEnemy*>::iterator ii=enemies.begin();ii!=enemies.end();++ii)
    {
        int temp = (*ii).second->name;
        free((*ii).second);
        enemies.erase(temp);
    }
  // initialise new enemies !!


  for(int k=0;k<level;k++)
  {
    myEnemy *E = new myEnemy(&T);
    eCount+=1;
    E->name = eCount; // starting from 1
    
    // initialise Enemies here    
        E->playerCamera = &C;
        E->G = &G;
    // enemies initialised 
    
    enemies[eCount] = E;
  }
}

void initRendering() {
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_COLOR_MATERIAL);

     glDepthFunc(GL_LEQUAL);

    glEnable(GL_LIGHTING); //Enable lighting
    glEnable(GL_LIGHT0); //Enable light #0
    glEnable(GL_LIGHT1); // SPOT LIGHT #1
    glEnable(GL_NORMALIZE); 
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);

    // init camera and world;
    C.worldWidth = T.width; 
    C.worldLength = T.length; 
    C.worldHeight = T.height;
    C.drawGrid = false;
    C.drawBoundBox = true;
    C.showCursor = false;
    C.mode=2;
    T.heightFactor = 1;
    C.G = &G;
    S.C = &C;
    S.G = &G;
    
    initialiseEnemy();

   // G.T = &T;
    //copying terrain Normals into C instance
    int x,y;
    for(x=0;x<T.width;x++)
    {
      for(y=0;y<T.length;y++)
      {
        C.terrainNormals[x][y]=T.normals[x][y];
      }
    }
 }

void handleResize(int w, int h) {
   glViewport(0,0, w, h);

   glMatrixMode(GL_PROJECTION);


   gluPerspective(45.0,
                (double)w/(double)h,
                 1.0,
                 3000.0);
}

void lightsOn() {

  //Add ambient light
  GLfloat ambientColor[] = {0.5f, 0.5f, 0.5f, 0.5f}; //Color (0.2, 0.2, 0.2)
  GLfloat lightColor0[] = {1.0f, 1.0f, 1.0f, 1.0f}; 
  GLfloat lightPos0[] = {0,0,0, 1.0f}; 
  GLfloat spotDirection[] = {0.0f,0.0f,-1.0f};
  GLfloat whiteSpecularMaterial[] = {0.5, 0.5 , 0.5 ,1};
  const GLfloat cutOff = 60.0f;
  const GLfloat exponent = 40;
  
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

  glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

  glMaterialfv(GL_FRONT, GL_AMBIENT, ambientColor);
  glMaterialf(GL_FRONT, GL_SHININESS, 20.0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecularMaterial);
  //Add positioned light
// GLfloat lightPos0[] = {C.position.x, C.position.y, C.position.z, 1.0f};
/**
//relative coordinates of origin
  myVector targetDirection;
  targetDirection.assign(1,1,1);

  myVector axis(0,0,1);
  axis = C.front.crossProduct(axis).normalise();
  float angle = acos(C.front.dot(axis)/C.front.mod());//radians
  targetDirection.rotateVector(axis,angle);
  targetDirection = targetDirection * -1;
  
  GLfloat spotDirection[]= {targetDirection.x,targetDirection.y,targetDirection.z};
  GLfloat lightPos0[]= {-C.position.x, -C.position.y, -C.position.z}; 
**/
//relative attributes of origin found :)
  
 glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0); 
 glLightfv(GL_LIGHT0, GL_POSITION,lightPos0);
 glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF,&cutOff);
 glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT,&exponent);
 glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION,spotDirection);

 glEnable(GL_LIGHT0);

  //glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

/**
  //Add directed light
  GLfloat lightColor1[] = {0.5f, 0.2f, 0.2f, 1.0f}; //Color (0.5, 0.2, 0.2)
  //Coming from the direction (-1, 0.5, 0.5)
  GLfloat lightPos1[] = {-1.0f, 0.5f, 0.5f, 0.0f};
  //GLfloat lightPos1[] = {C.position.x, C.position.y, C.position.z, 0.0f};
  glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
  glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
  **/
}
void detectAndActEnemyAndSelfAndBallCollissions(){
// All enemies are located in enemies map , all particles are present in particles map, and individual particles of individual elements are located in positions map of individual enemies classes :p
    
    // I assume friendly fire off, that means, the weapon that U shoot, CANNOT shurt you :) , same with the enemies :)
  float explosionImpactRadius = 60;
      
    for(map<int, myEnemy*>::iterator iE=enemies.begin();iE!=enemies.end();++iE)
    {
      if((*iE).second->isDead)
      {
        int temp = (*iE).second->name;
        free((*iE).second);
        enemies.erase(temp);
        
        continue;
      }
      else
      {
      // these are the particles that the player shoots, so we check its collissions with all the enemies

          for(map<int, myParticle*>::iterator ii=particles.begin();ii!=particles.end();++ii) // loop through all the particles and check its collission with the particles
          {
            if((*ii).second->isDead)
            {
              int temp = (*ii).second->name;
              free((*ii).second);
              particles.erase(temp);
              continue;
            }
            else
            {
                // collission check between enemy and particle to be done here
                if((*ii).second->isExploding &&  (*iE).second->inExplosionRange((*ii).second->position, explosionImpactRadius))
                {
                  G.enemyLife -= 0.2;
                   G.Score += 10;
                  (*iE).second->life -=0.2;
                }
                else if((*ii).second->isActivated  && (*iE).second->isColliding((*ii).second->position))
                {
                  // found an enemy in the range of one of my weapons so, explode the weapon, and decrease enemyLife :)
                  (*ii).second->startExplosion();
                   G.enemyLife -= 0.2;
                   G.Score += 10;
                  (*iE).second->life -=0.2;
                }
            }
          } 
         // collission check between enemy and my particles done !!

            // now check their shot particles for any collission with the player
              for(map<int, myParticle*>::iterator eI=(*iE).second->particles.begin(); eI != (*iE).second->particles.end();eI++)
              {
                if((*eI).second->isActivated &&  ((*eI).second->position - C.position).mod()<=(*eI).second->radius)
                {
                  (*eI).second->startExplosion();
                  G.playerLife -=0.2;
                }
                else if((*eI).second->isExploding &&  ((*eI).second->position - C.position).mod()<= explosionImpactRadius*1.2)
                {
                  G.playerLife -=0.2;
                }
              }

              if((*iE).second->life<=0)
              {
                (*iE).second->isDead = true;
              }
        }
    }
}
void increaseLevel(){
  level+=1;

  C.mode = 2;
  
  if(level>=10)
  {
    C.mode = 1;
  }
  
  if(level>=15)
  {
    bounceBeforeExplosion = 4;
  }

  C.position.z=15;
  initialiseEnemy();
  interLevelState = false;

  G.playerLife = 100;
  G.playerMAXLife = 100;

  G.enemyLife = level*100;
  G.enemyMAXLife = level * 100; 
  
  G.canShoot = true;
  G.isPaused = false;
  
  G.isGameOver = false;
}
void drawScene() {
  detectAndActEnemyAndSelfAndBallCollissions();

  glMatrixMode(GL_MODELVIEW);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  
  lightsOn(); // custom Function !! :)

// S.render();
//glColor3f(1,1,1);
// render Camera
C.render();
// renderring terrain
T.render();


glColor3f(1,1,1);
// rendering particles
for(map<int, myParticle*>::iterator ii=particles.begin();ii!=particles.end();++ii)
{
  if((*ii).second->isDead)
  {
    int temp = (*ii).second->name;
    free((*ii).second);
    particles.erase(temp);
    continue;
  }
  else
  {
  (*ii).second->render();
  }
}
// particles rendered


//rendering enemy
for(map<int, myEnemy*>::iterator ii=enemies.begin();ii!=enemies.end();++ii)
{
  if((*ii).second->isDead)
  {
    continue;
  }
  else
  {
  (*ii).second->render();
  }
}
//enemy renderd


// redering 2d screen :)
S.render(G.Score,level);

CT.render();

//ending screen
G.render();

if(G.isGameOver)
{
  if(G.winner)
  {
    C.mode = 1;
    S.printText(300,430,"LEVEL CLEAR !!",0,0,1);
    S.printText(250,300,"press 'M' to start the next level............",1,0,0);
    if(level==10-1)
    {
    S.printText(250,200,"From the next level, you can very well fly......... :)",0,0,1);
    }
    else if(level==15-1)
    {
    S.printText(250,200,"Things just got harder, your ammo needs 4 bounces to be activated............ :)",0,0,1);
    }
    interLevelState = true;
  }
  else
  {
    C.mode = 1;
    S.printText(300,430,"You Lose !! :(",0,0,1);
    S.printText(250,300,"No second chances, start over again :)",1,0,0);
  }
}

//ending screen drawn


//fixing mouse Pointer to center of screen
glutWarpPointer(width/2,height/2);
glutSwapBuffers();
glutPostRedisplay();
}
void update(int value) {

//   glutPostRedisplay();
  glutTimerFunc(25,update, 0);
}
int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  
  glutInitWindowSize(800,800); 
  glutCreateWindow("SPM rockzzz");
  
  initRendering();
  glutDisplayFunc(drawScene);
//  glutSpecialFunc(handleKeyPress);
  glutMouseFunc(myMouseClick);
  glutKeyboardFunc(handleKeyPress);
  glutPassiveMotionFunc(motion);
  glutMotionFunc(motion);

  glutReshapeFunc(handleResize);
  
// glutTimerFunc(25,update, 0);

  
  glutMainLoop();

return 0;
}


#endif //MAIN
