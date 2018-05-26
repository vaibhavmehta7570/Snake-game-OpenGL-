#include <ctime>
#include <deque>
#include <GL/glut.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
// A macro for unused variables (to bypass those pesky G++ warnings)
#define UNUSED(param) (void)(param)
#include<stdio.h>
// Snake direction macros
#define UP    1
#define DOWN  2
#define LEFT  3
#define RIGHT 4

char title[] = "OpenGL Snake";
int score=-1;
int res=-1;
float map_half_length = 30.0f;
int ch=0;

int direction = DOWN;
int move_speed = 110;
bool moved = false;
std::deque< std::deque<float> > part_coords;

bool food_available = false;
int food_coords[2];

int growth_stage = 0;
int growth = 2;
int count=-1;
void DrawString(void *font,const char s[],float x,float y)
{
    unsigned int i;
    glColor3f(1.0,1.0,0.0);
    glRasterPos2f(x,y);
    for(i=0;i<strlen(s);i++)
    {
        glutBitmapCharacter(font,s[i]);
    }
}
void instruction(){
    
    DrawString(GLUT_BITMAP_HELVETICA_18, "INSTRUCTIONS", 250, 550);
    DrawString(GLUT_BITMAP_HELVETICA_18, "1. Don't run the snake into the wall, or its own tail: you die.", 50, 500);
    DrawString(GLUT_BITMAP_HELVETICA_18, "2. Use your cursor keys: up, left, right, and down.", 50, 450);
    DrawString(GLUT_BITMAP_HELVETICA_18, "3. Eat the red colored apples to gain points.", 50, 400);
    DrawString(GLUT_BITMAP_HELVETICA_18, " GOOD LUCK :)", 250, 230);
    
    
    DrawString(GLUT_BITMAP_HELVETICA_18, "Press enter key to Start the game",10, 100);
    DrawString(GLUT_BITMAP_HELVETICA_18, "Press 3 to exit the game", 350, 100);
    
}

void frontscreen()
{
    DrawString(GLUT_BITMAP_HELVETICA_18, "1. Press ENTER to start the game", 105, 580);
    DrawString(GLUT_BITMAP_HELVETICA_18, "2. Press 2 for Instructions", 105, 550);
    DrawString(GLUT_BITMAP_HELVETICA_18, "3. Press 3 to Exit the game", 105, 520);
    
    DrawString(GLUT_BITMAP_HELVETICA_12,"Maximize window for better view",45,5);
    DrawString(GLUT_BITMAP_HELVETICA_18, "SAI VIDYA INSTITUTE OF THECHNOLOGY", 120, 400);
    DrawString(GLUT_BITMAP_HELVETICA_18, "Mini Project On:-", 10, 130);
    DrawString(GLUT_BITMAP_HELVETICA_18, "Snake Clone Game", 10, 110);
    DrawString(GLUT_BITMAP_HELVETICA_18, "Developed By:-", 10, 80);
    DrawString(GLUT_BITMAP_HELVETICA_18, "Vaibhav Mehta & Siddharth Singh", 10, 60);
    DrawString(GLUT_BITMAP_HELVETICA_18, "Under the guidence of:-", 380, 80);
    DrawString(GLUT_BITMAP_HELVETICA_18, "Prof Sukuruth Gowda M A", 380, 60);
    
    
    
}
void spawnFood(){
    if(!food_available){
        while(true){
            bool collides = false;

            // Produce a temporary random coordinate
            int temp_food_coords[2] = { food_coords[0] = 2 * (rand() % ((int) map_half_length + 1)) - (int) map_half_length,
                                        food_coords[1] = 2 * (rand() % ((int) map_half_length + 1)) - (int) map_half_length };

            // Does it collide with the snake?
            for(unsigned int a = 0; a < part_coords.size(); a++){
                if(temp_food_coords[0] == part_coords[a][0] &&
                   temp_food_coords[1] == part_coords[a][1]){
                    collides = true;
                }
            }

            // If it doesn't collide with the snake, then make it the real food coordinates
            if(collides == false){
                food_coords[0] = temp_food_coords[0];
                food_coords[1] = temp_food_coords[1];

                food_available = true;
                score=score+1;
                res=res+1;
                count++;
                if(count==5)
                   {
                    glClearColor(0.287933,0.735398,0.669251,1.0);
                    move_speed=90;
                }
                if(count==10)
                {
                    glClearColor(0.239216,0.109804,0,1.0);
                    move_speed=70;
                    
                }
                if(count==15)
                {
                    glClearColor(1,0.411765,0.705883,1.0);
                        move_speed=50;
                
                    
                    
                }
                if(count==20)
                {
                    glClearColor(0.980392,0.815686,0.537255,1.0);    
                            move_speed=35;
                
                }
                
                

                break;
            }
        }
    }

    glLoadIdentity();
    glTranslatef(food_coords[0], food_coords[1], -40.0f);
    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_POLYGON);
        glVertex2d( 1.0f,  1.0f);
        glVertex2d( 1.0f, -1.0f);
        glVertex2d(-1.0f, -1.0f);
        glVertex2d(-1.0f,  1.0f);
    glEnd();
    
}
void displayRasterText(float x ,float y,char *stringToDisplay) {
    int length;
    glColor3f(1.0,0.0,0.0);
    glRasterPos2f(x, y);
        length = strlen(stringToDisplay);

    for(int i=0 ;i<length ;i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,stringToDisplay[i]); 
    }
    glutPostRedisplay();
}
void displayRasternum(float x ,float y,int num) {
    glColor3f(1.0,0.0,0.0);
    int p=num;
    int k=0;   
    while(p > 9)
        {
            k = p % 10;
            glRasterPos2f(x,y);    
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,48+k);
            p /= 10;
        }
            glRasterPos2f(x+1,y);   
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,48+p);
    glutPostRedisplay();
}
void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode (GL_PROJECTION); // Tell opengl that we are doing project matrix work
    glLoadIdentity(); // Clear the matrix
    gluOrtho2D(100,0,100,0); // Setup an Ortho view
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // Clear the matrix
    displayRasterText(20,20,"SCORE:");
    displayRasternum(6.5,20,score);
    glMatrixMode (GL_PROJECTION); // Tell opengl that we are doing project matrix work
    glLoadIdentity(); // Clear the matrix
    gluPerspective(75.0f, 1, 0.0f, 35.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // The vertex order is clockwise
    // The side order is front, back, left, right, top, bottom (if applicable)

    // Loop over snake size and draw each part at it's respective coordinates
    for(unsigned int a = 0; a < part_coords.size(); a++){
        glLoadIdentity();
        glTranslatef(part_coords[a][0], part_coords[a][1], -40.0f);
        glColor3f(0.0f, 1.0f, 0.0f);

        glBegin(GL_POLYGON);
            glVertex2d( 1.0f,  1.0f);
            glVertex2d( 1.0f, -1.0f);
            glVertex2d(-1.0f, -1.0f);
            glVertex2d(-1.0f,  1.0f);
        glEnd();
    }

    spawnFood();
   

    glutSwapBuffers();
}



void moveSnake(int new_direction){
    direction = new_direction;

    int last_part = part_coords.size() - 1;
    std::deque<float> new_head = part_coords[last_part];

    if(direction == UP){
        // Did we slither into ourself?
        for(unsigned int a = 0; a < part_coords.size(); a++){
            if(part_coords[0][0]        == part_coords[a][0] &&
               part_coords[0][1] + 2.0f == part_coords[a][1]){
            printf("Final Score is:- %d",res);
                exit(0);
            }
        }

        // Did we slither into a wall?
        if(part_coords[0][1] == map_half_length){
        printf("Final Score is:- %d",res);
            exit(0);
		   
        }

        // Did we get food?
        if(part_coords[0][0]        == food_coords[0] &&
           part_coords[0][1] + 2.0f == food_coords[1]){
            growth_stage++;
            food_available = false;
        }

        new_head[1] = part_coords[0][1] + 2.0f;
    } else if(direction == DOWN){
        // Did we slither into ourself?
        for(unsigned int a = 0; a < part_coords.size(); a++){
            if(part_coords[0][0]        == part_coords[a][0] &&
               part_coords[0][1] - 2.0f == part_coords[a][1]){
            printf("Final Score is:- %d",res);
                exit(0);
            }
        }

        // Did we slither into a wall?
        if(part_coords[0][1] == -map_half_length){
        	printf("Final Score is:- %d",res);
            exit(0);
        }

        // Did we get food?
        if(part_coords[0][0]        == food_coords[0] &&
           part_coords[0][1] - 2.0f == food_coords[1]){
            growth_stage++;
            food_available = false;
        }

        new_head[1] = part_coords[0][1] - 2.0f;
    } else {
        new_head[1] = part_coords[0][1];
    }

    if(direction == LEFT){
        // Did we slither into ourself?
        for(unsigned int a = 0; a < part_coords.size(); a++){
            if(part_coords[0][0] - 2.0f == part_coords[a][0] &&
               part_coords[0][1]        == part_coords[a][1]){
            printf("Final Score is:- %d",res);
                exit(0);
            }
        }

        // Did we slither into a wall?
        if(part_coords[0][0] == -map_half_length){
        	printf("Final Score is:- %d",res);
            exit(0);
        }

        // Did we get food?
        if(part_coords[0][0] - 2.0f == food_coords[0] &&
           part_coords[0][1]        == food_coords[1]){
            growth_stage++;
            food_available = false;
        }

        new_head[0] = part_coords[0][0] - 2.0f;
    } else if(direction == RIGHT){
        // Did we slither into ourself?
        for(unsigned int a = 0; a < part_coords.size(); a++){
            if(part_coords[0][0] + 2.0f == part_coords[a][0] &&
               part_coords[0][1]        == part_coords[a][1]){
            printf("Final Score is:- %d",res);
                exit(0);
            }
        }

        // Did we slither into a wall?
        if(part_coords[0][0] == map_half_length){
        	printf("Final Score is:- %d",res);
            exit(0);
        }

        // Did we get food?
        if(part_coords[0][0] + 2.0f == food_coords[0] &&
           part_coords[0][1]        == food_coords[1]){
            growth_stage++;
            food_available = false;
        }

        new_head[0] = part_coords[0][0] + 2.0f;
    } else {
        new_head[0] = part_coords[0][0];
    }

    part_coords.push_front(new_head);

    if(!growth_stage){
        part_coords.pop_back();
    } else if(growth_stage == growth){
        growth_stage = 0;
    } else {
        growth_stage++;
    }

    //glutPostRedisplay();
}

void keyboard(int key, int x, int y){
    UNUSED(x);
    UNUSED(y);

    switch(key){
        case GLUT_KEY_UP:{
            if(direction == LEFT || direction == RIGHT){
                moved = true;

                moveSnake(UP);
            }

            break;
        }

        case GLUT_KEY_DOWN:{
            if(direction == LEFT || direction == RIGHT){
                moved = true;

                moveSnake(DOWN);
            }

            break;
        }

        case GLUT_KEY_LEFT:{
            if(direction == UP || direction == DOWN){
                moved = true;

                moveSnake(LEFT);
            }

            break;
        }

        case GLUT_KEY_RIGHT:{
            if(direction == UP || direction == DOWN){
                moved = true;

                moveSnake(RIGHT);
            }

            break;
        }
    
    }
    

    glutPostRedisplay();
}

void initGL(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Specify the coordinates to each part of the snake
     int initSize = 3;
    for(int a = 1; a <= initSize; a++){
        std::deque<float> row;

        row.push_back(0.0f);
        row.push_back((map_half_length + 2.0f + (initSize * 2)) - (a * 2));

        part_coords.push_front(row);
    }
    srand(time(NULL));
    
}

void moveSnakeAuto(int value){
    if(!moved){
        UNUSED(value);

        if(direction == UP){
            moveSnake(UP);
        } else if(direction == DOWN){
            moveSnake(DOWN);
        } else if(direction == LEFT){
            moveSnake(LEFT);
        } else if(direction == RIGHT){
            moveSnake(RIGHT);
        }
    } else {
        moved = false;
    }

    glutTimerFunc(move_speed, moveSnakeAuto, 0);
}

void reshape(GLsizei width, GLsizei height){
    UNUSED(width);
    UNUSED(height);

    // Make the window non-resizable so we don't have to worry about size changes
    glutReshapeWindow(600, 600);
}
void game(int id)
{
 switch(id)
 {  
  case 1:
    ch=0;
    glClearColor(0.287933,0.735398,0.669251,1.0);
      break;
  case 2:
    ch=1;
    glClearColor(0,0.501961,0.501961,1.0);
      break;
  case 3:
    ch=2;
    glClearColor(1,0.411765,0.705883,1.0);
      break;
  case 4:
    ch=3;
    glClearColor(0.980392,0.815686,0.537255,1.0);
      break;
  case 5:    
    ch=4;
    glClearColor(0.239216,0.109804,0,1.0);
      break;
  case 6:    
      exit(0);
 }
 glutPostRedisplay();
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    frontscreen();
    glFlush();
}
void display_ins()
{
    glClear(GL_COLOR_BUFFER_BIT);
    instruction();
    glFlush();
}
void instruct(){
    
    glutDisplayFunc(display_ins);
    glutPostRedisplay();
     
}
void maingame(){
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard);
    initGL();
    glutTimerFunc(move_speed, moveSnakeAuto, 0);
    glutCreateMenu(game);
    glutAddMenuEntry("[Themes]",0);
    glutAddMenuEntry("Fun and Learning",1);
       glutAddMenuEntry("Google",2);
    glutAddMenuEntry("Kid Mode",3);
    glutAddMenuEntry("Cohesive Colors-I",4);
    glutAddMenuEntry("Cohesive Colors-II",5);
    glutAddMenuEntry("Exit",6);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    glutPostRedisplay();
     
}
void keyy(unsigned char key,int x,int y)
{
    if(key==13||key==49)
        maingame();
    else if(key==50)
        instruct();
    else if(key==51)
        exit(0);
        
}
void myinit()
{
    glClearColor(0.0,0.0,0.0,0.0);
    gluOrtho2D(0.0,600.0,0.0,600.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}
int main(int argc,char **argv)
{
    glutInit(&argc,argv);
    glutInitWindowSize(600,600);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutCreateWindow("SNAKE");
    glutDisplayFunc(Display);
    glutKeyboardFunc(keyy);
    myinit();
    
    glutMainLoop();
    return 0;
    
}

