// Copyright 2015 Eric Knapik
// MIT License

// This is the main controlling function for the Prim Maze Generation
// I am using OpenGL and a single buffer so that I do not redraw all
// the previously rendered cells and only consider the newest cell
// to the maze, this is better on memory, the amount of buffer writes
// and is actually easier to implememnt, where you do not have to 
// consider and worry about walls because this makes an image. Not an
// actuall program for going through mazes


#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

// define some paramaters as the default for the maze
#define MazeRows 20
#define MazeCols 30

#define WindowHeight 750
#define WindowWidth 750

#define TOP 0
#define RIGHT 1
#define BOTTOM 2
#define LEFT 3

/*
*   top is edge 0
*   right is edge 1
*   bottom is edge 2
*   left is edge 3
*/
typedef struct WallStruct {
    unsigned int row : 15; // 15 bits allocated
    unsigned int col : 15; // 15 bits allocated
    unsigned int edge : 2; // 2 bits allocated (I only need 4 values)
} Wall; // total is 4 bytes


// Globals
_Bool maze[MazeRows][MazeCols];
int totalCells = MazeRows * MazeCols;
int cellsInMaze = 0;
Wall *wallArray;
int wallArraySize = 0;
int curRow = 0;
int curCol = 0;
_Bool firstPass = true;

// functions
void setupGLUT( int argc, char *argv[] );
void render( void );
void addWalls();
void setNextPosition();

// Main function
// This could have user input for maze size
int main( int argc, char *argv[] )
{
    // Initialize my window for rendering using GLUT
    setupGLUT( argc, argv );

    // Initilize the maze
    wallArraySize = 0;
    for( int row = 0; row < MazeRows; row++ )
    {
        for( int col = 0; col < MazeCols; col++ )
        {
            maze[row][col] = false; // initialzie all spaces to false
        }
    }

    // initialize random number
    srand(time(NULL));
    // Enter the display callback loop
    glutMainLoop();
    return 0; // compiler wants this though this is never hit
}


// use GLUT to setup my window that I am going to draw to
void setupGLUT( int argc, char *argv[] )
{
    glutInit( &argc, argv );

    // This program requires single buffer because I never
    // want to actually clear between draws
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_SINGLE );
    
    glutInitWindowSize( WindowHeight, WindowWidth );
    glutCreateWindow( "Prim Maze Generation" );

    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );

    // set the background color
    glClearColor( 0.0, 0.0, 1.0, 1.0 ); // blue full alpha

    glCullFace( GL_BACK );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    // this want the display function to use
    glutDisplayFunc( render );
}


void render( void )
{
    // Do not clear buffers because I draw one cell
    // and leave that cell

    // Initialize first cell to be added
    if (firstPass == true ) {
        firstPass = false;
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        curRow = 0;
        curCol = 0;
        maze[curRow][curCol] = true; // this cell is now in the maze
        addWalls();
        // render current position as red

    } else { // Enter the the minimum spanning tree loop of adding cells to the maze
        // Render current position as white
        // Determine next position by random choice from wall Array
        setNextPosition();
        // set maze at current position to true meaning it is in the maze
        // printf( "Row: %d\n", curRow );
        // printf( "Col: %d\n", curCol );
        maze[curRow][curCol] = true; // this cell is now in the maze
        // add current position's walls to the maze
        addWalls();
        // render current position as red
    }
    glutSwapBuffers();

    if (wallArraySize > 0 || cellsInMaze < totalCells) {
        glutPostRedisplay();
    }
}


// Using the current Global curRow and the curCol
// will add that cell's walls to the global wall array
void addWalls()
{
    if( wallArraySize < 1 )
    {
        wallArray = malloc( sizeof(Wall) );
    }

    Wall wall;
    // try and add top
    if( (curRow-1) > 0 )
    {
        wall.row = curRow;
        wall.col = curCol; 
        wall.edge = BOTTOM;
        wallArraySize++;
        wallArray = realloc(wallArray, (sizeof(Wall) * wallArraySize));
        wallArray[wallArraySize-1] = wall;
    }
    // try and add bottom
    if( (curRow+1) < MazeRows )
    {
        wall.row = curRow;
        wall.col = curCol; 
        wall.edge = TOP;
        wallArraySize++;
        wallArray = realloc(wallArray, (sizeof(Wall) * wallArraySize));
        wallArray[wallArraySize-1] = wall;
    }
    // try and add left
    if( (curCol-1) > 0 )
    {
        wall.row = curRow;
        wall.col = curCol; 
        wall.edge = LEFT;
        wallArraySize++;
        wallArray = realloc(wallArray, (sizeof(Wall) * wallArraySize));
        wallArray[wallArraySize-1] = wall;
    }
    // try and add right
    if( (curCol+1) < MazeCols )
    {
        wall.row = curRow;
        wall.col = curCol; 
        wall.edge = RIGHT;
        wallArraySize++;
        wallArray = realloc(wallArray, (sizeof(Wall) * wallArraySize));
        wallArray[wallArraySize-1] = wall;
    }
}



// Using the current global curRow and curCol
// will set the next cell that is going to
// be added to the maze, for the next loop
void setNextPosition()
{
    int randomNum; 
    Wall wall;

    while( wallArraySize > 0 )
    {
        randomNum = rand() % wallArraySize;    
        // need to assign each element to get the wall at random place
        wall = wallArray[randomNum];
        wall.row = wallArray[randomNum].row;
        wall.col = wallArray[randomNum].col;
        wall.edge = wallArray[randomNum].edge;
        wallArraySize--; // I just removed a wall
        // replace position with last wall need to assign all 3
        wallArray[randomNum] = wallArray[wallArraySize];
        wallArray[randomNum].row = wallArray[wallArraySize].row;
        wallArray[randomNum].col = wallArray[wallArraySize].col;
        wallArray[randomNum].edge = wallArray[wallArraySize].edge;
        wallArray = realloc(wallArray, (sizeof(Wall) * wallArraySize)); // resize the array

        if( TOP == wall.edge )
        {
            if( !maze[wall.row+1][wall.col] ) // all non added cells are false
            {
                curRow = wall.row+1;
                curCol = wall.col;
                return;
            }
        }
        else if( BOTTOM == wall.edge )
        {
            if( !maze[wall.row-1][wall.col] ) 
            {
                curRow = wall.row-1;
                curCol = wall.col;
                return;
            }
        }
        else if( RIGHT == wall.edge )
        {
            if( !maze[wall.row][wall.col+1] ) 
            {
                curRow = wall.row;
                curCol = wall.col+1;
                return;
            }
        }
        else
        {
            if( !maze[wall.row][wall.col-1] ) 
            {
                curRow = wall.row;
                curCol = wall.col-1;
                return;
            }
        }
    }
}
