#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include<iostream>
#include<stdio.h>
#include<cmath>

using namespace std;

int rt1,up1,rt2=0,up2=0;
float spin;
float b=2.5;
int p1sum=0;
int p2sum=0;
float d=5;     //cuboid width/2
float deg=30.0;
float x[10],y[10];

int dice1,dice2;
int p1flag=1;
int p2flag=0;
int windowWidth,windowHeight;
/* GLUT callback Handlers */
int angleX=0,angleY=0,angleZ=0;
void *currentfont;

//String Display Begin
void setFont(void *font) //function to change the font of the text
{
    currentfont = font;
}
void drawstring(float x, float y, float z, char *str)//To render the text on the screen
{
    char *c;
    glRasterPos3f(x, y, z);

    for (c = str;*c != '\0';c++)
    glutBitmapCharacter(currentfont, *c);
}

 void renderBitmapString(float x, float y,float z, void *font, char *string)
{
  char *c;
  glRasterPos3f(x,y,z);
  for (c=string; *c != '\0'; c++)
  {
    glutBitmapCharacter(font, *c);
  }
}
//String Display End

void reshape(int w, int h)
{
    glViewport(0, 0, windowWidth, windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,(float)w/(float)h, 1, 100);
    glutReshapeWindow(windowWidth,windowHeight);
    glMatrixMode(GL_MODELVIEW);

}

void spindDisplay()
{
  spin = spin+3.0;
  if (spin > 360)
  spin = spin -359;
  glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'q'    :   exit(0);
                        break;
    }
  glutPostRedisplay();
}

int gennum()
{
    int chancenum=rand()%6,dicenum;

    if(chancenum==0)
        dicenum=gennum();
    else
        dicenum=chancenum;
    printf("dicenum:%d\n",dicenum);
    return dicenum;
}

void leftclick()
{
    glutIdleFunc(spindDisplay);
    if(p1flag==1)
    {
        printf("\nPlayer1\n");
        dice1=gennum();
        printf("%d\n",dice1);
        if((p1sum+dice1)>100)
            dice1=0;
        p1sum+=dice1;
        printf("\nsum%d\n",p1sum);

        if(!(p1sum%10==0))
        {
            rt1=(p1sum%10);
            up1=(p1sum/10);
        }
        else
        {
            rt1=9;
            up1=(p1sum/10)-1;
        }

        p2flag=1;
        p1flag=0;
    }
    if(p2flag==1)
    {
        printf("\nPlayer2\n");
        dice2=gennum();
        printf("%d\n\n",dice2);
        if((p2sum+dice2)>100)
            dice2=0;
        p2sum+=dice2;
        printf("\nsum%d\n",p2sum);
        if(!(p2sum%10==0))
        {
            rt2=(p2sum%10);
            up2=(p2sum/10);
        }
        else
        {
            rt2=9;
            up2=(p2sum/10)-1;
        }
        p1flag=1;
        p2flag=0;
    }
}

void rightclick()
{
    glutIdleFunc(NULL);
    if(p1flag==1)
    {

     }
     else
    {

     }
}

void mouse (int button, int state, int x, int y)
{

    if(state == GLUT_DOWN)
    {
        switch(button)
        {
            case GLUT_LEFT_BUTTON   :   leftclick();
                                        break;

            case GLUT_RIGHT_BUTTON  :   rightclick();
                                        break;

            default                 :   break;
        }
    }
}

void drawSquare(int x,int y,int z)
{
    int num=30;
    glBegin(GL_POLYGON);
        glVertex3f(x-num,y+num,z);
        glVertex3f(x+num,y+num,z);
        glVertex3f(x+num,y-num,z);
        glVertex3f(x-num,y-num,z);
    glEnd();
}
void drawDice()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(30,0,-50.0);  //Translation motion along(x,y,z) axis
    //rotation after translation (order matters)
    glRotatef( angleX, 1.0, 0.0, 0.0);      //rotate the cube along x-axis
    glRotatef( angleY, 0.0, 1.0, 0.0);      //rotate along y-axis
    glRotatef( angleZ, 0.0, 0.0, 1.0);
    glPointSize(10.0);
    glTranslatef(-40,-5,-15.0);  //Translation motion along(x,y,z) axis

    int num = 1;
    //int swap;
    for(int j=0;j<10;j++)
        for(int i=0;i<10;i++)
        {
            /*if(num>=11 && num<=30)
            {
                swap=i;i=j;j=swap;
            }*/

            glColor3f(1.0,0,0);
            char buffer[10]={'\0'};
            sprintf(buffer, "%d", num);
            renderBitmapString(i+0.5, j+0.5, 50, GLUT_BITMAP_HELVETICA_18, buffer);

            glPointSize(4.0);
            glBegin(GL_LINE_LOOP);
            glColor3f(0.0,0.0,0.0);
                glVertex3f(i,j,50);
                glVertex3f(i,j+1,50);
                glVertex3f(i+1,j+1,50);
                glVertex3f(i+1,j,50);
            glEnd();

            num++;

        }

        glPointSize(20.0);
        glColor3f(0.0,0.0,1.0);
        glBegin(GL_POLYGON);
            int x=0,y=0,pi=3.14;
            float th=0,r=0.3;
            for(int i=0;i<360;i++)
            {
                glVertex3f((r*cos((pi/(float)180)*th))+0.4+rt1,(r*sin((pi/(float)180)*th))+0.5+up1,50);
                th=th+1;
            }
        glEnd();

        glColor3f(1.0,0.0,1.0);
        glBegin(GL_POLYGON);
            for(int i=0;i<360;i++)
            {
                glVertex3f((r*cos((pi/(float)180)*th))+0.6+rt2,(r*sin((pi/(float)180)*th))+0.5+up2,50);
                th=th+1;
            }
        glEnd();

        glTranslatef(40.0,0.0,0.0);
        glRotatef(spin, 1.0, 0.5, 1.0);

        //Dice Faces
        glColor3f(1,0,1);
        float deg=0.0;
        glBegin(GL_QUADS);

            //top
            glColor3f(1,0,1);
            glVertex3f(-d,d,+d); //-x,y,z
            glVertex3f(d,d,+d); //x,y,z
            glVertex3f(d,d,-d); //x,y,-z
            glVertex3f(-d,d,-d); //-x,y,-z

            //bottom
            glColor3f(1,0,0);
            glVertex3f(-d,-d,d); //-x,y=0,z
            glVertex3f(d,-d,d); //x,y=0,z
            glVertex3f(d,-d,-d); //x,y=0,-z
            glVertex3f(-d,-d,-d); //-x,y=0,-z

            //left
            glColor3f(0,1,0);
            glVertex3f(-d,-d,d); //-x,y=0,z
            glVertex3f(-d,-d,-d); //-x,y=0,-z
            glVertex3f(-d,d,-d); //-x,y,-z
            glVertex3f(-d,d,d); //-x,y,z

            //right
            glColor3f(1,1,0);
            glVertex3f(d,-d,d); //x,y=0,z
            glVertex3f(d,-d,-d); //x,y=0,-z
            glVertex3f(d,d,-d); //x,y,-z
            glVertex3f(d,d,d); //x,y,z

            //front
            glColor3f(0,1,1);
            glVertex3f(d,-d,d); //x,y=0,z
            glVertex3f(d,d,d); //x,y,z
            glVertex3f(-d,d,d); //-x,y,z
            glVertex3f(-d,-d,d); //-x,y=0,z

            //back
            glColor3f(0,0,1);
            glVertex3f(d,-d,-d); //x,y=0,z
            glVertex3f(d,d,-d); //x,y,z
            glVertex3f(-d,d,-d); //-x,y,z
            glVertex3f(-d,-d,-d); //-x,y=0,z

        glEnd();

        deg+=5.625;
}

void display(void)
{
    drawDice();
    glutSwapBuffers();
}

void init()
{
    glClearColor(1.0,1.0,1.0,1.0);
    glEnable(GL_DEPTH_TEST);  //enables DEPTH_TEST
    glDepthFunc(GL_LEQUAL);   //Lesser depth & EQUAL depth valued objects displayed in the front
    glPointSize(5.0);
    glOrtho(0,windowWidth,0,windowHeight,-50,50);
}
int main(int argc, char *argv[])
{
    for(int i=0;i<=10;i+=1)        x[i]=100+100*i;
    for(int j=0;j<=10;j+=1)        y[j]=100+100*j;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    windowWidth = glutGet(GLUT_SCREEN_WIDTH);
    windowHeight = glutGet(GLUT_SCREEN_HEIGHT);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(0,0);
    glutCreateWindow("snake & ladder");
    glutFullScreen();

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
