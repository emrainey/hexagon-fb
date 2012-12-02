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

#ifndef _COLOR_H_
#define _COLOR_H_

class Color
{       
        //************
        // VARIABLES
        //************
    public:
        enum EColorIndex { Red = 0, Green, Blue, Alpha, Channels };
    private:
        float colors[Channels];
            
        //*********
        // METHODS
        //*********        
    public:    
        Color();
        Color(float r, float g, float b, float a);
        Color(int seed);
        ~Color();
        
        inline unsigned char getRedByte()   { return ((unsigned char)colors[Red]* 255);   }
        inline unsigned char getGreenByte() { return ((unsigned char)colors[Green]* 255); }
        inline unsigned char getBlueByte()  { return ((unsigned char)colors[Blue]* 255);  }
        inline unsigned char getAlphaByte() { return ((unsigned char)colors[Alpha]* 255); }
        inline float getRedFloat()   { return colors[Red];   }
        inline float getGreenFloat() { return colors[Green]; }
        inline float getBlueFloat()  { return colors[Blue];  }
        inline float getAlphaFloat() { return colors[Alpha]; }
               
        void setRedFloat(float value);
        void setGreenFloat(float value);
        void setBlueFloat(float value);
        void setAlphaFloat(float value);
                  
        void print();
        inline float *getFloatArray() { return colors; }
        float operator[](int index);
        
        void blendWith(Color *other);
        void perturb(int seed);
        
    private:
        void setColorIndex(int index, float value);
};


#endif // _COLOR_H
