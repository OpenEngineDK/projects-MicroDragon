// utilities.cpp,  29.1.07, pm-n.
#include "utilities.h"
//#include "../setup.h"

#include <Utils/Timer.h>
#include <Meta/OpenGL.h>
#include <math.h>

using OpenEngine::Utils::Timer;

float Abs(float a) { return (a>0?a:-a); }
double Abs(double a) { return (a>0?a:-a); }
int Abs(int a) { return (a>0?a:-a); }

float customFmod( float i, float mMod ){
    float tmp = fmod( i, mMod);
    return (tmp>=0 ? tmp : tmp+mMod);
}

void drawTriangleQuad(Vec3 point[4], Vec3 normal[4], bool flipDiagonal) {
    int order[] = {0,1,2,2,3,0};
    for (int i=0; i<6; i++) {
        int j = order[i];
        if (flipDiagonal) j = (j+1)%4;
        glNormal3f( normal[j].x, normal[j].y, normal[j].z );
        glVertex3f( point[j].x,  point[j].y,  point[j].z  );
    }
}

void drawColorTriangleQuad(Vec3 point[4], Vec3 normal[4], Vec3 color[4],Vec3 tex[4], bool flipDiagonal) {
    int order[] = {0,1,2,2,3,0};
    for (int i=0; i<6; i++) {
        int j = order[i];
        if (flipDiagonal) j = (j+1)%4;
        glColor3f(  color[j].x,  color[j].y,  color[j].z  );
        glTexCoord2f( tex[j].x, tex[j].y );
        glNormal3f( normal[j].x, normal[j].y, normal[j].z );
        glVertex3f( point[j].x,  point[j].y,  point[j].z  );
    }
}

void drawUVTriangleQuad(Vec3 point[4], Vec3 normal[4], float u[4], float v[4], bool flipDiagonal) {
    int order[] = {0,1,2,2,3,0};
    for (int i=0; i<6; i++) {
        int j = order[i];
        if (flipDiagonal) j = (j+1)%4;
        glNormal3f( normal[j].x, normal[j].y, normal[j].z );
        glVertex3f( point[j].x,  point[j].y,  point[j].z  );
    }
}

void draw_pyramid() {
    /*  bottom  */

    glBegin( GL_QUADS );
    glNormal3f( 0.0, 0.0, -1.0 );
    glVertex3f( 0.5, -0.5, 0.0 );
    glVertex3f( -0.5, -0.5, 0.0 );
    glVertex3f( -0.5, 0.5, 0.0 );
    glVertex3f( 0.5, 0.5, 0.0 );
    glEnd();

    /*  right  */

    glBegin( GL_TRIANGLES );
    glNormal3f( 0.5, 0.0, 0.5 );
    glVertex3f( 0.5, -0.5, 0.0 );
    glVertex3f( 0.5, 0.5, 0.0 );
    glVertex3f( 0.0, 0.0, 1.0 );

    /*  back  */

    glNormal3f( 0.0, 0.5, 0.5 );
    glVertex3f( 0.5, 0.5, 0.0 );
    glVertex3f( -0.5, 0.5, 0.0 );
    glVertex3f( 0.0, 0.0, 1.0 );

    /*  left  */

    glNormal3f( -0.5, 0.0, 0.5 );
    glVertex3f( -0.5, 0.5, 0.0 );
    glVertex3f( -0.5, -0.5, 0.0 );
    glVertex3f( 0.0, 0.0, 1.0 );

    /*  front  */

    glNormal3f( 0.0, -0.5, 0.5 );
    glVertex3f( -0.5, -0.5, 0.0 );
    glVertex3f( 0.5, -0.5, 0.0 );
    glVertex3f( 0.0, 0.0, 1.0 );
    glEnd();
}

double  wall_time() {
  return Timer::GetTime();
  /*
#if defined(_WIN32)
    return GetTickCount()/1000.0f;
#else
    struct timeval  t;
    double ts, tus;
    gettimeofday( &t, NULL );
    ts = t.tv_sec;
    tus = t.tv_usec;
    return ( ts + tus/1000000.0 );
#endif
  */
}

float  gaussPeak( float my_x, float my_y,
                  float sigma_x, float sigma_y,
                  float x, float y )
{
    float xn, yn;

    xn = (x-my_x)/sigma_x;
    yn = (y-my_y)/sigma_y;

    return ( exp( -(xn*xn + yn*yn ) ) );
}
