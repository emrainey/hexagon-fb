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

#include "Hexagon.h" // project's header file

// class constructor
Light::Light(Vector p, Light_Type_e t, int total_num_lights)
{
    number = total_num_lights;
    if (total_num_lights > GL_MAX_LIGHTS)
        return;

    debug->info(DEBUG_TRACE,"Light(%u of %u)\n",number,GL_MAX_LIGHTS);

	memset(ambient,0,sizeof(ambient));
    memset(diffuse,0,sizeof(diffuse));
    memset(specular,0,sizeof(specular));

    position = p;
    type = t;
    cutoff = 180;
}

// class destructor
Light::~Light()
{
	debug->info(DEBUG_TRACE,"~Light(%u)\n",number);
}

/*
 * If the parameter is true then the light shines if false it's turned off.
 * Regardless, it's position is set if it has moved.
 */
void Light::shine(bool on)
{
    if (on == true)
    {
        glEnable(getGLenum());
    }
    else
    {
        glDisable(getGLenum());
    }

    // turn it's position to an array
    static GLfloat position_a[4];
    position_a[0] = position[0];
    position_a[1] = position[1];
    position_a[2] = position[2];
    position_a[3] = 1;
	glLightfv(getGLenum(), GL_POSITION, position_a);

    glBegin(GL_LINES);
        glVertex3f(position.x,position.y,position.z);
        glVertex3f(position.x+direction.x,position.y+direction.y,position.z+direction.z);
    glEnd();
}

GLenum Light::getGLenum(void)
{
    return GL_LIGHT0 + number;
}

// Sets the lights ambient, diffuse, and specular colors
void Light::setColorAttributes(GLfloat *a, GLfloat *d, GLfloat *s)
{
	if (a != NULL)
    {
        ambient[0] = *a++;
        ambient[1] = *a++;
        ambient[2] = *a++;
        ambient[3] = *a++;
        glLightfv(getGLenum(), GL_AMBIENT, ambient);
    }
	if (d != NULL)
    {
        diffuse[0] = *d++;
        diffuse[1] = *d++;
        diffuse[2] = *d++;
        diffuse[3] = *d++;
        glLightfv(getGLenum(), GL_DIFFUSE, diffuse);
    }
	if (s != NULL)
    {
        specular[0] = *s++;
        specular[1] = *s++;
        specular[2] = *s++;
        specular[3] = *s++;
        glLightfv(getGLenum(), GL_DIFFUSE, specular);
    }
}

// Specifies the cutoff
void Light::setCutoff(int c)
{
	cutoff = c;
    glLightfv(getGLenum(),GL_SPOT_CUTOFF,&cutoff);
}

// Sets the direction of the spot light
void Light::setDirection(Vector d)
{
    direction = d;
    static GLfloat dir[4];
    dir[0] = direction[0];
    dir[1] = direction[1];
    dir[2] = direction[2];
    dir[3] = 1;
    glLightfv(getGLenum(),GL_SPOT_DIRECTION,dir);
}

