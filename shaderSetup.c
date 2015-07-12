//
//  shaderSetup
//
//  Sets up a GLSL shader based on supplied source files.
//
//  Based on code from www.lighthouse3d.com
//
//  This code can be compiled as either C or C++.
//

#ifdef __cplusplus
#include <cstdio>
#include <cstdlib>
#include <cstring>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#include "shaderSetup.h"

/*
** shaderErrorCode
**
** Contains a code (see shaderSetup.h) indicating the status
** of the most recent shaderSetup() call
*/

GLuint shaderErrorCode;

/*
** read_text_file(name)
**
** Read the text file given as 'name'.
**
** Returns the contents of the file in a dynamically-allocated
** string buffer, or NULL if an error occurs.
*/

GLchar *read_text_file( const char *name ) {
    FILE *fp;
    GLchar *content = NULL;
    int count=0;

    if( name != NULL ) {

        // Attempt to open the file
        // On Windows systems, may want to use "rt" here
        fp = fopen( name, "r" );
        if( fp != NULL ) {

            // Determine its length
            fseek( fp, 0, SEEK_END );
            count = ftell( fp );
            rewind( fp );

            if( count > 0 ) {

                // Allocate the string buffer
#ifdef __cplusplus
                content = new GLchar[ sizeof(GLchar) * (count+1) ];
#else
                content = (GLchar *) malloc( sizeof(GLchar) * (count+1) );
#endif
                // Read the file into the buffer
                count = fread( content, sizeof(GLchar), count, fp );
                // Add a NUL terminator
                content[count] = '\0';
            }

            fclose(fp);
        } else {
            perror( name );
        }
    } else {
        fprintf( stderr, "error:  no file name specified\n" );
    }

    return content;

}

/*
** print_shader_info_log(shader)
**
** Print the information log from a shader compilation attempt
*/

void print_shader_info_log( GLuint shader ) {
    GLint length = 0;
    GLsizei nchars  = 0;
    char *log;

    // Determine the length of the information log
    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &length );

    if( length > 0 ) {

        // Allocate a buffer for the log
#ifdef __cplusplus
        log = new char[ length ];
#else
        log = (char *) malloc( length );
#endif

        if( log != NULL ) {

            // Retrieve the log
            glGetShaderInfoLog( shader, length, &nchars, log );

            // Report it
            if( log[0] != '\0' ) {
                printf( "Shader log:  '%s'\n", log );
            }

#ifdef __cplusplus
            delete [] log;
#else
            free( log );
#endif

        } else {

            printf( "Shader log non-empty, but allocate failed\n" );

        }
    }

}

/*
** print_program_info_log(shader)
**
** Print a program information log
**
** This is identical to print_shader_info_log(), except that it uses
** glGetProgramiv() and glGetProgramInfoLog() instead of the *Shader*()
** versions.
*/

void print_program_info_log( GLuint shader ) {
    GLint length = 0;
    GLsizei nchars  = 0;
    char *log;

    // Determine the length of the information log
    glGetProgramiv( shader, GL_INFO_LOG_LENGTH, &length );

    if( length > 0 ) {

        // Allocate a buffer for the log
#ifdef __cplusplus
        log = new char[ length ];
#else
        log = (char *) malloc( length );
#endif

        if( log != NULL ) {

            // Retrieve the log
            glGetProgramInfoLog( shader, length, &nchars, log );

            // Report it
            if( log[0] != '\0' ) {
                printf( "Program log:  '%s'\n", log );
            }

#ifdef __cplusplus
            delete [] log;
#else
            free( log );
#endif

        } else {

            printf( "Program log non-empty, but allocate failed\n" );

        }
    }
}

/*
** errorString( code )
**
** Returns a const char* with a text version of the supplied error code.
*/

const char *errorString( GLuint code ) {
    const char *msg;
    static char buffer[256];

    switch( code ) {

        case E_NO_ERROR:
            msg = "No error";
            break;

        case E_VS_LOAD:
            msg = "Error loading vertex shader";
            break;

        case E_FS_LOAD:
            msg = "Error loading fragment shader";
            break;

        case E_VS_COMPILE:
            msg = "Error compiling vertex shader";
            break;

        case E_FS_COMPILE:
            msg = "Error compiling fragment shader";
            break;

        case E_SHADER_LINK:
            msg = "Error linking shader";
            break;

        default:
            sprintf( buffer, "Unknown error code %d", code );
            msg = (const char *) buffer;

    }

    return( msg );

}

/*
** shaderSetup(vertex,fragment)
**
** Set up a GLSL shader program.
**
** Requires the name of a vertex program and a fragment
** program.  Returns a handle to the created GLSL program
**
** Arguments:
**    vert   - vertex shader program source file
**    frag   - fragment shader program source file
**    errors - pointer to variable to be incremented in case of error
**
** On success:
**      returns the GLSL shader program handle, and sets the global
**      shaderErrorCode to E_NO_ERROR.
**
** On failure:
**    returns 0, and shaderErrorCode contains an error code
*/

GLuint shaderSetup( const char *vert, const char *frag ) {
    GLchar *vsrc = NULL, *fsrc = NULL;
    GLuint vs, fs, prog;
    GLint flag;

    // Assume that everything will work
    shaderErrorCode = E_NO_ERROR;

    // Create the shader handles
    vs = glCreateShader( GL_VERTEX_SHADER );
    fs = glCreateShader( GL_FRAGMENT_SHADER );

    // Read in shader source
    vsrc = read_text_file( vert );
    if( vsrc == NULL ) {
        fprintf( stderr, "Error reading vertex shader file %s\n",
             vert);
        shaderErrorCode = E_VS_LOAD;
        return( 0 );
    }

    fsrc = read_text_file( frag );
    if( fsrc == NULL ) {
        fprintf( stderr, "Error reading fragment shader file %s\n",
             frag);
        shaderErrorCode = E_FS_LOAD;
#ifdef __cplusplus
        delete [] vsrc;
#else
        free( vsrc );
#endif
        return( 0 );
    }

    // Attach the source to the shaders
    glShaderSource( vs, 1, (const GLchar **) &vsrc, NULL );
    glShaderSource( fs, 1, (const GLchar **) &fsrc, NULL );

    // We're done with the source code now
#ifdef __cplusplus
    delete [] vsrc;
    delete [] fsrc;
#else
    free(vsrc);
    free(fsrc);
#endif

    // Compile the shaders, and print any relevant message logs
    glCompileShader( vs );
    glGetShaderiv( vs, GL_COMPILE_STATUS, &flag );
    print_shader_info_log( vs );
    if( flag == GL_FALSE ) {
        shaderErrorCode = E_VS_COMPILE;
        return( 0 );
    }

    glCompileShader( fs );
    glGetShaderiv( fs, GL_COMPILE_STATUS, &flag );
    print_shader_info_log( fs );
    if( flag == GL_FALSE ) {
        shaderErrorCode = E_FS_COMPILE;
        return( 0 );
    }

    // Create the program and attach the shaders
    prog = glCreateProgram();
    glAttachShader( prog, vs );
    glAttachShader( prog, fs );

    // Report any message log information
    print_program_info_log( prog );

    // Link the program, and print any message log information
    glLinkProgram( prog );
    glGetProgramiv( prog, GL_LINK_STATUS, &flag );
    print_program_info_log( prog );
    if( flag == GL_FALSE ) {
        shaderErrorCode = E_SHADER_LINK;
        return( 0 );
    }

    return( prog );

}
