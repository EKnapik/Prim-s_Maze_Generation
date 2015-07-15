//
//  shaderSetup
//
//  Set up a GLSL shader based on supplied source files.
//

#ifndef _SHADERSETUP_H_
#define _SHADERSETUP_H_

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

/*
** Error codes returned by shaderSetup()
*/

#define	E_NO_ERROR	0
#define	E_VS_LOAD	1
#define	E_FS_LOAD	2
#define	E_VS_COMPILE	3
#define	E_FS_COMPILE	4
#define	E_SHADER_LINK	5

/*
** shaderErrorCode
**
** Contains a code (see shaderSetup.h) indicating the status
** of the most recent shaderSetup() call
*/

extern GLuint shaderErrorCode;

/*
** read_text_file(name)
**
** Read the text file given as 'name'.
**
** Returns the contents of the file in a dynamically-allocated
** string buffer, or NULL if an error occurs.
*/

GLchar *read_text_file( const char *name );

/*
** print_shader_info_log(shader)
**
** Print the information log from a shader compilation attempt
*/

void print_shader_info_log( GLuint shader );

/*
** print_program_info_log(shader)
**
** Print a program information log
**
** This is identical to print_shader_info_log(), except that it uses
** glGetProgramiv() and glGetProgramInfoLog() instead of the *Shader*()
** versions.
*/

void print_program_info_log( GLuint shader );

/*
** errorString(code)
**
** Returns a const char* with a text version of the supplied error code.
*/

const char *errorString( GLuint code );

/*
** shaderSetup(vertex,fragment)
**
** Set up a GLSL shader program.
**
** Requires the name of a vertex program and a fragment
** program.  Returns a handle to the created GLSL program
**
** Arguments:
**      vert   - vertex shader program source file
**      frag   - fragment shader program source file
**	errors - pointer to variable to be incremented in case of error
**
** On success:
**	returns the GLSL shader program handle, and sets the global
**	shaderErrorCode to E_NO_ERROR.
**
** On failure:
**	returns 0, and shaderErrorCode contains an error code
*/

GLuint shaderSetup( const char *vert, const char *frag );

#endif
