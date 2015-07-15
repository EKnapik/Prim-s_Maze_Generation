// Author: Eric M. Knapik
//
// the shader object header file
// this refers to how the shader program to
// be executed depending on the object
// holds all the info for vertex and fragment shaders
#ifndef ShaderClass
#define ShaderClass

#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include "shaderSetup.h"

typedef struct Shader_Struct
{
    GLuint shaderProgram;
    GLfloat rotateVals[3];
    GLfloat transVals[3];
    GLfloat scaleVals[3];
    GLfloat eyePos[3];
    GLfloat lookAt[3];
    GLfloat upVec[3];
    GLfloat left;
    GLfloat right;
    GLfloat top;
    GLfloat bot;
    GLfloat near;
    GLfloat far;
    GLfloat red;
    GLfloat green;
    GLfloat blue;
    GLfloat alpha;

} Shader;


// makes the a shader program with the default values
// invoked by the Object struct's create object call
// private
Shader* mkShader( const char *vert, const char *frag );

// deallocates the memory for the shader object
// invoked by the Object struct's destroy object call
// private
void destroyShader( Shader *shader);

// sets up the shader variables so that they are passed to the shader
// program for display, must be called before a call to drawTraingles can happen
// invoked by the Object struct's draw shape method so that the object can be 
// rendered propperly
// private
void shaderDisplay( Shader *shader );

// set camera to something other than default
// public
void setCamera( Shader *shader, GLfloat eyePos[], GLfloat lookAt[], GLfloat upVec[] );

// set rotatation values
// x, y, z
// public
void setRotate( Shader *shader, GLfloat rotateVals[] );

// set translation values
// public
void setTrans( Shader *shader, GLfloat transVals[] );

// set the scale values
// public
void setScale( Shader *shader, GLfloat scaleVals[] );

// sets the color values for the object to be rendered with
// values for each are between 0.0 and 1.0 where 0 is off and 1 is full on
// public
void setColor( Shader *shader, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha );

#endif
