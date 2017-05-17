#include <stdio.h>
#include <GL/glut.h>
#define SIZE 512

static int boxspace=200;
int windowWidth=GLUT_SCREEN_WIDTH;
int windowHeight=GLUT_SCREEN_HEIGHT;
int x1=(int)(windowWidth-900)/2,x2=x1+130;

// function prototypes
static void displayFunction(void);
static void ResizeFunction(int, int);
static void mouseButton(int, int, int, int);

void draw(GLenum mode=GL_RENDER)
{
    glLoadName(1);
    glColor3f(1.0,0.0,0.0);
    glRectf(125, 50, 230, 120);

    glLoadName(2);
    glColor3f(0.0,1.0,0.0);
    glRectf(125+boxspace, 50, 230+boxspace, 120);

    glLoadName(3);
    glColor3f(0.0,0.0,1.0);
    glRectf(125+boxspace+boxspace, 50, 230+boxspace+boxspace, 120);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutInitWindowSize(windowWidth,windowHeight);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(500,500);
    glutCreateWindow("Snake and Ladders");
    glutReshapeFunc(ResizeFunction);
    glutMouseFunc(mouseButton);
    glutDisplayFunc(displayFunction);
    glClearColor((23.0/255),(32.0/255),(42.0/255),0);
    gluOrtho2D(0,windowWidth,0,windowHeight);
    glutMainLoop();

    return 0;
}

static void displayFunction()
{
    glClear(GL_COLOR_BUFFER_BIT);
    draw();
    glutSwapBuffers();
}

static void ResizeFunction(int width, int height)
{
    //////////////////////////////////////////////////////
    ////// NEVER DELETE THIS SECTION - SINCE OBJECT //////
    ////// SELECTION DOESN'T WORK !!!!!!!!!!!!!!!!! //////
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D (0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    /////////////////////////////////////////////////////
}

// this function is used to detect which object is hit,
void processHits (GLint hits, GLuint buffer[])
{
   unsigned int i, j;
   GLuint names, *ptr;
   if(hits == 0)
        printf("No square is hit & ");
   ptr = (GLuint *) buffer;
   for (i = 0; i < hits; i++)
   {
      names = *ptr;
      ptr+=3;
      for (j = 0; j < names; j++)
      {
        if(*ptr == 1)
            printf("Red square is hit & ");
        else if(*ptr == 2)
            printf("Green square is hit & ");
        else if(*ptr == 3)
            printf("Blue square is hit & ");
        ptr++;
      }
   }
   printf("\n");
}

static void mouseButton(int button, int state, int x, int y)
{
    GLuint selectBuf[SIZE];
    GLint hits;
    GLint viewport[4];

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        glGetIntegerv (GL_VIEWPORT, viewport);

        glSelectBuffer (SIZE, selectBuf);
        glRenderMode(GL_SELECT);

        glInitNames();
        glPushName(0);

        glMatrixMode (GL_PROJECTION);
        glPushMatrix ();
        glLoadIdentity ();
        /* create 5x5 pixel picking region near cursor location */
        gluPickMatrix ((GLdouble) x, (GLdouble) (viewport[3] - y),
                      5.0, 5.0, viewport);
        gluOrtho2D (0, windowWidth, 0, windowHeight);
        draw(GL_SELECT);


        glMatrixMode (GL_PROJECTION);
        glPopMatrix ();
        glFlush ();

        hits = glRenderMode (GL_RENDER);
        processHits (hits, selectBuf);

        glutPostRedisplay();
        glutSwapBuffers();
    }
}
