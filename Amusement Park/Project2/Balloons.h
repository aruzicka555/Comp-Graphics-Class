/*
 * Balloons.h: Header file for a class that draws balloons.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */


#ifndef _Balloons_H_
#define _Balloons_H_

#include <Fl/gl.h>
#include <GL/glu.h>

class Balloons {
  private:
	//the three balloons quadric's
    GLUquadric * qobj1;
	GLUquadric * qobjtop1;
	GLUquadric * qobj2;
	GLUquadric * qobjtop2;
	GLUquadric * qobj3;
	GLUquadric * qobjtop3;
	float height1;
	float height2;
	float height3;
	float rotation;

    bool    initialized;    // Whether or not we have been initialised.

  public:
    // Constructor. Can't do initialization here because we are
    // created before the OpenGL context is set up.
    Balloons(void) { qobj1 = 0; qobjtop1 = 0; qobjtop2 = 0; qobjtop3 = 0; qobj2 = 0; qobj3 = 0; initialized = false; height1 = 5.0f; height2 = 10.0f; height3 = 20.0f; rotation = 0.0f;};

    // Destructor. Frees the display lists and texture object.
    ~Balloons(void);

    // Initializer. Creates the display list.
    bool    Initialize(void);

    // Does the drawing.
    void    Draw(void);

	void	Update(float);
};


#endif