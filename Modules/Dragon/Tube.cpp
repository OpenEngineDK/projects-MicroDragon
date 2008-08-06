#include "Tube.h"

#include "../../Common/OpenGLUtil.h"
#include "../../Common/utilities.h"

#include <Math/Math.h>

using OpenEngine::Math::PI;

Tube::Tube(float radius1, float radius2, float stiffness1, float stiffness2, float length) {
    this->length = length;
    this->radius1 = radius1;
    this->radius2 = radius2;
    this->stiffness1 = stiffness1;
    this->stiffness2 = stiffness2;
    adjuster = 10.0;
    twistAngle = 0.0;
}

Tube::~Tube() {
}

float Tube::getLinkRadius(int linkIndex) { return linkR[linkIndex]; }
Vec3 Tube::getLinkX(int linkIndex) { return linkX[linkIndex]; }
Vec3 Tube::getLinkY(int linkIndex) { return linkY[linkIndex]; }
Vec3 Tube::getLinkZ(int linkIndex) { return linkZ[linkIndex]; }
Vec3 Tube::getLinkPosition(int linkIndex) { return linkP[linkIndex]; }
float* Tube::getLinkMatrix(int linkIndex) { return linkM[linkIndex]; }


Vec3 Tube::spline(float s) {
    float t = 1-s;
    Vec3 v01 = p1*t+p0*s;
    Vec3 v12 = p0*t+p3*s;
    Vec3 v23 = p3*t+p2*s;
    Vec3 v012 = v01*t+v12*s;
    Vec3 v123 = v12*t+v23*s;
    return v012*t+v123*s;
}

void Tube::update(Vec3 pointA, Vec3 dirA, Vec3 upA, Vec3 pointB, Vec3 dirB, Vec3 upB, bool updateTwisting) {
    this->p1 = pointA;
    this->p2 = pointB;
    this->z1 = dirA;
    this->z2 = dirB;
    this->y1 = upA;
    this->y2 = upB;
  
    float curlen = 0.0;
    Vec3 pA, pB;
  
    if ((p2-p1).getLength()>length) {
        p2 = p1+(p2-p1).normalize()*length*0.999999;
    }
  
    adjuster = 10.0;
  
    int _m = 100;
    while (pow(curlen-length,2)>length*0.0001) {
        if (adjuster==0) break;

        // Recalculate tangent points with new _m value.
        this->p0 = (z1.normalize()* 1*stiffness1*length*adjuster)+p1;
            this->p3 = (z2.normalize()*-1*stiffness2*length*adjuster)+p2;
                
                pB = p1; // First point in spline spline.
                curlen = 0; // Reset calculated length.
                
                // Loop through all the points:
                for (int c=1; c<samples; c++) { // Run through all points but the first.
                    
// Calculate point on spine:
 pA = spline(c*1.0/(samples-1.0));
     
     // The length of the current segment is calculated and added to the whole length:
     curlen += (pA-pB).getLength();
         
         pB = pA;
             }
                                                  
                                                  // Compare calculated length with desired length and adjust adjuster.
                                                  adjuster = adjuster*length/curlen;
    
                                                  _m--;
                                                  if (_m<0) break;
                                                                }
  
    // Calculate length from the start point to each sample point:
    sample[0] = 0;
        pB = p1;
            for (int c=1; c<samples; c++) {
 pA = spline(c*1.0/(samples-1.0));
     sample[c] = sample[c-1]+((pB-pA).getLength());
         pB = pA;
             }
            // Convert sample values to T values between 0 and 1:
            for (int c=0; c<samples; c++) {
 sample[c] = sample[c]/sample[samples-1];
     }
  
            // If start point radius and end point radius is equal,
            // distribute points evenly:
            if (radius1==radius2) {
 for (int c=0; c<links; c++) { // Run through all points.
     linkR[c] = radius1; // All radii equals to the first radii.
     linkT[c] = c*1.0/(links-1); // Lineary distributed T values.
 }  
                                 } else { // Distribute T values exponentialy increasingly:
                                     // One sphere's radius relative to the previous sphere's radius:
                                     float growth = pow(radius2*1.0/radius1,1.0/(links-1.0));
                                         for (int c=0; c<links-1; c++) { // Run through all points but the last.
                                             linkR[c] = radius1*1.0*pow(growth*1.0,c*1.0); // Radius for current point.
                                             // Calculate T value based on radius:
                                             linkT[c] = (linkR[c]+0.0-radius1)/(radius2+0.0-radius1);
                                                 }
                                                                           }
            linkR[links-1] = radius2; // Radius for last point.
            linkT[links-1] = 1; // T value for last point is always 1.
  
            int _c = 0; // Counter for points.
            int _d = 0; // Counter for ranges (see below).
            float desired;
            
            // Calculate points
            while (_c<links) { // Run through all points.
                // The T value that must be found for the current point.
                desired = linkT[_c];
                    // If the right T value is in the range between the two sample T values...
                    if (desired>=sample[_d] && desired<=sample[_d+1]) {
// ...estimate best S value inside range
// then calculate spline point from that value...
 linkP[_c] = spline(
                    (
                     _d
                     +((desired-sample[_d])/(sample[_d+1]-sample[_d]))
                     )/(samples-1)
                    );
     
     _c++; // ...and go to next point.
                    } else {
                        _d++; // ...else, go to next range.
                    }
                                                                          
                                                                          }
            
            // Calculate transformations
            
            linkZ[0] = z1.normalize();
                linkY[0] = (linkZ[0].getCross(y1.getCross(linkZ[0]))).normalize();
                    linkZ[links-1] = z2.normalize();
                    
                    for (int c=1; c<links; c++) {
// Calculate z vector for current point by
// looking at previous point and next point:
 if (c<links-1)
                   linkZ[c] = (linkP[c+1]-linkP[c-1]).normalize();
                   
                   // Calculate current y vector, but without the twisting:
                   if ((linkZ[c]-linkZ[c-1]).getLength()>0) {
 linkY[c] = (linkY[c-1].normalize()).rotateAround(
                                                  linkZ[c-1].getCross(linkZ[c]),
                                                  acos(linkZ[c].getDot(linkZ[c-1]))
                                                  );
     } else {
 linkY[c] = linkY[c-1];
     }
                                                                }
  
                    if (updateTwisting) {
                        // Calculate angle the neck must be twisted.
                        Vec3 y2_corrected = ( linkZ[links-1].getCross( y2.getCross( linkZ[links-1] ) ) ).normalize(); // corrected y2
                        
                        twistAngle = acos(
                                          max(-1.0f,min(1.0f, y2_corrected.getDot( linkY[links-1].normalize() ) ))
                                          );
    
                        // Checks if the neck should be twisted clockwise or anticlockwise:
                        Vec3 clockwise = linkY[links-1].rotateAround(linkZ[links-1],+twistAngle);
                            Vec3 anticwise = linkY[links-1].rotateAround(linkZ[links-1],-twistAngle);
                                if ( (clockwise-y2).getLength()>(anticwise-y2).getLength() ) twistAngle = -twistAngle;
                                if (Abs(twistAngle)>PI*0.5) twistAngle = sin(twistAngle)*PI*0.5;
                    }
                    
                    for (int c=0; c<links; c++) { // Run through all points.
                        
// Corrects y vector with apropriate twisting:
 linkY[c] = linkY[c].rotateAround( linkZ[c], twistAngle*(c/(links-1.0)) );
     
     // Calculates x vector:
     linkX[c] = (linkY[c].getCross(linkZ[c])).normalize();
         
         // Transformation for each link:
         float matrix[] = {
             linkX[c].x,linkX[c].y,linkX[c].z,0,
             linkY[c].x,linkY[c].y,linkY[c].z,0,
             linkZ[c].x,linkZ[c].y,linkZ[c].z,0,
             linkP[c].x,linkP[c].y,linkP[c].z,1
         };
             for (int i=0; i<16; i++) {
 linkM[c][i] = matrix[i];
     }
                                          }
  
}

void Tube::draw() {
    // Draw fins
    glColor3f( 0.2, 0.8, 0.2 );
    for (int c=2; c<links; c+=3) {
        glPushMatrix();
        glMultMatrixf(linkM[c]);
        glScalef( linkR[c], linkR[c], linkR[c] );
        glTranslatef( 0, 1, 0 );
        glRotatef( -40, 1, 0, 0 );
        glScalef( 0.05, 0.75, 0.35 );
        OpenGLUtil::GLSolidSphere( 1.0, 8, 4 );
        glPopMatrix();
    }
  
    // Draw tube
    //  glColor3f( 0.2, 0.55, 0.2 );
    glBegin( GL_QUADS );
    for (int c=0; c<links-1; c++) {
        Vec3 o1 = linkP[c  ];
        Vec3 o2 = linkP[c+1];
        float r1 = linkR[c  ];
        float r2 = linkR[c+1];
    
        for (int r=0; r<8; r++) {
            float rot1 =  r   /8.0;
            float rot2 = (r+1)/8.0;
            Vec3 n11 = linkX[c  ]*sin(rot1*2*PI)+linkY[c  ]*cos(rot1*2*PI);
            Vec3 n12 = linkX[c  ]*sin(rot2*2*PI)+linkY[c  ]*cos(rot2*2*PI);
            Vec3 n21 = linkX[c+1]*sin(rot1*2*PI)+linkY[c+1]*cos(rot1*2*PI);
            Vec3 n22 = linkX[c+1]*sin(rot2*2*PI)+linkY[c+1]*cos(rot2*2*PI);

            glTexCoord2f((r  )/8.0, (c  )/(links-1.0));
            glNormal3f( n11.x,         n11.y,         n11.z         );
            glVertex3f( n11.x*r1+o1.x, n11.y*r1+o1.y, n11.z*r1+o1.z );
            glTexCoord2f((r+1)/8.0, (c  )/(links-1.0));
            glNormal3f( n12.x,         n12.y,         n12.z         );
            glVertex3f( n12.x*r1+o1.x, n12.y*r1+o1.y, n12.z*r1+o1.z );
            glTexCoord2f((r+1)/8.0, (c+1)/(links-1.0));
            glNormal3f( n22.x,         n22.y,         n22.z         );
            glVertex3f( n22.x*r2+o2.x, n22.y*r2+o2.y, n22.z*r2+o2.z );
            glTexCoord2f((r  )/8.0, (c+1)/(links-1.0));
            glNormal3f( n21.x,         n21.y,         n21.z         );
            glVertex3f( n21.x*r2+o2.x, n21.y*r2+o2.y, n21.z*r2+o2.z );
        }
    }
    glEnd();
}
