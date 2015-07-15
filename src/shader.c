// Author: Eric M. Knapik
//
// the shader object implementation file
// this refers to how the shader program to
// be executed depending on the object
// holds all the info for vertex and fragment shaders

/*
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
*/

#include "shader.h"


// makes the a shader program with the default values
Shader* mkShader( const char *vert, const char *frag )
{
    Shader *shader = malloc( sizeof( Shader ) );
    if( shader == NULL )
    {
        perror( "could not malloc for shader object\n" );
        exit( 1 );
    }

    shader->shaderProgram = shaderSetup( vert, frag );
    // transformation matrix values
    // rotation values
    shader->rotateVals[0] = 0.0f;
    shader->rotateVals[1] = 0.0f;
    shader->rotateVals[2] = 0.0f;
    // translation values
    shader->transVals[0] = 0.0f;
    shader->transVals[1] = 0.0f;
    shader->transVals[2] = 0.0f;
    // scale values
    shader->scaleVals[0] = 1.0f;
    shader->scaleVals[1] = 1.0f;
    shader->scaleVals[2] = 1.0f;

    // camera values
    // eye values
    shader->eyePos[0] = -1.0f;
    shader->eyePos[1] = 22.00f;
    shader->eyePos[2] = 0.0f;
    // lookAt values
    shader->lookAt[0] = 0.0f;
    shader->lookAt[1] = 0.0f;
    shader->lookAt[2] = -3.0f;
    // upVector values
    shader->upVec[0] = 0.0f;
    shader->upVec[1] = 0.0f;
    shader->upVec[2] = -1.0f;


    // clipping window boundaries
    shader->left = -1.0f;
    shader->right = 1.0f;
    shader->top = 1.0f;
    shader->bot = -1.0f;
    shader->near = 1.0f;
    shader->far = -1.0f;

    // set base color to be violet because its odd
    shader->red = 0.933f;
    shader->green = 0.509f;
    shader->blue = 0.933f;

    return shader;
}

// destroys the shader
void destroyShader( Shader *shader)
{
    if( shader != NULL )
    {
        free( shader );
    }

    return;
}


// the method that sets up the shader program to be executed in the
// main display function
void shaderDisplay( Shader *shader )
{
    // pass the values for the frustum projection using the clipping values
    GLuint leftLoc = glGetUniformLocation( shader->shaderProgram, "left" );
    GLuint rightLoc = glGetUniformLocation( shader->shaderProgram, "right" );
    GLuint topLoc = glGetUniformLocation( shader->shaderProgram, "top" );
    GLuint bottomLoc = glGetUniformLocation( shader->shaderProgram, "bottom" );
    GLuint nearLoc = glGetUniformLocation( shader->shaderProgram, "near" );
    GLuint farLoc = glGetUniformLocation( shader->shaderProgram, "far" );

    glUniform1f( leftLoc, shader->left );
    glUniform1f( rightLoc, shader->right );
    glUniform1f( topLoc, shader->top );
    glUniform1f( bottomLoc, shader->bot );
    glUniform1f( nearLoc, shader->near );
    glUniform1f( farLoc, shader->far );

    // pass the values for the transformation matrix
    GLuint thetaLoc = glGetUniformLocation( shader->shaderProgram, "theta" );
    GLuint transLoc = glGetUniformLocation( shader->shaderProgram, "trans" );
    GLuint scaleLoc = glGetUniformLocation( shader->shaderProgram, "scale" );

    glUniform3fv( thetaLoc, 1, shader->rotateVals );
    glUniform3fv( transLoc, 1, shader->transVals );
    glUniform3fv( scaleLoc, 1, shader->scaleVals );

    // pass the values for the camera
    GLuint posLoc = glGetUniformLocation( shader->shaderProgram, "cPosition" ); 
    GLuint lookLoc = glGetUniformLocation( shader->shaderProgram, "cLookAt" );
    GLuint upVecLoc = glGetUniformLocation( shader->shaderProgram, "cUp" );

    glUniform3fv( posLoc, 1, shader->eyePos );
    glUniform3fv( lookLoc, 1, shader->lookAt );
    glUniform3fv( upVecLoc, 1, shader->upVec );

    // pass the values for the lighting calculations
    // the vertex and normals are being passed through the object
    // display function
    setUpLighting( shader );

    return;
}

// set camera to something other than default
void setCamera( Shader *shader, GLfloat eyePos[], GLfloat lookAt[], GLfloat upVec[] )
{
    shader->eyePos[0] = eyePos[0];
    shader->eyePos[1] = eyePos[1];
    shader->eyePos[2] = eyePos[2];

    shader->lookAt[0] = lookAt[0];
    shader->lookAt[1] = lookAt[1];
    shader->lookAt[2] = lookAt[2];

    shader->upVec[0] = upVec[0];
    shader->upVec[1] = upVec[1];
    shader->upVec[2] = upVec[2];
    return;
}

// set rotatation values
// x, y, z
void setRotate( Shader *shader, GLfloat rotateVals[] )
{
    shader->rotateVals[0] = rotateVals[0];
    shader->rotateVals[1] = rotateVals[1];
    shader->rotateVals[2] = rotateVals[2];
    return;
}


// set translation values
void setTrans( Shader *shader, GLfloat transVals[] )
{
    shader->transVals[0] = transVals[0];
    shader->transVals[1] = transVals[1];
    shader->transVals[2] = transVals[2];
    return;
}

// set scale value
void setScale( Shader *shader, GLfloat scaleVals[] )
{
    shader->scaleVals[0] = scaleVals[0];
    shader->scaleVals[1] = scaleVals[1];
    shader->scaleVals[2] = scaleVals[2];
    return;
}

// sets the color values for the object to be rendered with
// values for each are between 0.0 and 1.0 where 0 is off and 1 is full on
void setColor( Shader *shader, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha )
{
    shader->red = red;
    shader->green = green;
    shader->blue = blue;
    shader->alpha = alpha;
    return;
}
