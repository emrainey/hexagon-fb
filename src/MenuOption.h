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

#ifndef MENUOPTION_H
#define MENUOPTION_H

// A single menu option
class MenuOption
{
    public:
        /** Allow child drestructors to be called! */
        virtual ~MenuOption() = 0;
    
		/**
         * Determines is a point was selected in the rectangle of the option.
         * @param index The index of the option in the menu, zero based.
         * @param x The x value of the selection point.
         * @param y The y value of the selection point.
         */
		virtual bool choose(int index, int x, int y) = 0;
		
		/**
         * Renders the menu option
         * @param index The index of the option in the menu, zero based.
         * @param ix The x value of the render point.
         * @param iy The y value of the render point.
         */
		virtual void render(int index, int ix, int iy) = 0;
		
		/**
         * Sets the MenuOptions Width.
         * @param width The new width.
         */
		virtual void setWidth(int width) = 0;
		
		/**
		 * Sets the MenuOption's Inset which is the distance from one
		 * menu item to it's border.
         * @param i The inset to use around all items.
		 */
		virtual void setInset(int i) = 0;
		
		/**
		 * @param n Sets the name of the option.
		 */
		virtual string setName(string n) = 0;
		
		/**
		 * @return Returns the name of the MenuOption
		 */
		virtual string getName() = 0;
		
        /**
         * Sets the ShellExecutor Object to call once the item is called.
         * @param se A pointer to a shell executor object, initialized to the
         *           correct parameters.
         * @param parameters The parameters to pass to the executor.
         */
        virtual void setShellExecutor(ShellExecutor *sh) = 0;
        
        /**
         * Calls the ShellExecutor object to execute the command.
         * @param parameters The parameters to pass to the executor.
         */
        virtual void execute(string parameters) = 0;
        
        /**
         * Sets the index of this MenuOption in the menu, zero based.
         * @param index Zero based index.
         */
        virtual void setIndex(int index) = 0;
};


#endif // MENUOPTION_H

