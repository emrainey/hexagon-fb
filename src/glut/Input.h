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

#ifndef INPUT_H
#define INPUT_H

// No description
class Input
{
	public:
	    int lastx, lasty;
		// class constructor
		Input();
		// class destructor
		~Input();
		// GLUT Mouse Drag Callback
		void mouseDrag(int x, int y);
		// GLUT Keyboard Callback for Special Keys
		void keyboardSpecial(int key, int x, int y);
		// GLUT Keyboard Callback for Normal Keys
		void keyboardNormal(unsigned char key, int x, int y);
		// GLUT Mouse Movement Callback
		void mouseMove(int x, int y);
		// GLUT Mouse Click Callback
		void mouseClick(int button, int state, int x, int y);

};

#endif // INPUT_H

