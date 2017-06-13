/*****    Header Files required in the program   *****/

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
#include "lodepng/lodepng.h"
#include "lodepng/lodepng.cpp"
#include<math.h>

using namespace std;

/*****    Variables to be used in the program   *****/

//Variables used for Window transitions and renderring
int windowWidth;
int windowHeight;
bool window1=false,window2=false,window3=false;

//Variables used for gameplay
float spin;                       //Stores spinning factor of the cube
int dice[4];                      //Stores dice values of players
int numplayers=0;                 //Stores number of players
int pc_counter=1;                 //Stores chances condition factor
int set_pointer=0;                //Set program counter
int select_flag=0;				        //Stores the user specified no. of players
int snake_pos[101];               //Stores snake heads in the mesh
int stair_pos[101];               //Stores ladders bottom in the mesh
int dice_position=-1;             //Stores the dice movement
float start[4]={-70};             //Start positions of the players
int player_sum[4]={0};            //Stores the current
float dice_dimension=50;          //Stores the dice
int player_flag[4]={1,0,0,0};     //Stores the player which has current chance
float right_movement[4]={0};      //Monitors player position horizontally
float up_movement[4]={0};         //Monitors player position vertically

/*****    Image renderring declarations   *****/

//Viewport and Window required variables
int pixelwidth=700;
int pixelheight=850;
int WIDTH=500;
int HEIGHT=500;

// storage for image(pixel array)

//Logo image and attributes
vector <unsigned char> image_logo;
unsigned logowidth;
unsigned logoheight;

//Board image and attributes
vector <unsigned char> image_board;
unsigned boardwidth;
unsigned boardheight;

//Dice 1 image and attributes
vector <unsigned char> image_dice1;
unsigned dice1width;
unsigned dice1height;

//Dice 2 image and attributes
vector <unsigned char> image_dice2;
unsigned dice2width;
unsigned dice2height;

//Dice 3 image and attributes
vector <unsigned char> image_dice3;
unsigned dice3width;
unsigned dice3height;

//Dice 4 image and attributes
vector <unsigned char> image_dice4;
unsigned dice4width;
unsigned dice4height;

//Dice 5 image and attributes
vector <unsigned char> image_dice5;
unsigned dice5width;
unsigned dice5height;

//Dice 6 image and attributes
vector <unsigned char> image_dice6;
unsigned dice6width;
unsigned dice6height;

//Texture to be used
GLuint texname;

/*****    Function prototypes involving Images   *****/

//For loading images
void setTexture(vector<unsigned char> img, unsigned width, unsigned height);
void invert(vector<unsigned char> &img,const unsigned width,const unsigned height);
void loadImage(const char* name,int n);

//For Stroke Drawing
void drawStrokeText(const char str[250],int x,int y,int z,float p1,float p2);

/*****    Function prototypes for User defined functions    *****/

//Required for First Window
void windowOne();
void drawoptions();
void selectoptions();

//Required for Second Window
void windowTwo();

//Required for Third Window
void windowThree();
void drawMesh();
void drawplayer();
void drawdice();
void spinDice();
void gameplay();
void diceposition();
void check_ladder();
void check_snake();

/***** Glut functions with changed definitions    *****/
static void init(void);
static void idle(void);
static void display(void);
static void key(unsigned char key,int x,int y);
static void specialkeys(int key,int x,int y);
void mouse(int button, int state, int x, int y);

/*****    Main Function   *****/
int main(int argc, char *argv[])
{
    //loading image to memory
      loadImage("logo.png",1);
      loadImage("board.png",0);

    //generating textures
      glGenTextures(1, &texname);

    //glut initialisation calls
      glutInit(&argc, argv);
      glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    //window attributes
      glutInitWindowSize(WIDTH,HEIGHT);
      glutInitWindowPosition(10,10);
      glutCreateWindow("Snake and Ladders");
      windowWidth=glutGet(GLUT_WINDOW_WIDTH);
      windowHeight=glutGet(GLUT_WINDOW_HEIGHT);

    //Other Initialisations
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);

    //Calls for functions
      init();
      glutFullScreen();
      glutDisplayFunc(display);
      glutKeyboardFunc(key);
      glutSpecialFunc(specialkeys);
      glutIdleFunc(idle);
      glutMouseFunc(mouse);

    //Continuous renderring the buffer
      glutMainLoop();

    return EXIT_SUCCESS;
}

/*****    Glut functions and their user defined definitions   *****/

static void init(void)
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
        select_flag=(select_flag+1)%3;
    }
    else if(key==GLUT_KEY_LEFT)
    {
        select_flag--;
        if(select_flag<0)
            select_flag=2;
    }
    numplayers=select_flag+2;
         printf("nop:%d\n",numplayers);
    printf("select_flag:%d\n",select_flag);
}

static void idle(void)
{
    glutPostRedisplay();
}

void mouse (int button, int state, int x, int y)            //mouse function...
{
    switch(button)
    {
        case GLUT_LEFT_BUTTON   :   if(state == GLUT_DOWN)
                                    {
                                        dice_position=-1;
                                        glutIdleFunc(spinDice);
                                        printf("%d$$",numplayers);
                                        if(numplayers==0)
                                        {
                                            numplayers=2;
                                        }
                                        if(set_pointer==0)
                                        {
                                            pc_counter=(numplayers-1);
                                            set_pointer++;
                                        }
                                    }
                                    break;
        case GLUT_RIGHT_BUTTON  :   if(state == GLUT_DOWN)
                                    {
                                        dice_position=2;
                                        glutIdleFunc(diceposition);
                                        pc_counter++;
                                        gameplay();
                                    }
                                    break;
        default                 :   break;
    }
}

/*****    Function definitions involving Images    *****/

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

/*****    Stroke Drawing    *****/

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

/*****    Game play user defined functions    *****/

/*****    Window One related functions  *****/

//Drawing options for selecting no. of players
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

//Selecting no. of players
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

    if(select_flag==0)
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
    else if(select_flag==1)
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
    else if(select_flag==2)
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

//Display function for Window One
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

/*****    Window Two related functions    *****/

//Display function for Window Two
void windowTwo()
{
    glClear(GL_COLOR_BUFFER_BIT);
    float xpos=windowWidth/5;
    float ypos=windowHeight*3/4;
    float xtrans=windowWidth/6;
    float ytrans=windowHeight;
    float fontsize=0.13;

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

/*****    Window Three related functions  *****/

//Function to draw Mesh over the given board
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

//Function to generate dice number
int generate_num()
{
    int chancenum,dicenum;
    chancenum=rand()%6;
    if(chancenum==0)
        dicenum=generate_num();
    else
        dicenum=chancenum;
    printf("dicenum:%d\n",dicenum);
    return dicenum;
}

//Function to draw Players
void drawplayer()
{
    glPointSize(200.0);
        glColor3f(1.0,0.0,1.0);
        glBegin(GL_POLYGON);
        int pi=3.14;
        float theta=0,radius=25;
        for(int i=0;i<360;i++){
            glVertex3f((radius*cos((pi/(float)180)*theta))+35+right_movement[0]+start[0],(radius*sin((pi/(float)180)*theta))+42.5+up_movement[0],-100);

            theta=theta+1;
        }
        glEnd();

                glPointSize(200.0);
        glColor3f(0.0,0.0,1.0);
        glBegin(GL_POLYGON);

        for(int i=0;i<360;i++){
            glVertex3f((radius*cos((pi/(float)180)*theta))+35+right_movement[1]+start[1],(radius*sin((pi/(float)180)*theta))+42.5+up_movement[1],-100);

            theta=theta+1;
        }
        glEnd();
         glPointSize(200.0);
        glColor3f(0.0,1.0,0.0);
        glBegin(GL_POLYGON);

        for(int i=0;i<360;i++){
            glVertex3f((radius*cos((pi/(float)180)*theta))+35+right_movement[2]+start[2],(radius*sin((pi/(float)180)*theta))+42.5+up_movement[2],-100);

            theta=theta+1;
        }
        glEnd();
         glPointSize(200.0);
        glColor3f(1.0,0.0,0.0);
        glBegin(GL_POLYGON);

        for(int i=0;i<360;i++){
            glVertex3f((radius*cos((pi/(float)180)*theta))+35+right_movement[3]+start[3],(radius*sin((pi/(float)180)*theta))+42.5+up_movement[3],-100);

            theta=theta+1;
        }
        glEnd();
}

//Function to Draw Dice
void drawdice()
{
    glColor3f(1,0,1);

    glBegin(GL_QUADS);
        //Top of the Dice
        glColor3f(1,0,1);
          glVertex3f(-dice_dimension,dice_dimension,+dice_dimension);
          glVertex3f(dice_dimension,dice_dimension,+dice_dimension);
          glVertex3f(dice_dimension,dice_dimension,-dice_dimension);
          glVertex3f(-dice_dimension,dice_dimension,-dice_dimension);

        //Bottom of the cube
        glColor3f(1,0,0);
          glVertex3f(-dice_dimension,-dice_dimension,dice_dimension);
          glVertex3f(dice_dimension,-dice_dimension,dice_dimension);
          glVertex3f(dice_dimension,-dice_dimension,-dice_dimension);
          glVertex3f(-dice_dimension,-dice_dimension,-dice_dimension);

        //Left of the cube
        glColor3f(0,1,0);
          glVertex3f(-dice_dimension,-dice_dimension,dice_dimension);
          glVertex3f(-dice_dimension,-dice_dimension,-dice_dimension);
          glVertex3f(-dice_dimension,dice_dimension,-dice_dimension);
          glVertex3f(-dice_dimension,dice_dimension,dice_dimension);

        //Right of the cube
        glColor3f(1,1,0);
          glVertex3f(dice_dimension,-dice_dimension,dice_dimension);
          glVertex3f(dice_dimension,-dice_dimension,-dice_dimension);
          glVertex3f(dice_dimension,dice_dimension,-dice_dimension);
          glVertex3f(dice_dimension,dice_dimension,dice_dimension);

        //Front of the cube
        glColor3f(0,1,1);
          glVertex3f(dice_dimension,-dice_dimension,dice_dimension);
          glVertex3f(dice_dimension,dice_dimension,dice_dimension);
          glVertex3f(-dice_dimension,dice_dimension,dice_dimension);
          glVertex3f(-dice_dimension,-dice_dimension,dice_dimension);

        //Back of the cube
        glColor3f(0,0,1);
          glVertex3f(dice_dimension,-dice_dimension,-dice_dimension);
          glVertex3f(dice_dimension,dice_dimension,-dice_dimension);
          glVertex3f(-dice_dimension,dice_dimension,-dice_dimension);
          glVertex3f(-dice_dimension,-dice_dimension,-dice_dimension);

    glEnd();
}

//Function to Spin Dice
void spinDice()
{
    spin = spin+50.0;
    if(spin > 360)
        spin-=359;
    glutPostRedisplay();
}

void gameplay()
{
    //Snake and Ladders Positions both points
    //snake positions={{16,6},{47,26},{49,30},{56,53},{62,19},{63,60},{87,24},{93,73},{95,75},{98,75}}
    //ladders positions={{1,38},{4,14},{9,31},{21,42},{28,84},{36,44},{51,67},{71,91},{80,100}}

    stair_pos[1]=38;stair_pos[4]=14;stair_pos[9]=31;stair_pos[21]=42;
    stair_pos[28]=84;stair_pos[36]=44;stair_pos[51]=67;stair_pos[71]=91;stair_pos[80]=100;

    snake_pos[16]=6;snake_pos[47]=26;snake_pos[49]=30;snake_pos[56]=53;snake_pos[62]=19;
    snake_pos[63]=60;snake_pos[87]=24;snake_pos[93]=73;snake_pos[95]=75;snake_pos[98]=78;

    if(player_flag[((pc_counter)%numplayers)]==1 )
    {
        printf("%d-->",numplayers);
        dice[((pc_counter)%numplayers)]=generate_num();

        if(( player_sum[((pc_counter)%numplayers)]+dice[((pc_counter)%numplayers)])>100)
        {
            player_flag[((pc_counter)%numplayers)]=0;
            player_flag[((pc_counter+1)%numplayers)]=1;
        }

        if(( player_sum[((pc_counter)%numplayers)]+dice[1])<=99 && (start[((pc_counter)%numplayers)]==0))
        {
            player_sum[((pc_counter)%numplayers)]+=dice[((pc_counter)%numplayers)];

            if(stair_pos[( player_sum[((pc_counter)%numplayers)]+1)]!=0)
            {
                //If Ladder is Found
                player_sum[((pc_counter)%numplayers)]=stair_pos[player_sum[((pc_counter)%numplayers)]+1]-1;

                if((( player_sum[((pc_counter)%numplayers)]/10)%2)!=0)
                {
                    right_movement[((pc_counter)%numplayers)]=70*(9-(player_sum[((pc_counter)%numplayers)]%10));
                }
                else
                {
                    right_movement[((pc_counter)%numplayers)]=70*( player_sum[((pc_counter)%numplayers)]%10);
                }

                up_movement[((pc_counter)%numplayers)]=85*( player_sum[((pc_counter)%numplayers)]/10);
                player_flag[((pc_counter)%numplayers)]=0;
                player_flag[((pc_counter+1)%numplayers)]=1;
            }
            else
            {
                if((( player_sum[((pc_counter)%numplayers)]/10)%2)!=0)
                {
                    right_movement[((pc_counter)%numplayers)]=70*(9-(player_sum[((pc_counter)%numplayers)]%10));
                }
                else
                {
                    right_movement[((pc_counter)%numplayers)]=70*( player_sum[((pc_counter)%numplayers)]%10);
                }

                up_movement[((pc_counter)%numplayers)]=85*( player_sum[((pc_counter)%numplayers)]/10);
                player_flag[((pc_counter)%numplayers)]=0;
                player_flag[((pc_counter+1)%numplayers)]=1;
            }

            if(snake_pos[player_sum[((pc_counter)%numplayers)]+1]!=0)
            {
                //If Snake is found
                player_sum[((pc_counter)%numplayers)]=snake_pos[player_sum[((pc_counter)%numplayers)]+1]-1;

                if((( player_sum[((pc_counter)%numplayers)]/10)%2)!=0)
                {
                    right_movement[((pc_counter)%numplayers)]=70*(9-( player_sum[((pc_counter)%numplayers)]%10));
                }
                else
                {
                    right_movement[((pc_counter)%numplayers)]=70*( player_sum[((pc_counter)%numplayers)]%10);
                }

                up_movement[((pc_counter)%numplayers)]=85*( player_sum[((pc_counter)%numplayers)]/10);
            }
        }

        if(start[((pc_counter)%numplayers)]==-70)
        {
            if(dice[((pc_counter)%numplayers)]==6)
            {
                start[((pc_counter)%numplayers)]=0;
                player_flag[((pc_counter)%numplayers)]=0;
                player_flag[((pc_counter+1)%numplayers)]=1;
            }
            else
            {
                player_flag[((pc_counter)%numplayers)]=0;
                player_flag[((pc_counter+1)%numplayers)]=1;
            }
        }
        printf("Program Counter : %d\n", pc_counter);
    }
}

//Function to show dice face
void diceposition()
{
    spin=0;

    //Player 1
    if((pc_counter%numplayers)==0)
    {
                        if(dice[0]==1){glColor3f(1.0,0.0,1.0);}
                        if(dice[0]==2){glColor3f(0.0,1.0,0.0);}
                        if(dice[0]==3){glColor3f(0.0,1.0,1.0);}
                        if(dice[0]==4){glColor3f(1.0,0.0,0.0);}
                        if(dice[0]==5){glColor3f(0.0,0.0,1.0);}
                        if(dice[0]==6){glColor3f(1.0,1.0,0.0);}
    }

    //Player 2
    if((pc_counter%numplayers)==1)
    {
                        if(dice[1]==1){glColor3f(1.0,0.0,1.0);}
                        if(dice[1]==2){glColor3f(0.0,1.0,0.0);}
                        if(dice[1]==3){glColor3f(0.0,1.0,1.0);}
                        if(dice[1]==4){glColor3f(1.0,0.0,0.0);}
                        if(dice[1]==5){glColor3f(0.0,0.0,1.0);}
                        if(dice[1]==6){glColor3f(1.0,1.0,0.0);}
    }

    //Player 3
    if((pc_counter%numplayers)==2)
    {
                        if(dice[2]==1){glColor3f(1.0,0.0,1.0);}
                        if(dice[2]==2){glColor3f(0.0,1.0,0.0);}
                        if(dice[2]==3){glColor3f(0.0,1.0,1.0);}
                        if(dice[2]==4){glColor3f(1.0,0.0,0.0);}
                        if(dice[2]==5){glColor3f(0.0,0.0,1.0);}
                        if(dice[2]==6){glColor3f(1.0,1.0,0.0);}
    }

    //Player 4
    if((pc_counter%numplayers)==3)
    {
                         if(dice[3]==1){glColor3f(1.0,0.0,1.0);}
                        if(dice[3]==2){glColor3f(0.0,1.0,0.0);}
                        if(dice[3]==3){glColor3f(0.0,1.0,1.0);}
                        if(dice[3]==4){glColor3f(1.0,0.0,0.0);}
                        if(dice[3]==5){glColor3f(0.0,0.0,1.0);}
                        if(dice[3]==6){glColor3f(1.0,1.0,0.0);}
    }

    glBegin(GL_QUADS);
      glVertex3f(60,-60,50);
      glVertex3f(60,60,50);
      glVertex3f(-60,60,50);
      glVertex3f(-60,-60,50);
    glEnd();

}

//Display function for Window Three
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
        if(dice_position<0)
            drawdice();
        if(dice_position>0)
            diceposition();
    glPopMatrix();

    glutSwapBuffers();
}
