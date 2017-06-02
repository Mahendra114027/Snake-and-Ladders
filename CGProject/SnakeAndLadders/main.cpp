#include <stdio.h>
#include <GL/glut.h>
#include"lodepng/lodepng.cpp"
#include"lodepng/lodepng.h"
#include <vector>
#include<GL/glext.h>
#include<iostream>

using namespace std;

int numplayers=0;
int windowWidth;
int windowHeight;
int flag=0;
bool window2=false,window3=false,window4=false;

static void displayFunction(void);
static void ResizeFunction(int, int);

void drawStrokeText(char str[250],int x,int y,int z,float p1,float p2)
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


void draw(GLenum mode=GL_RENDER)
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
void redraw(GLenum mode=GL_RENDER)
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

//image begin
int WIDTH=640; // window width & height
int HEIGHT=480;
vector <unsigned char> image; // storage for image(pixel array)
unsigned imageWidth;  // image width and height
unsigned imageHeight;
GLuint texname;
float scale=0.70;
float dx=0.0,dy=0.0;
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
    if((error=lodepng::decode(image,imageWidth,imageHeight,name)))
    {
        cout<<name<<":"<<lodepng_error_text(error)<<endl;
        exit(1);
    }
    else
        invert(image,imageWidth,imageHeight);

}
//image end


void specialkeys(int key,int x,int y)
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

void myKey(unsigned char key,int x,int y)
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


void windowOne()
{
    glClear(GL_COLOR_BUFFER_BIT);
    draw();
    redraw(flag);
    glPushMatrix();
    //image begin
     glEnable(GL_TEXTURE_2D);
    setTexture(image,imageWidth,imageHeight);
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
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.150, 0.200, 0.400,0.0);
    float xpos=windowWidth/5;
    float ypos=windowHeight*3/4;

    glLineWidth(4.0);
    glColor3f(1.0,1.0,1.0);
    drawStrokeText("Snake and Ladders - The Game of Chance",xpos,ypos,0,0.20,0.20);
    glFlush();
    glutSwapBuffers();
}


static void displayFunction()
{
    if(!window2)
        windowOne();
    else if(!window3)
        windowTwo();
    else
        windowThree();
}

static void ResizeFunction(int width, int height)
{
    windowHeight=height;
    windowWidth=width;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D (0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void myinit()
{
    glClearColor((23.0/255),(32.0/255),(42.0/255),0);
    gluOrtho2D(0,windowWidth,0,windowHeight);
}

void myidle()
{
    glutPostRedisplay();
}
int main(int argc, char *argv[])
{
    loadImage("logo.png");
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);

    windowWidth=glutGet(GLUT_SCREEN_WIDTH);
    windowHeight=glutGet(GLUT_SCREEN_HEIGHT);
    printf("%d   %d \n",windowWidth/2,windowHeight);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);

    glutInitWindowSize(windowWidth,windowHeight);
    glutInitWindowPosition(0,0);
    myinit();
    glutCreateWindow("Snakes and Ladders");
    glutFullScreen();

    glutSpecialFunc(specialkeys);
    glutKeyboardFunc(myKey);
    glutReshapeFunc(ResizeFunction);
    glutIdleFunc(myidle);
    glutDisplayFunc(displayFunction);
    glutMainLoop();
    return 0;
}
