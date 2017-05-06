#include <stdlib.h>
#include<stdio.h>
#include <stdio.h>
#include<cmath>
#include<GL/glut.h>

using namespace std;
float spin;
float b=2.5;
float r=20.0;  //circle "r"
float d=5;     //cuboid width/2
float deg=30.0;
float x[10],y[10];
int windowWidth,windowHeight;
/* GLUT callback Handlers */
int angleX=0,angleY=0,angleZ=0;
void *currentfont;
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
  spin = spin+0.5;
  if (spin > 360)
  spin = spin -359;
  glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
  switch(key) {
    case 'q':
            exit(0);
            break;
    case 'x':
      angleX = ( angleX + 10 ) % 360;
      break;
    case 'X':
      angleX = ( angleX - 10 ) % 360;
      break;
    case 'y':
      angleY = ( angleY + 10 ) % 360;
      break;
    case 'Y':
      angleY = ( angleY - 10 ) % 360;
      break;
    case 'z':
      angleZ = ( angleZ + 10 ) % 360;
      break;
    case 'Z':
      angleZ = ( angleZ - 10 ) % 360;
      break;
    case 'r':
        angleX=0,angleY=0,angleZ=0;
        break;
      glutPostRedisplay();
      }
  glutPostRedisplay();
}
void mouse (int button, int state, int x, int y)            //mouse function...
{
  switch(button) {
    case GLUT_LEFT_BUTTON:  if(state == GLUT_DOWN)
                                glutIdleFunc(spindDisplay);
                                break;
    case GLUT_RIGHT_BUTTON: if(state == GLUT_DOWN)
                              {
                                    glutIdleFunc(NULL);
                              }
                              break;
    default:break;
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //clears display with buffer color & depth values set in init()
    //circle
    glLoadIdentity();  //Loads identity matrix for each iteration of display
    glTranslatef(30,0,-50.0);  //Translation motion along(x,y,z) axis
    //rotation after translation (order matters)
    //glRotatef(30,1,1,1);  //(degree, x,y,z);
    glRotatef( angleX, 1.0, 0.0, 0.0);      //rotate the cube along x-axis
    glRotatef( angleY, 0.0, 1.0, 0.0);      //rotate along y-axis
    glRotatef( angleZ, 0.0, 0.0, 1.0);
   // boxdisplay();

    // deg+=1.5;
    glPointSize(10.0);
   glTranslatef(-40,-5,-15.0);  //Translation motion along(x,y,z) axis
   setFont(GLUT_BITMAP_HELVETICA_18);
    char name[]="sharma";
    drawstring(5,5,50,name);
    for(int i=0;i<9;i++)
        for(int j=0;j<9;j++)
        {
            glPointSize(4.0);
            glBegin(GL_LINE_LOOP);
            glVertex3f(i,j,50);
            glVertex3f(i,j+1,50);
            glVertex3f(i+1,j+1,50);
            glVertex3f(i+1,j,50);
            glEnd();
        }
      //  glTranslatef(30,0,-50.0);
        glTranslatef(40.0,0.0,0.0);
         glRotatef(spin, 1.0, 0.5, 1.0);

    glColor3f(1,0,1);
    float deg=0.0;
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
    for(int i=0;i<=10;i+=1)
        x[i]=100+100*i;
    for(int j=0;j<=10;j+=1)
        y[j]=100+100*j;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    windowWidth = glutGet(GLUT_SCREEN_WIDTH);
    windowHeight = glutGet(GLUT_SCREEN_HEIGHT);
    glutInitWindowSize(windowWidth,windowHeight);
    glutInitWindowPosition(0,0);
    glutCreateWindow("snake & ladder");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
     glutMouseFunc(mouse);
    glutMainLoop();

    return 0;
}
