#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#define TIMER_ID 0
#define TIMER_INTERVAL 20


static int animation_ongoing;
static float x_cam=0;
static float dx=0.1;
static float y_cam=0.3;
static int movement_flag=0;

static void on_keyboard(unsigned char key, int x, int y);
static void on_display(void);
static void on_reshape(int width,int height);
static void on_timer(int value);


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
  glutKeyboardFunc(on_keyboard);
  glutReshapeFunc(on_reshape);
  glutDisplayFunc(on_display);
  
  initialize_lights();
  
  
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
  
  
  gluLookAt(
	    x_cam,2+y_cam,10,
	    x_cam,y_cam,0,
	    0,1,0
  );
  
  glPushMatrix();
	glColor3f(0,0,1);
	glutSolidCube(3);
  glPopMatrix();
  
  glutSwapBuffers();
  
}