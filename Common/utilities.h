//   utilities.h,  29.1.07, pm-n.

#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <Math/Vector.h>

using OpenEngine::Math::Vector;

float Abs(float a);
double Abs(double a);
int Abs(int a);

float customFmod( float i, float mMod );
void drawTriangleQuad(Vector<3,float> point[4], Vector<3,float> normal[4], bool flipDiagonal);
void drawColorTriangleQuad(Vector<3,float> point[4], Vector<3,float> normal[4], Vector<3,float> color[4], Vector<3,float> tex[4], bool flipDiagonal);
void drawUVTriangleQuad(Vector<3,float> point[4], Vector<3,float> normal[4], float u[4], float v[4], bool flipDiagonal);

void    draw_pyramid();

//double  wall_time();

float   gaussPeak( float my_x, float my_y,
                   float sigma_x, float sigma_y,
                   float x, float y );

#endif
