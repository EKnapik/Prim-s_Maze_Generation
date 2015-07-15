// object.h
// Author: Eric M. Knapik
// header file for the object struct

#ifndef ObjectClass
#define ObjectClass

#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#include <stdlib.h>
#include <stdio.h>

#include "shader.h"

// Define the struct of my object
typedef struct Object_Struct
{
    GLfloat *vertData;
    GLshort *connectData;
    GLuint  numVert;
    GLuint numConnect;

    GLuint  vBuffer;
    GLuint  eBuffer;
    Shader  *shader;

} Object;


// the object creation method
// public
// @params:
// given the verticies, number of verticies, connections, number of connections
// the path to vertex shader, path to fragment shader
Object* mkObject( GLfloat *verticies, GLuint numVert, GLshort *connections, 
        GLuint numConnect, const char *vert, const char *frag );

// the object deletion method
// public
// given the object to delete
void destroyObject( Object *object );

// when ready setup the vBuffer and the eBuffer
// private
// given the object to setup the opengl buffers for
void setupGLBuffers( Object *object );

// draw the current object using its shader program
// public
// given the object to draw
void drawObject( Object *object );

#endif
