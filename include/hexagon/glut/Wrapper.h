/*******************************************************************************
 * COPYRIGHT (C) 2003, ERIK RAINEY.
 *
 * This source code has not yet been approved for release under any license
 * and as such has no warranty or fitness of purpose associated with it. Use at
 * your own discretion.
 *
 * @author Erik Rainey
 * @version 1.0
 ******************************************************************************/

#ifndef GLUT_WRAPPER_H_
#define GLUT_WRAPPER_H_
/*****************************************************************************/
// Some Typedefs
/*****************************************************************************/

// we need to get rid of these...
typedef unsigned int GLenum;
typedef unsigned int GLuint;
typedef float GLfloat;

/*****************************************************************************/
// Some prototypes
/*****************************************************************************/
void glLocalPushMatrix(char const *const filename, int const line);
void glLocalPopMatrix(char const *const filename, int const line);
void glutWrappers_assignCallbacks(void);
GLuint CheckError(GLuint error, bool stop, char const *const name, char const *const filename, int const line);
// No description
void toggleDisplayText(void);
void toggleControlState(void);
void createNewView(void);

void report_error(char const *const name, bool stop, char const *const filename, int const line);

/*****************************************************************************/
// Some FUNCTION MACROS
/*****************************************************************************/

#define GL_PUSH_MATRIX() glLocalPushMatrix(__FILE__, __LINE__)
#define GL_POP_MATRIX() glLocalPopMatrix(__FILE__, __LINE__)

#define REPORT_ERROR(name, stop) report_error(#name, stop, __FILE__, __LINE__)

#define CHECKERROR(a, b) CheckError(a, b, #a, __FILE__, __LINE__)

#endif  // GLUT_WRAPPER_H_
