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

#ifndef MODEL_H
#define MODEL_H

#include <l3ds.h>

typedef enum
{
    NORMAL,
    SELECTED,
    NUM_SELECT_TYPES
} Select_Type_e;

// This is the wrapper class around loading 3ds models from the FileSystem.
class Model
{
	public:
        // the normal and selected Call List items
        GLuint normal;
        GLuint selected;
    
        // this is the GL Name of the model used during rendering and selection
        GLuint select_name;

        // a bool to keep around to determine if the model has been selected.
        bool IsSelected;        

		// class constructor
        Model();
		Model(const char *extension);

		// class destructor
		~Model();

		// Loads a model from the filesystem
		void loadModel(const char *extension);

        // loads the Up Arrow Object
        void loadDownArrow();

        // loads a hexagonal polygon with wire outline
        void loadPoly();

        // a little debug
        void info();

		// Renders the model based on it's mode
		void render(bool selection, Select_Type_e mode);

        // we only need one copy of the following stuff...
        static GLuint buildHexagon(double radius, double height);
        static GLuint buildSelectedHexagon(double radius, double height);
        static GLuint buildDownArrow(double length, double width, double height);
        static GLuint buildPoly(double radius, double height);
        static GLuint buildSelPoly(double radius, double height);

        static void renderHexagon(double radius, double height);
        static void renderSelectedHexagon(double radius, double height);
        static void renderDownArrow(double length, double width, double height);
        static void renderPoly(double radius, double height);
        static void renderSelPoly(double radius, double height);

        static void wireHexagon(double radius, double height);
        static void solidHexagon(double radius, double height);
        static void wireDownArrow(double length, double width, double height);
        static void solidDownArrow(double length, double width, double height);
		static void HexPoly(double radius, double height);
		static void HexWirePoly(double radius, double height);
	protected:
		/*
		 * Loads the mesh into a Call List (returning the call name) 
		 * with the parameterized color.
		 */
		GLuint buildMesh(L3DS &loader, GLfloat r, GLfloat g, GLfloat b, GLfloat a);
};

#endif // MODEL_H

