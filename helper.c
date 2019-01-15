#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

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



void make_background(GLuint background,GLuint death,float min_x,float max_x,float min_y,float max_y,float far_z,float near_z){
      
    glBindTexture(GL_TEXTURE_2D,background);
    glBegin(GL_QUADS);
        glNormal3f(0,0,1);
        
        glTexCoord2f(12,6);
        glVertex3f(max_x,max_y,far_z);
        
        glTexCoord2f(12,0);
        glVertex3f(max_x,min_y,far_z);
        
        glTexCoord2f(0,0);
        glVertex3f(min_x,min_y,far_z);
        
        glTexCoord2f(0,6);
        glVertex3f(min_x,max_y,far_z);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D,death);
    glBegin(GL_QUADS);
        glNormal3f(0,1,0);
        
        glTexCoord2f(12,6);
        glVertex3f(max_x,min_y,near_z);
        
        glTexCoord2f(12,0);
        glVertex3f(max_x,min_y,far_z);
        
        glTexCoord2f(0,0);
        glVertex3f(min_x,min_y,far_z);
        
        glTexCoord2f(0,6);
        glVertex3f(min_x,min_y,near_z);
    glEnd();

    
    
    glBindTexture(GL_TEXTURE_2D,0);
}

void make_player(void){
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



void init_level(int* num, float** data , float* min_floor, float* max_floor){
	/*pomoc oko ideje za podloge (kako da ih zadajem i kasnije kako da postupam prilikom preskoka) 
     *dao kolega koji je radio slican projekat*/
    FILE* f=fopen("level.txt","r");
    if(f==NULL){
        fprintf(stderr,"fopen failed");
        exit(EXIT_FAILURE);
	}
    int n;
    fscanf(f,"%d",&n);
    *num=n;	
    if((*data=realloc(*data ,3 * n * sizeof(float)))==NULL){
        fprintf(stderr,"realloc failed");
        exit(EXIT_FAILURE);
    }
	
    fscanf(f,"%f",&(*data)[0]);
    fscanf(f,"%f",&(*data)[1]);
    fscanf(f,"%f",&(*data)[2]);
    *min_floor=(*data)[2];
    *max_floor=(*data)[2];
        
    int i;
    for(i=1;i<n;i++){
        fscanf(f,"%f",&(*data)[i*3]);
        fscanf(f,"%f",&(*data)[i*3+1]);
        fscanf(f,"%f",&(*data)[i*3+2]);
        if((*data)[i*3+2]<*min_floor)
            *min_floor=(*data)[i*3+2];
        if((*data)[i*3+2]>*max_floor)
            *max_floor=(*data)[i*3+2];			
    }
    fclose(f);
    return;
}


void make_block(GLuint name,float min_x, float max_x, float floor_y){

    glBindTexture(GL_TEXTURE_2D,name);
    glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);

        glTexCoord2f(12, 6);
        glVertex3f(max_x,floor_y,-1);

        glTexCoord2f(12, 0);
        glVertex3f(max_x,floor_y, 2);

        glTexCoord2f(0, 0);
        glVertex3f(min_x,floor_y, 2);

        glTexCoord2f(0, 6);
        glVertex3f(min_x,floor_y,-1);
    glEnd();
    
    	//pravimo prednju ivicu podloge
    	glBindTexture(GL_TEXTURE_2D,name);
     glBegin(GL_QUADS);
         glNormal3f(0, 0, 1);
 
         glTexCoord2f(12, 6);
         glVertex3f(max_x,floor_y  , 2);
 
         glTexCoord2f(12, 0);
         glVertex3f(max_x,floor_y-1, 2);
 
         glTexCoord2f(0, 0);
         glVertex3f(min_x,floor_y-1, 2);
 
         glTexCoord2f(0, 6);
         glVertex3f(min_x,floor_y  , 2);
     glEnd();
     
     	//leva ivica
     glBindTexture(GL_TEXTURE_2D,name);
     glBegin(GL_QUADS);
         glNormal3f(-1, 0, 0);
 
         glTexCoord2f(12, 6);
         glVertex3f(min_x,floor_y  , 2);
 
         glTexCoord2f(12, 0);
         glVertex3f(min_x,floor_y-1, 2);
 
         glTexCoord2f(0, 0);
         glVertex3f(min_x,floor_y-1,-1);
 
         glTexCoord2f(0, 6);
         glVertex3f(min_x,floor_y  ,-1);
     glEnd();
     
     	//desna ivica
     glBindTexture(GL_TEXTURE_2D,name);
     glBegin(GL_QUADS);
         glNormal3f(1, 0, 0);
 
         glTexCoord2f(12, 6);
         glVertex3f(max_x,floor_y  , 2);
 
         glTexCoord2f(12, 0);
         glVertex3f(max_x,floor_y-1, 2);
 
         glTexCoord2f(0, 0);
         glVertex3f(max_x,floor_y-1,-1);
 
         glTexCoord2f(0, 6);
         glVertex3f(max_x,floor_y  ,-1);
     glEnd();
     
     glBindTexture(GL_TEXTURE_2D, 0);	//iskljucujemo aktivnu teksturu
}

