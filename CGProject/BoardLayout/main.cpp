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
void loadImage(const char* name,int n);

void drawStrokeText(const char str[250],int x,int y,int z,float p1,float p2);

void drawoptions();
void selectoptions();
void windowOne();
void windowTwo();
void windowThree();
void drawMesh();
void drawplayer();
void drawdice();
void spindDisplay();
void gameplay();
void diceposition();
void mouse (int button, int state, int x, int y);

//Variables to be used in the program

int numplayers=0;
int windowWidth;
int windowHeight;
int flag=0;
bool window2=false,window3=false,window4=false;
int dice1,dice2;
int p1flag=1;
int p2flag=0;
float rt1=0,up1=0,rt2=0,up2=0,st1=-70,st2=-70;
int p1sum=0;
int p2sum=0;
int pc=0;
int dp=-1;

float d=50;
float spin;


int pixelwidth=700;
int pixelheight=850;
int WIDTH=500;
int HEIGHT=500;
// storage for image(pixel array)
vector <unsigned char> image_logo;
// image width and height
unsigned logowidth;
unsigned logoheight;

vector <unsigned char> image_board;
unsigned boardwidth;
unsigned boardheight;

GLuint texname;

//Glut functions and their user defined definitions
void init()
{
    glClearColor(0.0,0.0,0.0,0.0);
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
    loadImage("logo.png",1);
    loadImage("board.png",0);
    glGenTextures(1, &texname);
    glutInit(&argc, argv);
    glutInitWindowSize(WIDTH,HEIGHT);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
    glutCreateWindow("Snake and Ladders");
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    init();

    windowWidth=glutGet(GLUT_WINDOW_WIDTH);
    windowHeight=glutGet(GLUT_WINDOW_HEIGHT);

    glutFullScreen();
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(specialkeys);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);
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
void loadImage(const char* name,int n)
{
    //use lodepng decode to decode image
    int error;
    if(n)
    {
            if((error=lodepng::decode(image_logo,logowidth,logoheight,name)))
            {
                cout<<name<<":"<<lodepng_error_text(error)<<endl;
            }
            else
                invert(image_logo,logowidth,logoheight);
    }
    else
    {
            if((error=lodepng::decode(image_board,boardwidth,boardheight,name)))
            {
                cout<<name<<":"<<lodepng_error_text(error)<<endl;
                exit(1);
            }
            else
                invert(image_board,boardwidth,boardheight);
    }

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
    		glutStrokeCharacter(GLUT_STROKE_ROMAN , str[i]);
	 }
	 glPopMatrix();
}

// Game play user defined functions

void drawoptions()
{
    float cn=windowWidth/2;
    float transx=windowWidth/2;

    glColor3f(1.0,0.0,0.0);
    glPushMatrix();
    glTranslatef(transx,100,0);
        glRectf(cn-75,50.0,cn+75,150.0);
    glPopMatrix();

    glColor3f(0.0,1.0,0.0);
    glPushMatrix();
        glTranslatef(transx,100,0);
    glRectf(cn-350,50,cn-200,150);
    glPopMatrix();

    glColor3f(0.0,0.0,1.0);
    glPushMatrix();
    glTranslatef(transx,100,0);
        glRectf(cn+200,50,cn+350,150);
    glPopMatrix();
}

//Selecting the player box
void selectoptions()
{
    float cn=windowWidth/2;
    float fontsize=0.13;
    float transx=windowWidth/2;

    //Text for Player Selection buttons
    glColor3f(1.0,1.0,1.0);

    glPushMatrix();
    glTranslatef(transx,100,0);
        drawStrokeText("2 Players",cn-335.0,100.0,0.0,fontsize,fontsize);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(transx,100,0);
        drawStrokeText("3 Players",cn-60.0,100.0,0.0,fontsize,fontsize);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(transx,100,0);
        drawStrokeText("4 Players",cn+215.0,100.0,0.0,fontsize,fontsize);
    glPopMatrix();

    if(flag==0)
    {
        glPushMatrix();
        glTranslatef(transx,100,0);
        glColor3f(1.0,1.0,1.0);
        glLineWidth(2.0);
        glBegin(GL_LINE_LOOP);
            glVertex2f(cn-355,45.0);
            glVertex2f(cn-355,155.0);
            glVertex2f(cn-195,155.0);
            glVertex2f(cn-195,45.0);
        glEnd();
        glPopMatrix();
    }
    else if(flag==1)
    {
        glPushMatrix();
        glTranslatef(transx,100,0);
        glColor3f(1.0,1.0,1.0);
        glLineWidth(2.0);
        glBegin(GL_LINE_LOOP);
            glVertex2f(cn-80,45.0);
            glVertex2f(cn-80,155.0);
            glVertex2f(cn+80,155.0);
            glVertex2f(cn+80,45.0);
        glEnd();
        glPopMatrix();
    }
    else if(flag==2)
    {
        glPushMatrix();
        glTranslatef(transx,100,0);
        glColor3f(1.0,1.0,1.0);
        glLineWidth(2.0);
        glBegin(GL_LINE_LOOP);
            glVertex2f(cn+355,45.0);
            glVertex2f(cn+355,155.0);
            glVertex2f(cn+195,155.0);
            glVertex2f(cn+195,45.0);
        glEnd();
        glPopMatrix();
    }

}

void windowOne()
{
    glClear(GL_COLOR_BUFFER_BIT);
    float scale=0.70;
    drawoptions();
    selectoptions();
    glPushMatrix();
    //image begin
     glEnable(GL_TEXTURE_2D);
    setTexture(image_logo,logowidth,logoheight);
    glPushMatrix();
    glTranslatef(300,500,0);
    glScalef(scale,scale,1);
    glBegin(GL_POLYGON);
        glTexCoord2d(0,0);  glVertex2f(0,0);
        glTexCoord2d(0,1);  glVertex2f(0,logoheight);
        glTexCoord2d(1,1);  glVertex2f(logowidth,logoheight);
        glTexCoord2d(1,0);  glVertex2f(logowidth,0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    //image end
    glPopMatrix();
    glutSwapBuffers();
}

void windowTwo()
{
    glClear(GL_COLOR_BUFFER_BIT);
    float xpos=windowWidth/5;
    float ypos=windowHeight*3/4;
    float xtrans=windowWidth/6;
    float ytrans=windowHeight;
    float fontsize=0.13;

    /*glColor3f(1.0,1.0,1.0);
        glLineWidth(2.0);
        glBegin(GL_LINE_LOOP);
            glVertex2f(10,10);
            glVertex2f(windowWidth-10,10);
            glVertex2f(windowWidth-10,windowHeight);
            glVertex2f(10,windowHeight);
        glEnd();
    */

    glPushMatrix();
    glTranslatef(xtrans,ytrans,0);
    glLineWidth(3.0);
    glColor3f(0.0,1.0,0.0);
        drawStrokeText("Snake and Ladders - The Game of Chance",xpos,ypos,0,0.210,0.210);
        glBegin(GL_LINES);
            glVertex2f(xpos,ypos-15);glVertex2f(xpos+620,ypos-15);
        glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(xtrans,ytrans,0);
    glColor3f(0.698, 0.133, 0.133);
    glLineWidth(2.0);
        drawStrokeText("Here's a Snakes and ladders board game to play with your friends and family. ",xpos-150,ypos-100,0,0.17,0.17);
        glColor3f(1.0, 1.0, 0.0);
        drawStrokeText("RULES:",xpos*2.50+80,ypos-180,0,0.17,0.17);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(xtrans,ytrans,0);
    glColor3f(0.0,0.8,1.0);
    glLineWidth(1.3);
        drawStrokeText("1. Objective of the game is to get to the number 100 which is the final destination.",0,ypos-250,0,fontsize,fontsize);
        drawStrokeText("2. Each player puts their counter on the space near the arrow mark.",0,ypos-300,0,fontsize,fontsize);
        drawStrokeText("3. Take it in turns to play the dice. Counter forwards to the number of spaces shown on the dice.",0,ypos-350,0,fontsize,fontsize);
        drawStrokeText("4. If your counter lands at the bottom of a ladder, you can move up to the top of the ladder.",0,ypos-400,0,fontsize,fontsize);
        drawStrokeText("5. If your counter lands on the head of a snake, you must slide down to the bottom of the snake.",0,ypos-450,0,fontsize,fontsize);
        drawStrokeText("6. The first player to get to the space that says '100' is the winner.",0,ypos-500,0,fontsize,fontsize);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(xtrans,ytrans,0);
    glColor3f(1.000, 0.843, 0.000);
        drawStrokeText("Press P to Play.                  Press Q to Quit.",xpos*1.2,ypos-650,0,fontsize+0.03,fontsize+0.03);
    glPopMatrix();

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
    setTexture(image_board,boardwidth,boardheight);
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
    drawplayer();
    glPushMatrix();
        glTranslatef(900.0,200.0,0.0);
        glRotatef(spin, 1.0, 0.5, 1.0);
    if(dp<0)
    {
        drawdice();
    }
    if(dp>0)
    {
        diceposition();
    }
    glPopMatrix();
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

void drawplayer()
{
    glPointSize(200.0);
        glColor3f(1.0,0.0,1.0);
        glBegin(GL_POLYGON);
        int pi=3.14;
        float th=0,r=25;
        for(int i=0;i<360;i++){
            glVertex3f((r*cos((pi/(float)180)*th))+35+rt1+st1,(r*sin((pi/(float)180)*th))+42.5+up1,-100);

            th=th+1;
        }
        glEnd();

                glPointSize(200.0);
        glColor3f(0.0,0.0,1.0);
        glBegin(GL_POLYGON);

        for(int i=0;i<360;i++){
            glVertex3f((r*cos((pi/(float)180)*th))+35+rt2+st2,(r*sin((pi/(float)180)*th))+42.5+up2,-100);

            th=th+1;
        }
        glEnd();

}

void mouse (int button, int state, int x, int y)            //mouse function...
{
  switch(button) {
    case GLUT_LEFT_BUTTON:  if(state == GLUT_DOWN)
                            {   dp=-1;
                                glutIdleFunc(spindDisplay);

                            }
                        break;
    case GLUT_RIGHT_BUTTON: if(state == GLUT_DOWN)
                              {     dp=2;
                                  glutIdleFunc(diceposition);
                                pc++;
                                gameplay();
                              }
                              break;
    default:break;
  }
}


void drawdice()
{
    glColor3f(1,0,1);

    glBegin(GL_QUADS);
    //top
    glColor3f(1,0,1);
    glVertex3f(-d,d,+d); //-x,y,z
    //glColor3f(1,1,1);
    glVertex3f(d,d,+d); //x,y,z
    //glColor3f(1,0,1);
    glVertex3f(d,d,-d); //x,y,-z
    //glColor3f(1,0,0);
    glVertex3f(-d,d,-d); //-x,y,-z

    //bottom
    glColor3f(1,0,0);
    glVertex3f(-d,-d,d); //-x,y=0,z
    //glColor3f(0,1,1);
    glVertex3f(d,-d,d); //x,y=0,z
    //glColor3f(0,0,1);
    glVertex3f(d,-d,-d); //x,y=0,-z
    //glColor3f(0,1,1);
    glVertex3f(-d,-d,-d); //-x,y=0,-z

    //left
    glColor3f(0,1,0);
    glVertex3f(-d,-d,d); //-x,y=0,z
    //glColor3f(0,1,1);
    glVertex3f(-d,-d,-d); //-x,y=0,-z
    //glColor3f(1,0,0);
    glVertex3f(-d,d,-d); //-x,y,-z
    //glColor3f(1,1,0);
    glVertex3f(-d,d,d); //-x,y,z

    //right
    glColor3f(1,1,0);
    glVertex3f(d,-d,d); //x,y=0,z
    //glColor3f(0,0,1);
    glVertex3f(d,-d,-d); //x,y=0,-z
    //glColor3f(1,0,1);
    glVertex3f(d,d,-d); //x,y,-z
    //glColor3f(1,1,1);
    glVertex3f(d,d,d); //x,y,z

    //front
    glColor3f(0,1,1);
    glVertex3f(d,-d,d); //x,y=0,z
   //glColor3f(1,1,1);
    glVertex3f(d,d,d); //x,y,z
    //glColor3f(1,1,0);
    glVertex3f(-d,d,d); //-x,y,z
    //glColor3f(0,1,0);
    glVertex3f(-d,-d,d); //-x,y=0,z

    //back
    glColor3f(0,0,1);
    glVertex3f(d,-d,-d); //x,y=0,z
    //glColor3f(1,0,1);
    glVertex3f(d,d,-d); //x,y,z
    //glColor3f(1,0,0);
    glVertex3f(-d,d,-d); //-x,y,z
    //glColor3f(0,1,1);
    glVertex3f(-d,-d,-d); //-x,y=0,z
    glEnd();

}
void spindDisplay()
{
  spin = spin+15.0;
  if (spin > 360)
  spin = spin -359;
  glutPostRedisplay();
}

void gameplay()
{


                 if(p2flag==1 && ((pc%2)==0))
                                      {
                                        dice2=(rand()%6)+1;

                                            if((p2sum+dice2)>100)
                                            {
                                                p2flag=0;
                                                p1flag=1;
                                            }


                                      if((p2sum+dice2)<=99 && (st2==0))
                                      {


                                        p2sum+=dice2;
                                        if(p2sum==4 || p2sum==9 || p2sum==20 || p2sum==34  || p2sum==59 || p2sum==27 || p2sum==70 || p2sum==79 || p2sum==35)

                                        {
                                              if(p2sum==35)
                                                {   p2sum=43;
                                                    rt2=70*(p2sum%10);
                                                    up2=85*(p2sum/10);

                                                }


                                            if(p2sum==4)
                                                {   p2sum=16;
                                                rt2=70*(p2sum%10);
                                                up2=85*(p2sum/10);

                                                }

                                            if(p2sum==9)
                                                {   p2sum=39;
                                                rt2=70*(p2sum%10);
                                                up2=85*(p2sum/10);

                                                }
                                            if(p2sum==20)
                                                {   p2sum=41;
                                                    rt2=70*(p2sum%10);
                                                    up2=85*(p2sum/10);

                                                }
                                                  if(p2sum==34)
                                                {   p2sum=43;
                                                    rt2=70*(p2sum%10);
                                                    up2=85*(p2sum/10);

                                                }
                                                  if(p2sum==27)
                                                {   p2sum=83;
                                                    rt2=70*(p2sum%10);
                                                    up2=85*(p2sum/10);

                                                }
                                                  if(p2sum==59)
                                                {   p2sum=66;
                                                    rt2=70*(p2sum%10);
                                                    up2=85*(p2sum/10);

                                                    }
                                                  if(p2sum==70)
                                                {   p2sum=90;
                                                    rt2=70*(p2sum%10);
                                                    up2=85*(p2sum/10);

                                                }
                                                  if(p2sum==79)
                                                {   p2sum=99;
                                                    rt2=70*(p2sum%10);
                                                    up2=85*(p2sum/10);

                                                }

                                                p2flag=0;
                                                p1flag=1;
                                                printf("\n%d@@@",p2sum);
                                            }
                                        else
                                        {
                                            if(((p2sum/10)%2)!=0)
                                                    {
                                                        rt2=70*(9-(p2sum%10));
                                                    }
                                             else
                                             {
                                                 rt2=70*(p2sum%10);
                                             }
                                            up2=85*(p2sum/10);
                                            p2flag=0;
                                            p1flag=1;
                                            printf("\n%d@@@",p2sum);
                                        }
                                        if(p2sum==14 || p2sum==46 || p2sum==48 || p2sum==61 || p2sum==62 || p2sum==86 || p2sum==92 || p2sum==95 || p2sum==97)
                                        {
                                              if(p2sum==14)
                                                {   p2sum=5;
                                                    rt2=70*(p2sum%10);
                                                    up2=85*(p2sum/10);

                                                }
                                                  if(p2sum==46)
                                                {   p2sum=25;
                                                    rt2=70*(p2sum%10);
                                                    up2=85*(p2sum/10);

                                                }
                                                  if(p2sum==48)
                                                {   p2sum=29;
                                                    rt2=70*(p2sum%10);
                                                    up2=85*(p2sum/10);

                                                }
                                                  if(p2sum==61)
                                                {   p2sum=11;
                                                    rt2=70*(p2sum%10);
                                                    up2=85*(p2sum/10);

                                                }
                                                  if(p2sum==62)
                                                {   p2sum=50;
                                                    rt2=70*(p2sum%10);
                                                    up2=85*(p2sum/10);

                                                }
                                                  if(p2sum==86)
                                                {   p2sum=23;
                                                    rt2=70*(p2sum%10);
                                                    up2=85*(p2sum/10);

                                                }
                                                  if(p2sum==92)
                                                {   p2sum=72;
                                                    rt2=70*(p2sum%10);
                                                    up2=85*(p2sum/10);

                                                }
                                                  if(p2sum==95)
                                                {   p2sum=75;
                                                    rt2=70*(p2sum%10);
                                                    up2=85*(p2sum/10);

                                                }
                                                  if(p2sum==97)
                                                {   p2sum=77;
                                                    rt2=70*(p2sum%10);
                                                    up2=85*(p2sum/10);

                                                }
                                        }
                                    }

                                    if(st2==-70)
                                    {   if(dice2==6)
                                        {st2=0;
                                         p2flag=0;
                                         p1flag=1;
                                        }
                                    }
                            }

                                      if(p1flag==1  && ((pc%2)==1))
            {
                dice1=(rand()%6)+1;


                if((p1sum+dice1)>100)
                                            {
                                                p1flag=0;
                                                p2flag=1;
                                            }


                                      //check player reached dest or not
                                      if((p1sum+dice1)<=99 && (st1==0))
                                      {
                                        p1sum+=dice1;



                                        if(p1sum==4 || p1sum==9 || p1sum==20 || p1sum==34  || p1sum==59 || p1sum==27 || p1sum==70 || p1sum==79 )
                                        {

                                            if(p1sum==4)
                                                {   p1sum=16;
                                                rt1=70*(p1sum%10);
                                                up1=85*(p1sum/10);

                                                }

                                            if(p1sum==9)
                                                {   p1sum=39;
                                                rt1=70*(p1sum%10);
                                                up1=85*(p1sum/10);

                                                }
                                            if(p1sum==20)
                                                {   p1sum=41;
                                                    rt1=70*(p1sum%10);
                                                    up1=85*(p1sum/10);

                                                }
                                                  if(p1sum==34)
                                                {   p1sum=43;
                                                    rt1=70*(p1sum%10);
                                                    up1=85*(p1sum/10);

                                                }
                                                  if(p1sum==27)
                                                {   p1sum=83;
                                                    rt1=70*(p1sum%10);
                                                    up1=85*(p1sum/10);

                                                }
                                                  if(p1sum==59)
                                                {   p1sum=66;
                                                    rt1=70*(p1sum%10);
                                                    up1=85*(p1sum/10);

                                                    }
                                                  if(p1sum==70)
                                                {   p1sum=90;
                                                    rt1=70*(p1sum%10);
                                                    up1=85*(p1sum/10);

                                                }
                                                  if(p1sum==79)
                                                {   p1sum=99;
                                                    rt1=70*(p1sum%10);
                                                    up1=85*(p1sum/10);

                                                }

                                                p1flag=0;
                                                p2flag=1;
                                                printf("\n%d",p1sum);
                                            }
                                        else
                                        {    if(((p1sum/10)%2)!=0)
                                                    {
                                                        rt1=70*(9-(p1sum%10));
                                                    }
                                             else
                                             {
                                                 rt1=70*(p1sum%10);
                                             }
                                            up1=85*(p1sum/10);
                                            p1flag=0;
                                            p2flag=1;
                                            printf("\n%d",p1sum);
                                        }
                                        if(p1sum==14 || p1sum==46 || p1sum==48 || p1sum==61 || p1sum==62 || p1sum==86 || p1sum==92 || p1sum==95 || p1sum==97 || p1sum==35)
                                        {

                                              if(p1sum==35)
                                                {   p1sum=43;
                                                    rt1=70*(p1sum%10);
                                                    up1=85*(p1sum/10);

                                                }



                                              if(p1sum==14)
                                                {   p1sum=5;
                                                    rt1=70*(p1sum%10);
                                                    up1=85*(p1sum/10);

                                                }
                                                  if(p1sum==46)
                                                {   p1sum=25;
                                                    rt1=70*(p1sum%10);
                                                    up1=85*(p1sum/10);

                                                }
                                                  if(p1sum==48)
                                                {   p1sum=29;
                                                    rt1=70*(p1sum%10);
                                                    up1=85*(p1sum/10);

                                                }
                                                  if(p1sum==61)
                                                {   p1sum=11;
                                                    rt1=70*(p1sum%10);
                                                    up1=85*(p1sum/10);

                                                }
                                                  if(p1sum==62)
                                                {   p1sum=50;
                                                    rt1=70*(p1sum%10);
                                                    up1=85*(p1sum/10);

                                                }
                                                  if(p1sum==86)
                                                {   p1sum=23;
                                                    rt1=70*(p1sum%10);
                                                    up1=85*(p1sum/10);

                                                }
                                                  if(p1sum==92)
                                                {   p1sum=72;
                                                    rt1=70*(p1sum%10);
                                                    up1=85*(p1sum/10);

                                                }
                                                  if(p1sum==95)
                                                {   p1sum=75;
                                                    rt1=70*(p1sum%10);
                                                    up1=85*(p1sum/10);

                                                }
                                                  if(p1sum==97)
                                                {   p1sum=77;
                                                    rt1=70*(p1sum%10);
                                                    up1=85*(p1sum/10);

                                                }
                                        }
                                    }


                                    if(st1==-70)
                                    {   if(dice1==6)
                                        {   st1=0;
                                            p1flag=0;
                                            p2flag=1;
                                        }
                                    }
                }
}

void diceposition()
{
    spin=0;

    if((pc%2)==1) {
                        if(dice1==1){glColor3f(1.0,0.0,1.0);}
                        if(dice1==2){glColor3f(0.0,1.0,0.0);}
                        if(dice1==3){glColor3f(0.0,1.0,1.0);}
                        if(dice1==4){glColor3f(1.0,0.0,0.0);}
                        if(dice1==5){glColor3f(0.0,0.0,1.0);}
                        if(dice1==6){glColor3f(1.0,1.0,0.0);}
    }
      if( (pc%2)==0) {
                        if(dice2==1){glColor3f(1.0,0.0,1.0);}
                        if(dice2==2){glColor3f(0.0,1.0,0.0);}
                        if(dice2==3){glColor3f(0.0,1.0,1.0);}
                        if(dice2==4){glColor3f(1.0,0.0,0.0);}
                        if(dice2==5){glColor3f(0.0,0.0,1.0);}
                        if(dice2==6){glColor3f(1.0,1.0,0.0);}
    }


    glBegin(GL_QUADS);

    glVertex3f(d,-d,d); //x,y=0,z
   //glColor3f(1,1,1);
    glVertex3f(d,d,d); //x,y,z
    //glColor3f(1,1,0);
    glVertex3f(-d,d,d); //-x,y,z
    //glColor3f(0,1,0);
    glVertex3f(-d,-d,d); //-x,y=0,z


    glEnd();

}
