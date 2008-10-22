#ifndef _OPENGL_UTIL_H_
#define _OPENGL_UTIL_H_

#include <Meta/OpenGL.h>
//#include <Meta/GLUT.h>

//#include "../setup.h"

#include <math.h>
#include <stdlib.h>

class OpenGLUtil {
public:
    OpenGLUtil();
    ~OpenGLUtil();

    static void fghCircleTable(double **sint,double **cost,const int n);
    static void GLSolidSphere(double radius, int slices, int stacks);
    static void GLSolidCube( double dSize );
    static void GLWireCube( double dSize );

};


#endif
