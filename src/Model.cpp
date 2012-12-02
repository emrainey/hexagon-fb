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

#include "Hexagon.h" // class's header file
#include <l3ds.h>    // include the L3DS loader header file...

extern Port *platform;

Model::Model()
{
    // insert your code here
}

Model::Model(const char *extension)
{
    loadModel(extension);
}

// class destructor
Model::~Model()
{
	// insert your code here
}

void Model::loadDownArrow()
{
    normal = buildDownArrow(0.8,0.8,0.2);   
    selected = buildDownArrow(0.8,0.8,0.2);
    info();
}

// Loads a model from the filesystem
void Model::loadModel(const char *extension)
{
    // fullpath to location
    char fullpath[255];

    // find this file in the format of <extension>.3ds
    Key *dir = platform->ps->getKey("IconDirectory");
    
    sprintf(fullpath,"%s\\3ds\\%s.3ds",dir->getValue().c_str(),extension);
    debug->info(DEBUG_MODELS,"Searching for object: %s\n",fullpath);

    L3DS loader;
    loader.LoadFile(fullpath);
    int mesh_count = loader.GetMeshCount();
    if (mesh_count != 0)
    {
        debug->info(DEBUG_MODELS,"There are %d meshes in the 3ds file\n", mesh_count);
        for (int i=0; i < mesh_count; i++)
        {
            debug->info(DEBUG_MODELS,"Mesh %d has %d vertices and %d triangles\n",
                    i, loader.GetMesh(i).GetVertexCount(),
                    loader.GetMesh(i).GetTriangleCount());
        }
    
        normal = buildMesh(loader,0.7,0.1,0.7,0.3);
        selected = buildMesh(loader,1.0,1.0,1.0,0.7);
    }
    else
    {
        // couldn't find it?
        // just load the hexagon call item for now...
        normal = buildHexagon(0.8,0.2);   
        selected = buildSelectedHexagon(0.8,0.2);
    }
    info();
}

void Model::loadPoly()
{
    normal = buildPoly(0.8,0);
    selected = buildSelPoly(0.8,0);
    info();
}

void Model::info()
{
    debug->info(DEBUG_MODELS,"Loaded Model into GL Call List (%i,%i)\n",normal,selected);
    REPORT_ERROR(info,true);
}

// Renders the model based on it's mode
void Model::render(bool selection, Select_Type_e mode)
{
    if (selection == true)
    {
        IsSelected = false;
        glLoadName(select_name);
        debug->info(DEBUG_SELECTION,"Name,Normal,Selected={0x%08x,%d,%d}\n",select_name,normal,selected);
        REPORT_ERROR(render,true);
    }

    if (mode == NORMAL)
    {
        //glutSolidTorus(0.2,0.6,20,20);
        //renderHexagon(0.8,0.2);    
        glCallList(normal);
    }
    else if (mode == SELECTED)  
    {
        //glutSolidTorus(0.2,0.6,20,20);
        //renderSelectedHexagon(0.8,0.2);
        glCallList(selected);
    }
    REPORT_ERROR(render,true);        
}


/*
 * Loads the mesh into a Call List (returning the call name) 
 * with the parameterized color.
 */
GLuint Model::buildMesh(L3DS &loader, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	GLuint shape = glGenLists(1);
	CHECKERROR(shape,false);
	glNewList(shape,GL_COMPILE);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_CULL_FACE);
   
    glColor4f(r,g,b,a);
    for (uint i = 0; i < loader.GetMeshCount(); i++)
    {
        LMesh &mesh = loader.GetMesh(i);

        glVertexPointer(4, GL_FLOAT, 0, &mesh.GetVertex(0));
        glNormalPointer(GL_FLOAT, 0, &mesh.GetNormal(0));
        //glColorPointer(3, GL_FLOAT, 0, &mesh.GetBinormal(0));
        glDrawElements(GL_TRIANGLES, mesh.GetTriangleCount()*3, 
                        GL_UNSIGNED_SHORT, &mesh.GetTriangle(0));
    }

    glDisable(GL_CULL_FACE);
    //glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    
	glEndList();

	return shape;
}

/****************************************************************************
 *        Name:
 *  Parameters:
 *     Returns:
 * Description:
 ****************************************************************************/
GLuint Model::buildHexagon(double radius, double height)
{
	static GLuint shape = 0;
    static double last_radius = 0;
    static double last_height = 0;

    if ( shape != 0 && last_radius == radius && last_height == height )
        return shape;

	shape = glGenLists(1);
	CHECKERROR(shape,false);
	glNewList(shape,GL_COMPILE);
	//glutSolidTorus(0.2,0.6,20,20);
    renderHexagon(radius,height);
	glEndList();

    REPORT_ERROR(buildHexagon,false);

	last_height = height;
	last_radius = radius;

	return shape;
}

void Model::renderHexagon(double radius, double height)
{
	glColor4f(0.7,0.1,0.7,0.3);
	solidHexagon(radius,height);
	glColor4f(1.0,1.0,1.0,0.7);
	wireHexagon(radius,height);
}


/****************************************************************************
 *        Name:
 *  Parameters:
 *     Returns:
 * Description:
 ****************************************************************************/
GLuint Model::buildSelectedHexagon(double radius, double height)
{
    static GLuint shape = 0;
    static double last_radius = 0;
    static double last_height = 0;

    if ( shape != 0 && last_radius == radius && last_height == height )
        return shape;

	shape = glGenLists(1);
	CHECKERROR(shape,false);
	glNewList(shape,GL_COMPILE);
	//glutSolidTorus(0.2,0.6,20,20);
    renderSelectedHexagon(radius,height);
	glEndList();

    REPORT_ERROR(buildHexagon,false);

	last_height = height;
	last_radius = radius;

	return shape;
}

void Model::renderSelectedHexagon(double radius, double height)
{
    glColor4f(0.8,0.1,0.0,0.3);
    solidHexagon(radius,height);
	glColor4f(1.0,1.0,1.0,0.7);
	wireHexagon(radius,height);
}
/****************************************************************************
 *        Name:
 *  Parameters:
 *     Returns:
 * Description:
 ****************************************************************************/
GLuint Model::buildPoly(double radius, double height)
{
	static GLuint shape = 0;
    static double last_radius = 0;
    static double last_height = 0;

    if ( shape != 0 && last_radius == radius && last_height == height )
        return shape;

	shape = glGenLists(1);
	CHECKERROR(shape,false);
	glNewList(shape,GL_COMPILE);
    renderPoly(radius,height);
	glEndList();

	last_height = height;
	last_radius = radius;

	return shape;
}

void Model::renderPoly(double radius, double height)
{
	glColor4f(0.7,0.1,0.7,0.3);
	HexPoly(radius,height);
	glColor4f(1.0,1.0,1.0,0.7);
	HexWirePoly(radius,height);
}

/****************************************************************************
 *        Name:
 *  Parameters:
 *     Returns:
 * Description:
 ****************************************************************************/
GLuint Model::buildSelPoly(double radius, double height)
{
	static GLuint shape = 0;
    static double last_radius = 0;
    static double last_height = 0;

    if ( shape != 0 && last_radius == radius && last_height == height )
        return shape;

	shape = glGenLists(1);
	CHECKERROR(shape,false);
	glNewList(shape,GL_COMPILE);
    renderSelPoly(radius,height);
	glEndList();

	last_height = height;
	last_radius = radius;

	return shape;
}

void Model::renderSelPoly(double radius, double height)
{
	glColor4f(0.8,0.1,0.0,0.3);
	HexPoly(radius,height);
	glColor4f(1.0,1.0,1.0,0.7);
	HexWirePoly(radius,height);
}

/****************************************************************************
 *        Name:
 *  Parameters:
 *     Returns:
 * Description:
 ****************************************************************************/
GLuint Model::buildDownArrow(double length, double width, double height)
{
    static GLuint shape = 0;

    if ( shape != 0 )
        return shape;

	shape = glGenLists(1);
	CHECKERROR(shape,false);
	glNewList(shape,GL_COMPILE);
    //glRotated(-90,1,0,0);
    renderDownArrow(length,width,height);
	glEndList();
	return shape;
}

void Model::renderDownArrow(double length, double width, double height)
{
    glColor4f(0.7,0.1,0.7,0.3);
    solidDownArrow(length,width,height);
    glColor4f(1.0,1.0,1.0,0.7);
    wireDownArrow(length,width,height);
}

/****************************************************************************
 *        Name:
 *  Parameters:
 *     Returns:
 * Description:
 ****************************************************************************/
void Model::wireHexagon(double radius, double height)
{
	glBegin(GL_LINES);
		glVertex3d(radius*COS(0)  ,radius*SIN(0)	,height);
		glVertex3d(radius*COS(60) ,radius*SIN(60)	,height);
		glVertex3d(radius*COS(60) ,radius*SIN(60)	,height);
		glVertex3d(radius*COS(120),radius*SIN(120)	,height);
		glVertex3d(radius*COS(120),radius*SIN(120)	,height);
		glVertex3d(radius*COS(180),radius*SIN(180)	,height);
		glVertex3d(radius*COS(180),radius*SIN(180)	,height);
		glVertex3d(radius*COS(240),radius*SIN(240)	,height);
		glVertex3d(radius*COS(240),radius*SIN(240)	,height);
		glVertex3d(radius*COS(300),radius*SIN(300)	,height);
		glVertex3d(radius*COS(300),radius*SIN(300)	,height);
		glVertex3d(radius*COS(0)  ,radius*SIN(0)	,height);

		glVertex3d(radius*COS(0)  ,radius*SIN(0)	,0);
		glVertex3d(radius*COS(60) ,radius*SIN(60)	,0);
		glVertex3d(radius*COS(60) ,radius*SIN(60)	,0);
		glVertex3d(radius*COS(120),radius*SIN(120)	,0);
		glVertex3d(radius*COS(120),radius*SIN(120)	,0);
		glVertex3d(radius*COS(180),radius*SIN(180)	,0);
		glVertex3d(radius*COS(180),radius*SIN(180)	,0);
		glVertex3d(radius*COS(240),radius*SIN(240)	,0);
		glVertex3d(radius*COS(240),radius*SIN(240)	,0);
		glVertex3d(radius*COS(300),radius*SIN(300)	,0);
		glVertex3d(radius*COS(300),radius*SIN(300)	,0);
		glVertex3d(radius*COS(0)  ,radius*SIN(0)	,0);

		glVertex3d(radius*COS(0)  ,radius*SIN(0)	,height);
		glVertex3d(radius*COS(0)  ,radius*SIN(0)	,0);

		glVertex3d(radius*COS(60) ,radius*SIN(60)	,height);
		glVertex3d(radius*COS(60) ,radius*SIN(60)	,0);

		glVertex3d(radius*COS(120),radius*SIN(120)	,height);
		glVertex3d(radius*COS(120),radius*SIN(120)	,0);

		glVertex3d(radius*COS(180),radius*SIN(180)	,height);
		glVertex3d(radius*COS(180),radius*SIN(180)	,0);

		glVertex3d(radius*COS(240),radius*SIN(240)	,height);
		glVertex3d(radius*COS(240),radius*SIN(240)	,0);

		glVertex3d(radius*COS(300),radius*SIN(300)	,height);
		glVertex3d(radius*COS(300),radius*SIN(300)	,0);
	glEnd();
}

/****************************************************************************
 *        Name:
 *  Parameters:
 *     Returns:
 * Description:
 ****************************************************************************/
void Model::solidHexagon(double radius, double height)
{
	/* top face */
	glBegin(GL_POLYGON);
		glVertex3d(radius*COS(0)  ,radius*SIN(0)	,height);
		glVertex3d(radius*COS(60) ,radius*SIN(60)	,height);
		glVertex3d(radius*COS(120),radius*SIN(120)	,height);
		glVertex3d(radius*COS(180),radius*SIN(180)	,height);
		glVertex3d(radius*COS(240),radius*SIN(240)	,height);
		glVertex3d(radius*COS(300),radius*SIN(300)	,height);
	glEnd();

	/* bottom face (top face reverse order) */
	glBegin(GL_POLYGON);
		glVertex3d(radius*COS(300),radius*SIN(300)	,0);
		glVertex3d(radius*COS(240),radius*SIN(240)	,0);
		glVertex3d(radius*COS(180),radius*SIN(180)	,0);
		glVertex3d(radius*COS(120),radius*SIN(120)	,0);
		glVertex3d(radius*COS(60) ,radius*SIN(60)	,0);
		glVertex3d(radius*COS(0)  ,radius*SIN(0)	,0);
	glEnd();

	/* upper right face */
	glBegin(GL_QUADS);
		glVertex3d(radius*COS(0)  ,radius*SIN(0)	,height);
		glVertex3d(radius*COS(0)  ,radius*SIN(0)	,0);
		glVertex3d(radius*COS(60) ,radius*SIN(60)	,0);
		glVertex3d(radius*COS(60) ,radius*SIN(60)	,height);
	glEnd();

	/* upper center face */
	glBegin(GL_QUADS);
		glVertex3d(radius*COS(60) ,radius*SIN(60)	,height);
		glVertex3d(radius*COS(60) ,radius*SIN(60)	,0);
		glVertex3d(radius*COS(120),radius*SIN(120)	,0);
		glVertex3d(radius*COS(120),radius*SIN(120)	,height);
	glEnd();

	/* upper left face */
	glBegin(GL_QUADS);
		glVertex3d(radius*COS(120),radius*SIN(120)	,height);
		glVertex3d(radius*COS(120),radius*SIN(120)	,0);
		glVertex3d(radius*COS(180),radius*SIN(180)	,0);
		glVertex3d(radius*COS(180),radius*SIN(180)	,height);
	glEnd();

	/* lower left face */
	glBegin(GL_QUADS);
		glVertex3d(radius*COS(180),radius*SIN(180)	,height);
		glVertex3d(radius*COS(180),radius*SIN(180)	,0);
		glVertex3d(radius*COS(240),radius*SIN(240)	,0);
		glVertex3d(radius*COS(240),radius*SIN(240)	,height);
	glEnd();

	/* lower center face */
	glBegin(GL_QUADS);
		glVertex3d(radius*COS(240),radius*SIN(240)	,height);
		glVertex3d(radius*COS(240),radius*SIN(240)	,0);
		glVertex3d(radius*COS(300),radius*SIN(300)	,0);
		glVertex3d(radius*COS(300),radius*SIN(300)	,height);
	glEnd();

	/* lower right face */
	glBegin(GL_QUADS);
		glVertex3d(radius*COS(300),radius*SIN(300)	,height);
		glVertex3d(radius*COS(300),radius*SIN(300)	,0);
		glVertex3d(radius*COS(0)  ,radius*SIN(0)	,0);
		glVertex3d(radius*COS(0)  ,radius*SIN(0)	,height);
	glEnd();
}

/****************************************************************************
 *        Name:
 *  Parameters:
 *     Returns:
 * Description:
 ****************************************************************************/
void Model::wireDownArrow(double length, double width, double height)
{
    glBegin(GL_LINES);

        /* Top Plane */

		/* 1 - 2 */
		glVertex3d(0,length/2,height);
		glVertex3d(width/2,0,height);

		/* 2 - 3 */
		glVertex3d(width/2,0,height);
		glVertex3d(width/4,0,height);

		/* 3 - 4 */
		glVertex3d(width/4,0,height);
		glVertex3d(width/4,-length/2,height);

		/* 4 - 5 */
		glVertex3d(width/4,-length/2,height);
		glVertex3d(-width/4,-length/2,height);

		/* 5 - 6 */
		glVertex3d(-width/4,-length/2,height);
		glVertex3d(-width/4,0,height);

		/* 6 - 7 */
		glVertex3d(-width/4,0,height);
		glVertex3d(-width/2,0,height);

        /* 7 - 1 */
		glVertex3d(-width/2,0,height);
		glVertex3d(0,length/2,height);

        /* Bottom Plane */

		/* 1 - 2 */
		glVertex3d(0,length/2,0);
		glVertex3d(width/2,0,0);

		/* 2 - 3 */
		glVertex3d(width/2,0,0);
		glVertex3d(width/4,0,0);

		/* 3 - 4 */
		glVertex3d(width/4,0,0);
		glVertex3d(width/4,-length/2,0);

		/* 4 - 5 */
		glVertex3d(width/4,-length/2,0);
		glVertex3d(-width/4,-length/2,0);

		/* 5 - 6 */
		glVertex3d(-width/4,-length/2,0);
		glVertex3d(-width/4,0,0);

		/* 6 - 7 */
		glVertex3d(-width/4,0,0);
		glVertex3d(-width/2,0,0);

        /* 7 - 1 */
		glVertex3d(-width/2,0,0);
		glVertex3d(0,length/2,0);

        /* Side lines */

   		/* 1 - 1 */
		glVertex3d(0,length/2,height);
		glVertex3d(0,length/2,0);

		/* 2 - 2 */
		glVertex3d(width/2,0,height);
		glVertex3d(width/2,0,0);

		/* 3 - 3 */
		glVertex3d(width/4,0,height);
		glVertex3d(width/4,0,0);

		/* 4 - 4 */
		glVertex3d(width/4,-length/2,height);
		glVertex3d(width/4,-length/2,0);

		/* 5 - 5 */
		glVertex3d(-width/4,-length/2,height);
		glVertex3d(-width/4,-length/2,0);

		/* 6 - 6 */
		glVertex3d(-width/4,0,height);
		glVertex3d(-width/4,0,0);

        /* 7 - 7 */
		glVertex3d(-width/2,0,height);
		glVertex3d(-width/2,0,0);

	glEnd();
}

/****************************************************************************
 *        Name:
 *  Parameters:
 *     Returns:
 * Description:
 ****************************************************************************/
void Model::solidDownArrow(double length, double width, double height)
{
	glBegin(GL_POLYGON);
		glVertex3d(0,length/2,height);
		glVertex3d(width/2,0,height);
		glVertex3d(width/4,0,height);
		glVertex3d(width/4,-length/2,height);
		glVertex3d(-width/4,-length/2,height);
		glVertex3d(-width/4,0,height);
		glVertex3d(-width/2,0,height);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex3d(0,length/2,0);
		glVertex3d(width/2,0,0);
		glVertex3d(width/4,0,0);
		glVertex3d(width/4,-length/2,0);
		glVertex3d(-width/4,-length/2,0);
		glVertex3d(-width/4,0,0);
		glVertex3d(-width/2,0,0);
	glEnd();

	glBegin(GL_POLYGON);
	    glVertex3d(0,length/2,height);
	    glVertex3d(0,length/2,0);
	    glVertex3d(width/2,0,0);
		glVertex3d(width/2,0,height);
	glEnd();

    glBegin(GL_POLYGON);
        glVertex3d(width/2,0,height);
		glVertex3d(width/2,0,0);
		glVertex3d(width/4,0,0);
		glVertex3d(width/4,0,height);
    glEnd();

    glBegin(GL_POLYGON);
        glVertex3d(width/4,0,height);
        glVertex3d(width/4,0,0);
		glVertex3d(width/4,-length/2,0);
		glVertex3d(width/4,-length/2,height);
    glEnd();

    glBegin(GL_POLYGON);
		glVertex3d(width/4,-length/2,height);
		glVertex3d(width/4,-length/2,0);
    	glVertex3d(-width/4,-length/2,0);
   		glVertex3d(-width/4,-length/2,height);
    glEnd();

    glBegin(GL_POLYGON);
		glVertex3d(-width/4,-length/2,height);
		glVertex3d(-width/4,-length/2,0);
   		glVertex3d(-width/4,0,0);
		glVertex3d(-width/4,0,height);
    glEnd();

    glBegin(GL_POLYGON);
		glVertex3d(-width/4,0,height);
   		glVertex3d(-width/4,0,0);
   		glVertex3d(-width/2,0,0);
   		glVertex3d(-width/2,0,height);
    glEnd();

    glBegin(GL_POLYGON);
		glVertex3d(-width/2,0,height);
		glVertex3d(-width/2,0,0);
   		glVertex3d(0,length/2,0);
		glVertex3d(0,length/2,height);
    glEnd();
}


// Renders a single polygon
void Model::HexPoly(double radius, double height)
{
	glBegin(GL_POLYGON);
		glVertex3d(radius*COS(0)  ,radius*SIN(0)	,height);
		glVertex3d(radius*COS(60) ,radius*SIN(60)	,height);
		glVertex3d(radius*COS(120),radius*SIN(120)	,height);
		glVertex3d(radius*COS(180),radius*SIN(180)	,height);
		glVertex3d(radius*COS(240),radius*SIN(240)	,height);
		glVertex3d(radius*COS(300),radius*SIN(300)	,height);
	glEnd();
}

// Renders a single polygon wire frame
void Model::HexWirePoly(double radius, double height)
{
    glBegin(GL_LINES);
		glVertex3d(radius*COS(0)  ,radius*SIN(0)	,height);
		glVertex3d(radius*COS(60) ,radius*SIN(60)	,height);
		glVertex3d(radius*COS(60) ,radius*SIN(60)	,height);
		glVertex3d(radius*COS(120),radius*SIN(120)	,height);
		glVertex3d(radius*COS(120),radius*SIN(120)	,height);
		glVertex3d(radius*COS(180),radius*SIN(180)	,height);
		glVertex3d(radius*COS(180),radius*SIN(180)	,height);
		glVertex3d(radius*COS(240),radius*SIN(240)	,height);
		glVertex3d(radius*COS(240),radius*SIN(240)	,height);
		glVertex3d(radius*COS(300),radius*SIN(300)	,height);
		glVertex3d(radius*COS(300),radius*SIN(300)	,height);
		glVertex3d(radius*COS(0)  ,radius*SIN(0)	,height);
    glEnd();
}

