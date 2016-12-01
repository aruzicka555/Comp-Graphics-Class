/*
 * Bench.h: Header file for a class that draws a bench.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */


#ifndef _BENCH_H_
#define _BENCH_H_

#include <Fl/gl.h>

class Bench {
  private:
    GLubyte front_front;
	GLubyte front_left;
	GLubyte front_right;
	GLubyte front_back;
	GLubyte seat_front;
	GLubyte	seat_left;
	GLubyte	seat_right;
	GLubyte	seat_top;
	GLubyte	back_front;
	GLubyte	back_left;
	GLubyte	back_right;
	GLubyte	back_top;
	GLubyte	back_back;
	
    GLuint  texture_obj1;    // The object for the booth sides texture.
    bool    initialized;    // Whether or not we have been initialised.

  public:
    // Constructor. Can't do initialization here because we are
    // created before the OpenGL context is set up.
    Bench(void) { front_front = 0; front_left = 0; front_right = 0; front_back = 0; seat_front = 0; seat_left = 0; seat_right = 0; seat_top = 0; back_front = 0; back_left = 0; back_right = 0; back_back = 0; back_top = 0; initialized = false; };

    // Destructor. Frees the display lists and texture object.
    ~Bench(void);

    // Initializer. Creates the display list.
    bool    Initialize(void);

    // Does the drawing.
    void    Draw(void);
};


#endif