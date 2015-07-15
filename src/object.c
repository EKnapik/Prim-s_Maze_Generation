// object.c
// Author: Eric M. Knapik
// implemtation for ambiguous object


#include "object.h"

/*
// Define the struct of my object
typedef struct Object_Struct
{
    GLfloat *vertData;
    GLshort *connectData;
    GLuint  numVert;
    GLuint numConnect;

    GLuint  vBuffer;
    GLuint  eBuffer;
    Shader *shader;

} Object;
*/

// How to calculate a buffer offset into the vertex buffer
// this is so no warning occur because it expects a char *
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// the object creation method
Object* mkObject( GLfloat *verticies, GLuint numVert, GLshort *connections, 
        GLuint numConnect, const char *vert, const char *frag )
{
    Object *newObject = malloc( sizeof( Object ) );
    if( newObject == NULL )
    {
        perror( "UNABLE TO MALLOC NEW OBJECT\n" );
        exit( 1 );
    }

    newObject->vertData = verticies;
    newObject->numVert = numVert;
    newObject->connectData = connections;
    newObject->numConnect = numConnect;

    // create the shader object for this abstract object
    newObject->shader = mkShader( vert, frag );

    // setupt the openGL buffers for this object
    setupGLBuffers( Object *object )

    return newObject;
}

// the object deletion method
void destroyObject( Object *object )
{
    // This is basic cheching if the object exists then delete the
    // memory that was allocated for each place
    if( object != NULL )
    {
        // free each of my arrays that I might have allocated
        if( object->verticies != NULL )
        {
            free( object->verticies );
        }

        if( object->connectivity != NULL )
        {
            free( object->connectivity );
        }

        destroyShader( object->shader );

        free( object );
    }

    return;
}

// when ready setup the vBuffer and the eBuffer
void setupGLBuffers( Object *object )
{
    // 2 floats make up one vertex
    int dataSize = object->numVert * 2 * sizeof( GLfloat );

    // 3 verticies make one triangle
    int elementDataSize = object->numConnect * 3 * sizeof( GLshort );

    // ask OpenGL to create on buffer object
    glGenBuffers( 1, &object->vBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, object->vBuffer );
    glBufferData( GL_ARRAY_BUFFER, dataSize, 0, GL_STATIC_DRAW );

    // binds my verticies to this buffer
    glBufferSubData( GL_ARRAY_BUFFER, 0, dataSize, object->vertData );

    // now do the same for the element array
    glGenBuffers( 1, &object->eBuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, object->eBuffer );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, elementDataSize, object->connectData, GL_STATIC_DRAW );

    // unbind any buffer to the current array and element array
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void drawObject( Object *object )
{
    int dataSize = object->numVert * 2 * sizeof( GLfloat );

    // bind the shader program to use
    glUseProgram( object->shader->shaderProgram );

    // bind the array buffer
    glBindBuffer( GL_ARRAY_BUFFER, object->vBuffer );

    // bind the element array buffer
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, object->eBuffer );

    // enable my vertex
    GLuint vPosition = glGetAttribLocation( object->shader->shaderProgram, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    // pass the vertex to the shader the offset is 0 because it is first
    glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    // pass other things to my shader using the function call
    shaderDisplay( object->shader );
    
    // Everything ready for this object so draw it starting at 0 in the array
    glDrawElements( GL_TRIANGLES, object->numVert, GL_UNSIGNED_SHORT, (void *)0 );

    return;
}

// END OF FILE //
