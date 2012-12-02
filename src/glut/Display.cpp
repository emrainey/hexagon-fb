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

#include <Hexagon.h>

static Color mat_ambient(0,0,0,1);
static Color mat_specular(1.0,1.0,1.0,0.15);
static GLfloat mat_shiny = 100.0;

#define GL_SET_AND_TEST(flag) {            \
    glEnable(flag);                        \
    if (glIsEnabled(flag) == GL_TRUE)      \
        debug->info(DEBUG_INTERNAL,#flag" Enabled\n");    \
	else if(glIsEnabled(flag)==GL_FALSE)   \
		debug->info(DEBUG_INTERNAL,#flag" Disabled\n");   \
}

// class constructor
Display::Display()
{
	debug->info(DEBUG_TRACE,"+Display()\n");
    
    renderGrid = false;
    renderText = false;
    renderHelp = false;
    renderFullName = true;    

    font = GLUT_BITMAP_9_BY_15;
    font_height = 15;

    memset(control_message,0,sizeof(control_message));

    bg.print();

    fg.setRedFloat(1.0);
    fg.setGreenFloat(1.0);
    fg.setBlueFloat(1.0);
    fg.print();

    debug->info(DEBUG_TRACE,"-Display()\n");
}

// class destructor
Display::~Display()
{
	// insert your code here
}

// Enable or Disable special parts of the OpenGL pipeline.
void Display::initOpenGL()
{
    debug->info(DEBUG_TRACE,"initOpenGL()\n");

    // Enable Blending Method...
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // set and test...
    GL_SET_AND_TEST(GL_BLEND);

    // Enable Hidden Surface Culling...
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    // Enable Depth Testing...
    GL_SET_AND_TEST(GL_DEPTH_TEST);

    // Turn on flat shading...
    glShadeModel(GL_SMOOTH);

    // enable AA of the lines...
    glEnable(GL_LINE_SMOOTH);

    // Set the clear color to all black...
	glClearColor(bg[0],bg[1],bg[2],bg[3]);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient.getFloatArray());
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular.getFloatArray());
    glMaterialfv(GL_FRONT, GL_SHININESS, &mat_shiny);
    //glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
}


// GLUT Callback to Resize the Display
void Display::resize(int w, int h)
{
    debug->info(DEBUG_TRACE,"Resized to %ix%i\n",w,h);
    width = w;
    height = h;
	glViewport(0,0,w,h);
	platform->camera.aspect = (GLfloat)w/h;
	platform->camera.place();
}

// do selection stuff...
int Display::drawSelection(int x, int y)
{
    #define BUFFER_LENGTH (1024)
    GLint hits=0,name=0;

    /* initialize a selection buffer */
    GLuint sel[BUFFER_LENGTH];

    glSelectBuffer(BUFFER_LENGTH,sel);

    /* setup a selection render */
    platform->camera.selection(x,y);

    /* render!!!! */
    glMatrixMode(GL_MODELVIEW);
    GL_PUSH_MATRIX();
    
    glInitNames();
    glPushName(0);

    // render the scene but LOAD YOUR NAME FIRST!
    world->render(true);

    GL_POP_MATRIX();
    glFlush();

    /* munch buffer */
    hits = glRenderMode(GL_RENDER);
    
    debug->info(DEBUG_SELECTION,"Hits: %d\n",hits);
    debug->flush();
    
    if ( hits > 0 )
    {
        int i=0,j;
        for ( j=0; j < hits; j++ )
        {
            debug->info(DEBUG_SELECTION,"%i: %u Names on stack at time of hit\n",i,sel[i]);
            i++;
            debug->info(DEBUG_SELECTION,"%i: %u min z\n",i,sel[i]);
            i++;
            debug->info(DEBUG_SELECTION,"%i: %u max z\n",i,sel[i]);
            i++;
            debug->info(DEBUG_SELECTION,"%i: 0x%08x on bottom of name stack\n",i,sel[i]);
            name = sel[i];
            i++;
            debug->flush();
        }
    }

    /* 
     * The old state will be restored when the draw runs again and 
     * places the camera.
     */
    
    if (debug->state)
    {
        debug->info(DEBUG_SELECTION,"-----------------------------------\n");
        debug->flush();
    }
    debug->info(DEBUG_TRACE,"drawSelection()::name=0x%08x\n",name);
    return name;
}

void Display::draw(void)
{
    // Set the clear color to all black...
	glClearColor(bg[0],bg[1],bg[2],bg[3]);
	
    // clear the screen and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    platform->camera.place();
    
    glEnable(GL_COLOR_MATERIAL);
    if (renderGrid == true)
    {
	   renderXYGrid(10,10);
    }
    glEnable(GL_LIGHTING);
    drawframe();
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    
    // now we render screen text.
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,(float)width,(float)height,0.0,0.0,1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
	glColor3f(fg.getRedFloat(),fg.getGreenFloat(),fg.getBlueFloat());
    renderScreenText();
    renderOverLay();

    glutSwapBuffers();
    frameStatistics();    
}

int Display::print2DText(int x, int y, char *format, ...)
{
    int i,len,line,nexty;
    va_list args;
    static char buffer[255];

    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

	glRasterPos2i(x,y);
    len = strlen(buffer);
    line = 0;
    
    nexty = y;

	for(i=0;i<len;i++)
	{
	    if ( buffer[i] == '\n' )
	    {
	        line++;
            nexty = y + font_height * line;
	        glRasterPos2i(x,nexty);
	    }
	    else
	    {
	    	glutBitmapCharacter(font,buffer[i]);
	    }
    }
    return nexty;
}

void Display::print3DText(Vector p, char *format, ...)
{
    int i,len,line;
    va_list args;
    static char buffer[255];

    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    line = 0;

	glRasterPos3d(p.x,p.y,p.z);
	len = strlen(buffer);

	for(i=0;i<len;i++)
	{
		if ( buffer[i] == '\n' )
		{
		    /* add some position to the text */
		    line++;
		    glRasterPos3d(p.x,p.y,p.z - (0.5 * line));
		}
		else
		{
		    glutBitmapCharacter(font,buffer[i]);
		}
    }
}

/*****************************************************************************/
// PRIVATE
/*****************************************************************************/

void Display::drawframe(void)
{
    // render the lights...
    world->shine(true);
	
	REPORT_ERROR(drawframe,true);
	
    // render all the nodes here...
	GL_PUSH_MATRIX();
    world->render(false); // not select mode...
    GL_POP_MATRIX();
}

void Display::frameStatistics()
{
	/************************************************/
    /* Compute Frame Count and FPS                  */
    /* Must run for every frame rendered            */
	/************************************************/

	frames++;
	platform->scheduler.finish = clock();
	if(frames >= MIN_FRAMES)
	{
	    clock_t diff = platform->scheduler.finish - platform->scheduler.start;
	   	fps = (float)frames / ((float)(diff)/CLOCKS_PER_SEC);
		platform->scheduler.start = clock();
		frames = 0;
	}
}

void Display::renderScreenText(void)
{
    int start_point=15,end=0;
    if (renderText == true)
    {
    	print2DText(5,start_point += 15,
    	            "f.p.s.: %.3lf\nRun Time: %.3lf secs",
    	            fps,(float)platform->scheduler.timer/CLOCKS_PER_SEC);
        end = height - (font_height*3);
        //end = print2DText(5,end,platform->camera.printInfo());
        end = print2DText(5,end,"%s\n",control_message);
        end = print2DText(5,end,"%s\n",world->focus->top->path.c_str());
        end = print2DText(5,end,"kb = kibibytes (1024 bytes)\n");
    }
    else
    {
        end = height - (font_height*2);
        end = print2DText(5,end,"%s\n",world->focus->top->path.c_str());
    }
    if (renderHelp == true)
    {
        start_point = height/8;
        print2DText(width/3,start_point,
            "Help Menu\n"
            "-----------------------------------\n"
            "In Keyboard Control Mode:\n"
            "(ALT+w) rotate up (ALT+s) rotate down\n"
            "(ALT+a) rotate left (ALT+d) rotate right\n"
            "\n"
            "In Mouse Zoom Mode the up and down motion\n"
            "of a mouse drag will zoom in or out\n"
            "\n"
            "In Mouse Drag Mode you can drag the screen\n"
            "around my dragging the mouse\n"
            "\n"
            "(SPACE) to recenter camera\n"
            "(-) zoom out (+) zoom in\n"
            "(UP or click the Up arrow) to go up a directory\n"
            "(F1) to see the help\n"
            "(F2) to enable/disable debug output\n"
            "(F3) to disable/enable blending\n"
            "(F4) to change the control state\n"
            "(F5) to restart at the roots\n"
            "(F6) to enable extra text\n"
            "(F7) to go fullscreen\n"
            "(F8) come back from fullscreen\n"
            "(F9) to render the XY plane\n"
            "(ESC) to exit\n\n"
            "Hit (F1) to make this go away or come back\n");
    }
}

void Display::renderOverLay(void)
{
    if (world->selection.state == SELECTION_CONTEXT_MENU)
    {
        if (world->menu != NULL)
        {
            world->menu->render(world->selection.x,world->selection.y);
        }
    }
}

void Display::renderXYGrid(int xbound, int ybound)
{
	int i,j;

	glColor3ub(255,0,0);

	glBegin(GL_LINES);

	for(i=-xbound;i<=xbound;i++)
	{
		glVertex3f(i,ybound,0);
		glVertex3f(i,-ybound,0);
	}
	for(j=-ybound;j<=ybound;j++)
	{
		glVertex3f(xbound,j,0);
		glVertex3f(-xbound,j,0);
	}

	glEnd();
}

/*
 * Do any post-instatiation initialization.
 */
void Display::postInitialize()
{
	// nothing to do at the moment...
}

