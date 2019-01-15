#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "image.h"
#include "config.h"
#include "helper.h"

#define TIMER_ID 0
#define TIMER_INTERVAL 20
#define FILENAME0 "background.bmp"
#define FILENAME1 "brick.bmp"
#define FILENAME2 "death.bmp"



GLuint names[2];
static int animation_ongoing;


static void on_keyboard(unsigned char key, int x, int y);
static void on_display(void);
static void on_reshape(int width,int height);
static void on_timer(int value);
    
static void init_tex(void);



int main(int argc, char** argv){
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

  glutCreateWindow("Platform jumper");
  glutFullScreen();
  glutKeyboardFunc(on_keyboard);
  glutReshapeFunc(on_reshape);
  glutDisplayFunc(on_display);
  
  data=malloc(sizeof(float));
  init_level(&num,&data,&min_floor,&max_floor);
  y_jump=data[2];

  
  init_tex();
  init_lights();
  
  
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
  
  if(y_jump <= death_floor){
      
      printf("Game Over\n");
      free(data);
      exit(EXIT_SUCCESS);
}

  if(movement_flag ==1){
      //pomeramo se desno
    x_cam = x_cam + dx;
  }

  if(movement_flag == -1){
      //pomeramo se u levo
    x_cam = x_cam - dx;
  }
  
  
  //skok
  if(jump_flag == 1 && fall_flag!=1){
      
    t+=0.2;
    
    jump_speed = jump_speed - g*t;
    y_jump=y_jump+jump_speed*t;
    
    if(jump_speed<=0){
        fall_flag=1;
        jump_flag=0;
        t=0;
    }
  }
  
  else{
      
    if(death_flag==0 && y_jump <=floor){
      fall_flag=0;
      jump_flag=0;
      t=0;
      y_jump=floor;
      jump_speed=0.5;
    }
  }
  //pad
  if(jump_flag != 1 && (death_flag!=0 || y_jump > floor)){
      
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

  int i;
  
  for(i=0;i<num;i++)
    funcMakeBlock(names[1],data[3*i],data[3*i+1],data[3*i+2]);
  
  
  //inicijalizacija promenljivih
  if(floor_counter>=0 && floor_counter<num && change_floors==1){
      if(floor_counter==num-1){
          prev_x_r=curr_x_r;
          curr_x_l=next_x_l;
          curr_x_r=data[3*floor_counter+1];
          floor=data[3*floor_counter+2];
          next_x_l=curr_x_r + 200;
          
          change_floors=0;
    }
  else{
          next_x_l=data[3*(floor_counter+1)];
          curr_x_l=data[3*floor_counter];
          curr_x_r=data[3*floor_counter+1];
          floor=data[3*floor_counter+2];
          next_floor=data[3*(floor_counter+1)+2];
          
          if(floor_counter==0){
              prev_x_r=curr_x_l - 200;
              prev_floor=death_floor;
              
        }
        else{
            prev_x_r=data[3*(floor_counter-1)+1];
            prev_floor=data[3*(floor_counter-1)+2];
        }
          
        change_floors=0;

        
    }
}
  
  if(x_cam+0.2 >= curr_x_l && x_cam-0.2 <= curr_x_r && y_jump >= floor){
      death_flag=0;
}
    //umire levo
  if(x_cam + 0.2 < curr_x_l && x_cam - 0.2 > prev_x_r){
        death_flag=1;
    }
    //umire desno
  if(x_cam - 0.2 > curr_x_r && x_cam + 0.2 < next_x_l){
        death_flag=1;
    }
    
    //sledeca podloga
    
  if(x_cam + 0.2 >= next_x_l && y_jump >= next_floor){
        change_floors=1;
        floor_counter++;
        glutPostRedisplay();
    }
    //ukoliko ne moze da dohvati podlogu u skoku
  if(x_cam + 0.2 >= next_x_l && y_jump < next_floor){
        
        //velicina igraca je 0.8, podloge je 1
        if(y_jump > next_floor - 1.8){
            x_cam=next_x_l-0.2;
        }
    }
  
    //prethodna podloga
    
  if(x_cam - 0.2 <= prev_x_r && y_jump >= next_floor){
        change_floors=1;
        floor_counter--;
        glutPostRedisplay();
    }
    //ukoliko ne moze da dohvati podlogu u skoku
  if(x_cam - 0.2 <= prev_x_r && y_jump < next_floor){
        if(y_jump > prev_floor - 1.8){
            x_cam=prev_x_r-0.2;
        }
    }
    
    //kraj igre
  if(floor_counter==(num-1) && y_jump==floor){
        free(data);
        printf("Congratulations\n");
        exit(EXIT_SUCCESS);
    }

  
  //igrac
  glPushMatrix();
	glTranslatef(x_cam,y_jump,1);
	makePlayer();
  glPopMatrix();
  
  
  //pozadina
  death_floor = min_floor-0.5;
  makeBackground(names[0],names[2],data[0]-20,data[3*num-2]+30,death_floor,max_floor+10,-6,8);

  glutSwapBuffers();

}

static void init_tex(void)
{
    //kod preuzet sa vezbi
    /* Objekat koji predstavlja teskturu ucitanu iz fajla. */
    Image * image;

    /* Ukljucuju se teksture. */
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);

    /*
     * Inicijalizuje se objekat koji ce sadrzati teksture ucitane iz
     * fajla.
     */
    image = image_init(0, 0);

    
    /* Generisu se identifikatori tekstura. */
    glGenTextures(2, names);
    
    
    /* Kreira se tekstura pozadina. */
    image_read(image, FILENAME0);
    
    glBindTexture(GL_TEXTURE_2D, names[0]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    /* Kreira se tekstura podloge. */
    image_read(image, FILENAME1);

    glBindTexture(GL_TEXTURE_2D, names[1]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    
    image_read(image, FILENAME2);
    
    glBindTexture(GL_TEXTURE_2D, names[2]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);


	
	
    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat za citanje tekstura iz fajla. */
    image_done(image);

    /* Inicijalizujemo matricu rotacije. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
}

static void init_lights(void){
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


