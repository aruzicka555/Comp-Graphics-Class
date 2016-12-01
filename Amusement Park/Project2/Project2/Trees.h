/*
 * Trees.h: Header file for a class that draws trees.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */


#ifndef _Trees_H_
#define _Trees_H_

#include <Fl/gl.h>
#include <GL/glu.h>

class Trees {
  private:
	//the three trees quadric's
    GLUquadric * qobj1;
	GLUquadric * qobjtop1;
	GLUquadric * qobj2;
	GLUquadric * qobjtop2;
	GLUquadric * qobj3;
	GLUquadric * qobjtop3;
    bool    initialized;    // Whether or not we have been initialised.

  public:
    // Constructor. Can't do initialization here because we are
    // created before the OpenGL context is set up.
    Trees(void) { qobj1 = 0; qobjtop1 = 0; qobjtop2 = 0; qobjtop3 = 0; qobj2 = 0; qobj3 = 0; initialized = false; };

    // Destructor. Frees the display lists and texture object.
    ~Trees(void);

    // Initializer. Creates the display list.
    bool    Initialize(void);

    // Does the drawing.
    void    Draw(void);
};


#endif