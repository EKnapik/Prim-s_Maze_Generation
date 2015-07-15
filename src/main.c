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
#include <stdio.h>

// define some paramaters as the default for the maze
#define MazeRows 20
#define MazeCols 30

#define WindowHeight 750
#define WindowWidth 750

// functions
void setupGLUT( int argc, char *argv[] );
void render( void );



// Main function
// This could have user input for maze size
int main( int argc, char *argv[] )
{
    setupGLUT( argc, argv );

    // lets start the generation
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
    glClearColor( 0.0, 0.0, 0.0, 1.0 ); // black full alpha

    glCullFace( GL_BACK );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    // this want the display function to use
    glutDisplayFunc( render );
}


void render( void )
{

    //Clear buffers
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


}
