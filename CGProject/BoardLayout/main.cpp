#ifdef __APPLE__
#include <GLUT/glut.h>
#include <GLUT/glext.h>
#else
#include <GL/glut.h>
#include <GL/glext.h>
#endif

#include<stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "lodepng.h"
#include "lodepng.cpp"
#include<math.h>

using namespace std;

//Function prototypes

void setTexture(vector<unsigned char> img, unsigned width, unsigned height);
void invert(vector<unsigned char> &img,const unsigned width,const unsigned height);
void loadImage(const char* name);

void drawStrokeText(const char str[250],int x,int y,int z,float p1,float p2);

void drawoptions();
void selectoptions();
void windowOne();
void windowTwo();
void windowThree();
void drawMesh();

//Variables to be used in the program

int numplayers=0;
int windowWidth;
int windowHeight;
int flag=0;
bool window2=false,window3=false,window4=false;

int pixelwidth=700;
int pixelheight=850;
int WIDTH=500;
int HEIGHT=500;
// storage for image(pixel array)
vector <unsigned char> image_logo;
vector <unsigned char> image_board;
// image width and height
unsigned imageWidth;
unsigned imageHeight;
GLuint texname;
float dx=0.0,dy=0.0;

//Glut functions and their user defined definitions
void init()
{
    glClearColor(0,0,0,0);
    glViewport(0, 0,WIDTH, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,1000,0,1000,0,1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}
static void display(void)
{
    if(!window2)
        windowOne();
    else if(!window3)
        windowTwo();
    else
        windowThree();
}

static void key(unsigned char key,int x,int y)
{
    if(key=='q' || key=='Q' || key==27)
    {
        exit(1);
    }
    else if(key==13)
    {
        window2=true;
    }
    else if(key=='p' || key=='P')
    {
        window3=true;
    }
}

static void specialkeys(int key,int x,int y)
{
    if(key==GLUT_KEY_RIGHT)
    {
        flag=(flag+1)%3;
    }
    else if(key==GLUT_KEY_LEFT)
    {
        flag--;
        if(flag<0)
            flag=2;
    }
    numplayers=flag+2;
         printf("nop:%d\n",numplayers);
    printf("flag:%d\n",flag);
}

static void idle(void)
{
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    //load image to memory

    if(!window2 &&!window3)
        loadImage("logo.png");
    else if(window3)
        loadImage("board.png");

    glutInit(&argc, argv);
    glutInitWindowSize(WIDTH,HEIGHT);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
    glutCreateWindow("Snake and Ladders");
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    init();

    glutFullScreen();
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(specialkeys);
    glutIdleFunc(idle);
    glutMainLoop();
    return EXIT_SUCCESS;
}



//Functions definitions

//Functions used in image loading
/** Sets current texture to given image
    @param img is image vector that has already been loaded
    @param width is width of the image
    @param height is height of image
*/
void setTexture(vector<unsigned char> img, unsigned width, unsigned height)
{
    glGenTextures(1, &texname);
    glBindTexture(GL_TEXTURE_2D, texname);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // without this texture darkens
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                0, GL_RGBA, GL_UNSIGNED_BYTE, &img[0]);
}
/** OpenGL seems to draw images vertically flipped
        this function inverts our data so that it displays correctly
        @param img is our image data vector
        @param width is our image width
        @param height is our image height
*/
void invert(vector<unsigned char> &img,const unsigned width,const unsigned height)
{
    unsigned char *imageptr = &img[0];
    unsigned char *first = NULL;
    unsigned char *last = NULL;
    unsigned char temp = 0;
    for( int h = 0; h <(int) height/2; ++h )
    {

        first = imageptr + h * width * 4;
        last = imageptr + (height - h - 1) * width*4;
        for( int i = 0; i < (int)width*4; ++i )
        {
            temp = *first;
            *first = *last;
            *last = temp;
            ++first;
            ++last;
        }
    }
}
/**
Load image into memory
*/
void loadImage(const char* name)
{
    //use lodepng decode to decode image
    int error;
    if((error=lodepng::decode(image_logo,imageWidth,imageHeight,name)))
    {
        cout<<name<<":"<<lodepng_error_text(error)<<endl;
    }
    else
        invert(image_logo,imageWidth,imageHeight);

    if((error=lodepng::decode(image_board,imageWidth,imageHeight,name)))
    {
        cout<<name<<":"<<lodepng_error_text(error)<<endl;
        exit(1);
    }
    else
        invert(image_board,imageWidth,imageHeight);
}

//String Writing functions

void drawStrokeText(const char str[250],int x,int y,int z,float p1,float p2)
{
      int i;
	 glPushMatrix();
	 glTranslatef(x, y,z);
	 glScalef(p1,p2,z);

	 for (i=0;str[i]!='\0';i++)
	 {
    		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN , str[i]);
	 }
	 glPopMatrix();
}

// Game play user defined functions

void drawoptions()
{
    float cn=windowWidth/2;
    glColor3f(1.0,0.0,0.0);
    glRectf(cn-75,70.0,cn+75,150.0);

    glColor3f(0.0,1.0,0.0);
    glRectf(cn-350,70,cn-200,150);

    glColor3f(0.0,0.0,1.0);
    glRectf(cn+200,70,cn+350,150);
}

//Selecting the player box
void selectoptions()
{
    float cn=windowWidth/2;
    float fontsize=0.13;

    //Text for Player Selection buttons
    glColor3f(1.0,1.0,1.0);
    drawStrokeText("2 Players",cn-335.0,100.0,0.0,fontsize,fontsize);
    drawStrokeText("3 Players",cn-60.0,100.0,0.0,fontsize,fontsize);
    drawStrokeText("4 Players",cn+215.0,100.0,0.0,fontsize,fontsize);

    if(flag==0)
    {
        glColor3f(1.0,1.0,1.0);
        glLineWidth(2.0);
        glBegin(GL_LINE_LOOP);
            glVertex2f(cn-355,65.0);
            glVertex2f(cn-355,155.0);
            glVertex2f(cn-195,155.0);
            glVertex2f(cn-195,65.0);
        glEnd();
    }
    else if(flag==1)
    {
        glColor3f(1.0,1.0,1.0);
        glLineWidth(2.0);
        glBegin(GL_LINE_LOOP);
            glVertex2f(cn-80,65.0);
            glVertex2f(cn-80,155.0);
            glVertex2f(cn+80,155.0);
            glVertex2f(cn+80,65.0);
        glEnd();
    }
    else if(flag==2)
    {   glColor3f(1.0,1.0,1.0);
        glLineWidth(2.0);
        glBegin(GL_LINE_LOOP);
            glVertex2f(cn+355,65.0);
            glVertex2f(cn+355,155.0);
            glVertex2f(cn+195,155.0);
            glVertex2f(cn+195,65.0);
        glEnd();
    }
}

void windowOne()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0,0.0,0.0,0.0);
    float scale=0.70;
    drawoptions();
    selectoptions();
    glPushMatrix();
    //image begin
     glEnable(GL_TEXTURE_2D);
    setTexture(image_logo,imageWidth,imageHeight);
    glPushMatrix();
    glTranslatef(windowWidth/2-250,400,0);
    glScalef(scale,scale,1);
    glBegin(GL_POLYGON);
        glTexCoord2d(0,0);  glVertex2f(0,0);
        glTexCoord2d(0,1);  glVertex2f(0,imageHeight);
        glTexCoord2d(1,1);  glVertex2f(imageWidth,imageHeight);
        glTexCoord2d(1,0);  glVertex2f(imageWidth,0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    //image end
    glPopMatrix();
    glutSwapBuffers();
}

void windowTwo()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.150, 0.200, 0.400,0.0);
    float xpos=windowWidth/5;
    float ypos=windowHeight*3/4;

    glLineWidth(4.0);
    glColor3f(1.0,1.0,1.0);
    drawStrokeText("Snake and Ladders - The Game of Chance",xpos,ypos,0,0.210,0.210);

    glColor3f(0.698, 0.133, 0.133);
    glLineWidth(2.0);
    drawStrokeText("RULES:",xpos*2.5-15,ypos-90,0,0.17,0.17);

    glColor3f(0.0,0.8,1.0);
    glLineWidth(1.0);
    drawStrokeText("1. Objective of the game is to get the number 2048.",20,180,0,0.08,0.08);
    drawStrokeText("2. You will have a grid of 16 tiles.",20,160,0,0.08,0.08);
    drawStrokeText("3. Move using arrow keys to join two equal numbers.",20,140,0,0.09,0.08);
    drawStrokeText("4. When two equal numbers are in touch, they will add up.",20,120,0,0.08,0.08);
    drawStrokeText("5. If there are no free tiles on our grid, the game ends.",20,100,0,0.08,0.08);
    glColor3f(1.000, 0.843, 0.000);
    drawStrokeText("Press P to Play.                  Press Q to Quit.",20,60,0,0.08,0.08);
    glFlush();
    glutSwapBuffers();
}

void windowThree()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0,0.0,0.0,0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    setTexture(image_board,imageWidth,imageHeight);
    glPushMatrix();
    glTranslatef(30,80,0);
    glScalef(0.9,1,1);
    glBegin(GL_POLYGON);
        glTexCoord2d(0,0);  glVertex2f(0,0);
        glTexCoord2d(0,1);  glVertex2f(0,pixelheight);
        glTexCoord2d(1,1);  glVertex2f(pixelwidth,pixelheight);
        glTexCoord2d(1,0);  glVertex2f(pixelwidth,0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    drawMesh();
    glutSwapBuffers();
}

void drawMesh()
{
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(31,81,-50.0);  //not to delete
    glPointSize(10.0);
    glScalef(0.9,1,1);
    for(int i=0;i<pixelwidth;i+=70)
        for(int j=0;j<pixelheight;j+=85)
        {
            glPointSize(4.0);
            glColor3f(1.0,0.0,0.0);
            glBegin(GL_LINE_LOOP);
                glVertex3f(i,j,50);
                glVertex3f(i,j+85,50);
                glVertex3f(i+70,j+85,50);
                glVertex3f(i+70,j,50);
            glEnd();
        }
}


