#include "OscSurface.h"

#include "../../Common/utilities.h"
#include "../Island/HeightMap.h"

#include <Core/Exceptions.h>
#include <Logging/Logger.h>
#include <Meta/OpenGL.h>
#include <Renderers/IRenderingView.h>
#include <Utils/Convert.h>
#include <Utils/Timer.h>

#include <string>

using OpenEngine::Core::Exception;
using OpenEngine::Utils::Convert;

OscSurface::OscSurface(HeightMap* heightMap) : heightMap(heightMap) {
    M = 80;
    N = 80;
    aspect = 1.0;
    c  = 0.2;
    dt = 0.005;

    scale = 120;
    translate = Vector<3,float>(-60,0,-60);

    runningTime = 0;
}

OscSurface::~OscSurface() {
}

void OscSurface::Handle(InitializeEventArg arg) {
    timer.Start();

    float *zInit;

    z_norm = (float *)malloc( M*N*3*sizeof(float) );

    zInit = (float *)malloc( M*N*sizeof(float) );
    for ( int i=0; i<M; i++ )
        for ( int j=0; j<N; j++ ) {
            zInit[i*N+j] = 0;
        }

    taux2 = (dt*c * (M-1)) * (dt*c * (M-1));
    tauy2 = (dt*c/aspect*(N-1)) * (dt*c/aspect*(N-1));
    tauxy2 = 2.0 - 2.0*taux2 - 2.0*tauy2;

    if ( (taux2 > 0.25) || (tauy2 > 0.25) ) {
      throw Exception( string("Unstable rubberPlane,") +
		       " tau2x= " + Convert::ToString(taux2) +
		       " tau2y= " + Convert::ToString(tauy2) );
    }

    zOld = (float *)malloc( M*N*sizeof(float) );
    zCur = (float *)malloc( M*N*sizeof(float) );
    zNew = (float *)malloc( M*N*sizeof(float) );

    for ( int i=0; i<M*N; i++)
        zNew[i] = zCur[i] = zOld[i] = zInit[i];

    t = 0;

    free( zInit );
}

void OscSurface::OnLogicEnter(float timeStep) {
    float nzx, nzy, hx, hy;

    runningTime += timeStep*0.2;

    hx = 1.0/(M-1);
    hy = aspect/(N-1);

    preFrame( runningTime );

    // approximate the normal in all grid points:
    for (int i=0; i<M; i++)
        for (int j=0; j<N; j++) {
            if (i==0 )
                nzx = (zCur[1*N+j] - zCur[0*N+j])/hx;
            else if (i==M-1)
                nzx = (zCur[(M-1)*N+j] - zCur[(M-2)*N+j])/hx;
            else
                nzx = (zCur[(i+1)*N+j] - zCur[(i-1)*N+j])/hx/2.0;

            if (j==0)
                nzy = (zCur[i*N+1] - zCur[i*N+0])/hy;
            else if (j==N-1)
                nzy = (zCur[i*N+N-1] - zCur[i*N+N-2])/hy;
            else
                nzy = (zCur[i*N+j+1] - zCur[i*N+j-1])/hy/2.0;

            z_norm[(i*N+j)*3+0] = -nzx*15.0/120;
            z_norm[(i*N+j)*3+1] = -nzy*15.0/120;
            z_norm[(i*N+j)*3+2] = 1.0;
        }
}

void OscSurface::preFrame( double time ) {
    long    steps;
    float   *zDummy;

    steps = (long)floor(time/dt)-(long)floor(t/dt); // New corrected calculation

    //  Calc. z and zOld for the current time
    for (int st = 0; st<steps; st++) {
        for (int i=1; i<(M-1); i++)
            for (int j=1; j<(N-1); j++) {
                zNew[i*N+j] =
                    taux2 * ( zCur[(i+1)*N+j  ] + zCur[(i-1)*N+j  ] )
                    + tauy2 * ( zCur[ i   *N+j+1] + zCur[ i   *N+j-1] )
                    + tauxy2*zCur[i*N+j]  
                    - zOld[i*N+j];

                // Block waves where there is solid land
                if (heightMap->HeightAt(i*scale/(M-1)+translate[0],j*scale/(N-1)+translate[2])>0)
                    zNew[i*N+j] = 0;
            }

        for (int i=1; i<(M-1); i++)
            for (int j=1; j<(N-1); j++) {
                zCur[i*N+j] *= 0.998;
                zNew[i*N+j] *= 0.998;
            }

        zDummy = zOld;
        zOld   = zCur;
        zCur   = zNew;
        zNew   = zDummy;

        if (rand()%20==0) {
            float my_x = (rand()%100/100.0)*scale+translate[0];
            float my_z = (rand()%100/100.0)*scale+translate[2];
            createRipple(my_x, my_z, 3.0, 3.0);
        }
    }
    t = time;
}

void OscSurface::createRipple(float posX, float posZ, float width, float height) {
    // Create new ripple
    float hx = 1.0/(M-1);  float hy = aspect/(N-1);
    for ( int i=1; i<(M-1); i++ )
        for ( int j=1; j<(N-1); j++ )
            zCur[i*N+j] += gaussPeak(
                                     (posX-translate[0])/scale,
                                     (posZ-translate[2])/scale,
                                     width/scale, width/scale, i*hx, j*hy
                                     )*height/scale;
}

void OscSurface::OnRenderEnter(float timeSte) {
    float hx = 1.0/(M-1);
    float hz = aspect/(N-1);

    // draw the surface:
    glPushMatrix();
    glColor3f( 0.8, 0.25, 0.0 );
    glTranslatef( translate[0], translate[1], translate[2]);
    glScalef(scale,scale,scale);

    glColor4f(0.8f,0.25f,0.0f,0.7f);

    glBegin( GL_TRIANGLES );

    for (int i=0; i<(M-1); i++)
        for (int j=0; j<(N-1); j++) {
            int x, z, xz;
            Vector<3,float> point[4];
            Vector<3,float> normal[4];

            for (int c=0; c<4; c++) {
                if (c==0) { x = i;   z = j;   }
                if (c==1) { x = i+1; z = j;   }
                if (c==2) { x = i+1; z = j+1; }
                if (c==3) { x = i;   z = j+1; }
                xz = x*N+z;
                point[c] = Vector<3,float>(x*hx, zCur[xz]*15.0/120, z*hz);
                normal[c] = Vector<3,float>(z_norm[xz*3+0], 1.0f, z_norm[xz*3+1]);
            }

            Vector<3,float> diagonalCross = (point[0]-point[2]) % (point[1]-point[3]);
            bool flip = (diagonalCross[0]*diagonalCross[2]>0);

            drawTriangleQuad(point,normal,flip);
        }

    glEnd();
}

void OscSurface::OnRenderLeave(float timeSte) {
    glPopMatrix();
}

void OscSurface::Handle(DeinitializeEventArg arg) {
    free( z_norm );
    free( zOld );
    free( zCur );
    free( zNew );
}

void OscSurface::Handle(ProcessEventArg arg) {
    unsigned int dt = timer.GetElapsedTimeAndReset().AsInt();
    float deltaTime = ((float)dt)/1000.0;
    OnLogicEnter(deltaTime/1000.0);
}

void OscSurface::Apply(IRenderingView* rv) {
    OnRenderEnter(0.0);
    VisitSubNodes(*rv);
    OnRenderLeave(0.0);
}
