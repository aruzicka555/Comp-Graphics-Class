/*
 * Wheel.h: Header file for a class that draws the ferris-wheel.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */


#ifndef _Wheel_H_
#define _Wheel_H_

#include <Fl/gl.h>
#include <GL/glu.h>

class Wheel {
  private:
	//the three balloons quadric's
    GLUquadric * i_wheel;
	GLUquadric * o_wheel;
	GLUquadric * disk1;
	GLUquadric * disk2;
	GLUquadric * spoke1;
	GLUquadric * spoke2;
    bool    initialized;    // Whether or not we have been initialised.

  public:
    // Constructor. Can't do initialization here because we are
    // created before the OpenGL context is set up.
    Wheel(void) { i_wheel = 0; spoke1 = 0; spoke2 = 0; o_wheel = 0; disk1 = 0; disk2 = 0; initialized = false; };

    // Destructor. Frees the display lists and texture object.
    ~Wheel(void);

    // Initializer. Creates the display list.
    bool    Initialize(void);

    // Does the drawing.
    void    Draw(void);
};


#endif