#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#define TIMER_ID 0
#define TIMER_INTERVAL 20


static int animation_ongoing;
static float x_cam=0;
static float dx=0.1;
static float y_cam=0.3;
static float y_jump=0;
static float jump_speed = 0.5;
static float g=0.07;
static float t=0;
static int movement_flag=0;
static int jump_flag=0;
static int fall_flag=0;
static int floor=0;

static void on_keyboard(unsigned char key, int x, int y);
static void on_display(void);
static void on_reshape(int width,int height);
static void on_timer(int value);
    
    GLfloat light_position[] = { 1, 1, 1, 0 };
    GLfloat no_material[] = { 0, 0, 0, 1 };
    GLfloat material_ambient[] = { 0.7, 0.7, 0.7, 1 };
    GLfloat material_ambient_heterogeneous[] = { 0.8, 0.8, 0.2, 1 };
    GLfloat material_diffuse[] = { 0.1, 0.5, 0.8, 1 };
    GLfloat material_specular[] = { 1, 1, 1, 1 };
    GLfloat no_shininess[] = { 0 };
    GLfloat low_shininess[] = { 5 };
    GLfloat high_shininess[] = { 100 };
    GLfloat material_emission[] = { 0.3, 0.2, 0.2, 0 };



void makePlayer(void){
  GLdouble headRadius = 0.2;
  GLdouble bodyHeight = 0.4;
  
  //head
  glPushMatrix();
    glTranslatef(0,bodyHeight/2,0);
    glPushMatrix();
      glMaterialfv(GL_FRONT,GL_AMBIENT,no_material);
      glMaterialfv(GL_FRONT,GL_DIFFUSE,material_diffuse);
      glMaterialfv(GL_FRONT,GL_SPECULAR,no_material);
      glMaterialfv(GL_FRONT,GL_SHININESS,no_shininess);
      glMaterialfv(GL_FRONT,GL_EMISSION,material_emission);
      glColor3f(0.7,0.1,0.1);
      glTranslatef(0,headRadius*2,0);
      glutSolidSphere(headRadius,50,50);
    glPopMatrix();
  
  
  //body
    glPushMatrix();
      glMaterialfv(GL_FRONT,GL_AMBIENT,no_material);
      glMaterialfv(GL_FRONT,GL_DIFFUSE,material_diffuse);
      glMaterialfv(GL_FRONT,GL_SPECULAR,no_material);
      glMaterialfv(GL_FRONT,GL_SHININESS,no_shininess);
      glMaterialfv(GL_FRONT,GL_EMISSION,material_emission);
      glColor3f(0,0.5,1);
      glutSolidCube(bodyHeight);
    glPopMatrix();
  glPopMatrix();


  
}


static void initialize_lights(void){
  GLfloat light_position[] = {1,10,5,0};
  GLfloat light_ambient[] = {0.1,0.1,0.1,1};
  GLfloat light_diffuse[] = {0.7,0.7,0.7,1};
  GLfloat light_specular[] = {0.9,0.9,0.9,1};

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0,GL_POSITION,light_position);
  glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
  glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular);
}


int main(int argc, char** argv){
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

  glutCreateWindow("Run man, Run");
  glutFullScreen();
  glutKeyboardFunc(on_keyboard);
  glutReshapeFunc(on_reshape);
  glutDisplayFunc(on_display);

  initialize_lights();
  
  glClearColor(0.75,0.75,0.75,0);


  glEnable(GL_DEPTH_TEST);

  glutMainLoop();

  return 0;
}


static void on_keyboard(unsigned char key, int x, int y){

  switch(key){
    case 27 :
      printf("escape pressed\n");
      exit(EXIT_SUCCESS);
      break;
    case 'A':
    case 'a':
      movement_flag=-1;
      if(!animation_ongoing){
	glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
	animation_ongoing = 1;
      }
      glutPostRedisplay();
      break;
    case 'D':
    case 'd':
      movement_flag=1;
      if(!animation_ongoing){
	glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
	animation_ongoing = 1;
      }
      glutPostRedisplay();
      break;

    case 'S':
    case 's':
      movement_flag=0;
      glutPostRedisplay();
      break;
    
    
    case 'W':
    case 'w':
      if(fall_flag != 1){
	jump_flag =1;
	t=0;
	if(!animation_ongoing){
	  glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
	  animation_ongoing = 1;
	}
      }
      break;
  }
}



static void on_timer(int value){

  if(value != TIMER_ID)
    return;

  if(movement_flag ==1){
    x_cam = x_cam + dx;
  }

  if(movement_flag == -1){
    x_cam = x_cam - dx;
  }
  
  
  //skok
  if(jump_flag == 1 && fall_flag!=1){
    t+=0.2;
    jump_speed = jump_speed - g*t;
    y_jump=y_jump+jump_speed*t;
    if(jump_speed<=0){
      jump_flag=0;
      t=0;
    }
  }
  else{
    if(y_jump <=floor){
      fall_flag=0;
      jump_flag=0;
      t=0;
      y_jump=floor;
      jump_speed=0.5;
    }
  }
  
  if(jump_flag != 1 && y_jump > floor){
    fall_flag=1;
    jump_flag=0;
    t=t+0.2;
    jump_speed=g*t;
    
    if(jump_speed>0.5){
      jump_speed=0.5;
    }
    
    y_jump=y_jump-jump_speed;			
  }	
  else{
    fall_flag=0;
    t=0;
  }

  glutPostRedisplay();

  if(animation_ongoing){
    glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
  }
}


static void on_reshape(int width,int height){
  glViewport(0,0,width,height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(60,(float) width/height,1,60);
  glMatrixMode(GL_MODELVIEW);
}


static void on_display(void){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
    if(y_cam+1<y_jump){
    	y_cam=y_jump-1;
    }
    if(y_cam-1>y_jump){
    	y_cam=y_jump+1;
    }



  gluLookAt(
	    x_cam,2+y_cam,10,
	    x_cam,y_cam,0,
	    0,1,0
  );

  glPushMatrix();
	glTranslatef(x_cam,y_jump,1);
	makePlayer();
  glPopMatrix();

  glutSwapBuffers();

}
