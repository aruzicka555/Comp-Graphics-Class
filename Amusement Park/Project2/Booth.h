/*
 * Booth.h: Header file for a class that draws a ticket booth.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */


#ifndef _BOOTH_H_
#define _BOOTH_H_

#include <Fl/gl.h>

class Booth {
  private:
    GLubyte front_list;   // The display list for the front that does all the work.
	GLubyte bottom_list;
	GLubyte left_list;
	GLubyte right_list;
	GLubyte back_list;
	GLubyte	roof_list;		//the display list for the roof that does all the work
    GLuint  texture_obj1;    // The object for the booth sides texture.
	GLuint  texture_obj2;		//The object for the front of the booth texture
	GLuint	texture_obj3;		//the texture for the roof of the booth
    bool    initialized;    // Whether or not we have been initialised.

  public:
    // Constructor. Can't do initialization here because we are
    // created before the OpenGL context is set up.
    Booth(void) { front_list = 0; bottom_list = 0; left_list = 0; right_list = 0; back_list = 0; roof_list = 0; initialized = false; };

    // Destructor. Frees the display lists and texture object.
    ~Booth(void);

    // Initializer. Creates the display list.
    bool    Initialize(void);

    // Does the drawing.
    void    Draw(void);
};


#endif
