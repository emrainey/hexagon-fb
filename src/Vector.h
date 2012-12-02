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

#ifndef VECTOR_H
#define VECTOR_H

/**
 * The generic, everyone-has-one, Vector math class.
 * This one is taken from the "Physics for Game Developers" by David M. Bourg.
 * Since I know I'm not nearly as good at this as he is I'll follow his lead
 * with one minor change, I like to call the Absolute Value of a Vector,
 * an Abs() call instead of the longer Magnitude().
 * And I'd prefer doubles instead of floats but what the heck...oh,  Appendix A.
 */
class Vector
{
	public:
	    // the vector components
        float x,y,z;
		// class constructor
		Vector();
		Vector(float xi,float yi,float zi);

		// Absolute Value
		float abs();
		// Normalize
		void normalize();
		// Reverse the vector
		void reverse();
		// set the vector to zero
		void zeroize();
        // set the values
        void set(double a, double b, double c);    

        float getAngleWith(Vector a);
        float getAngleX();
        float getAngleY();
        float getAngleZ();
        
        // Now we use C++ like it was meant to be used, to overload cool shit!
		Vector& operator+=(Vector a); // simple addition
        Vector& operator-=(Vector a); // simple subtract
		Vector& operator*=(float s); // scalar multiply
		Vector& operator/=(float s); // scalar divide
        Vector& operator=(Vector a); // set
        bool operator==(Vector a); // are abs() equal?
        Vector operator-(void);    // same as reverse
        float operator[](int i);   // index the vector components by index
                                   // x = 0, y = 1, z = 2;
        
		// Returns a string containing debug info for vector
		char * print(void);
};

Vector operator+(Vector u, Vector v); // vec addition
Vector operator-(Vector u, Vector v); // vec subtraction
float operator*(Vector u, Vector v); // dot product
Vector operator^(Vector u, Vector v); //cros product

#endif // VECTOR_H

