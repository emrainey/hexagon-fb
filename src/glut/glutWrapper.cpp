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

#define EXCLUDE_EXTERNS
#include "Port.h"

extern Port *platform;
extern Debug *debug;

#define TEST_PLATFORM() {   \
    if (platform==NULL)     \
    {                       \
        debug->info(DEBUG_INTERNAL,"Platform is NULL!\n");\
        return;             \
    }                       \
}

// The thin GLUT wrappers. These are needed because we can't cast these Class
// Functions into the necessary GLUT function types.
void glut_keyPressed(unsigned char key, int x, int y)
{
    TEST_PLATFORM();
    platform->input.keyboardNormal(key,x,y);
}

void glut_keyPressedSpecial(int key, int x, int y)
{
    TEST_PLATFORM();
    platform->input.keyboardSpecial(key,x,y);
}

void glut_mouseClick(int button, int state, int x, int y)
{
    TEST_PLATFORM();
    platform->input.mouseClick(button,state,x,y);
}

void glut_mouseDrag(int x, int y)
{
    TEST_PLATFORM();
    platform->input.mouseDrag(x,y);
}

void glut_mouseMove(int x, int y)
{
    TEST_PLATFORM();
    platform->input.mouseMove(x,y);
}

void glut_reshapeScreen(int width, int height)
{
    TEST_PLATFORM();
    platform->display.resize(width,height);
}

void glut_drawScreen(void)
{
    TEST_PLATFORM();
    platform->display.draw();
}

void glut_timerFired(int value)
{
    if (platform != NULL)
        platform->scheduler.timed(value);
	
	/* reinstall the timer */
    glutTimerFunc(5,glut_timerFired,0);    
}

void glut_idleLoop(void)
{   
    TEST_PLATFORM();
    platform->scheduler.idle();
}

// This function just does all the assignment for the wrappers
void glutWrappers_assignCallbacks(void)
{
    // Input Object Callbacks
    glutMouseFunc(glut_mouseClick);
	glutMotionFunc(glut_mouseDrag);
    glutPassiveMotionFunc(glut_mouseMove);
	glutKeyboardFunc(glut_keyPressed);
    glutSpecialFunc(glut_keyPressedSpecial);
    
    // Display Object Callbacks
    glutReshapeFunc(glut_reshapeScreen);
    glutDisplayFunc(glut_drawScreen);
    
    // Timer and Idle Callbacks
    glutTimerFunc(5,glut_timerFired,0);
    glutIdleFunc(glut_idleLoop);
}

void glLocalPushMatrix(char *filename, int line)
{
    glPushMatrix();
    CheckError(glGetError(),true,"Local Push",filename,line);
}

void glLocalPopMatrix(char *filename, int line)
{
    glPopMatrix();
    CheckError(glGetError(),true,"Local Pop",filename,line);
}   

GLuint CheckError(GLuint error, bool stop, char *name, char *filename, int line)
{
    switch(error)
    {
        case GL_NO_ERROR:
            // don't bother checking anything else.
            return error;
        case GL_INVALID_ENUM:
            debug->info(DEBUG_ANY,"ERROR: Invalid enumeration %s=0x%08x in %s on line %i\n",name,error,filename,line);
            break;
        case GL_INVALID_VALUE:
            debug->info(DEBUG_ANY,"ERROR: Invalid value %s=0x%08x in %s on line %i\n",name,error,filename,line);
            break;
        case GL_INVALID_OPERATION:
            debug->info(DEBUG_ANY,"ERROR: Invalid operation %s=0x%08x in %s on line %i\n",name,error,filename,line);
            break;
        case GL_STACK_OVERFLOW:
            debug->info(DEBUG_ANY,"ERROR: Stack overflow @%s in %s on line %i!\n",name,filename,line);
            break;
        case GL_STACK_UNDERFLOW:
            debug->info(DEBUG_ANY,"ERROR: Stack underflow @%s in %s on line %i!\n",name,filename,line);
            break;
        case GL_OUT_OF_MEMORY:
            debug->info(DEBUG_ANY,"ERROR: Out Of Memory @%s in %s on line %i!\n",name,filename,line);
            break;
        default:
            debug->info(DEBUG_ANY,"ERROR: Unknown Error %d (0x%08x) @%s in %s on line %i!\n",error,error,name,filename,line);
            break;
    }
    if (stop == true)
    {
        /* add any deconstructors here */
        exit(1);
    }
    
    return error;
}

/*****************************************************************************/
/* END                                                                       */
/*****************************************************************************/
