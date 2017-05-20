#include <stdio.h>
#include <GL/glut.h>
#include"lodepng/lodepng.cpp"
#include"lodepng/lodepng.h"
#include <vector>
#include<GL/glext.h>
#include<iostream>

using namespace std;

int windowWidth;
int windowHeight;
int flag=0;

static void displayFunction(void);
static void ResizeFunction(int, int);

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

void redraw(GLenum mode=GL_RENDER)
{
    float cn=windowWidth/2;
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

    printf("%d\n",flag);
}


static void displayFunction()
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

    glutInitWindowSize(windowWidth,windowHeight);
    glutInitWindowPosition(0,0);
    myinit();
    glutCreateWindow("Snake and Ladders");
    glutFullScreen();

    glutSpecialFunc(specialkeys);
    glutReshapeFunc(ResizeFunction);
    glutIdleFunc(myidle);
    glutDisplayFunc(displayFunction);
    glutMainLoop();
    return 0;
}
