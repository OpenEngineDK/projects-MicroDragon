
//   RandomGenerator.cpp
//
//     31.5.06, pm-n.
//

#include "random.h"

#include <math.h>

RandomGenerator::RandomGenerator()
{
    idum = -1;
    (void)ran1();    // initialize the stream of RandomGenerator deviates.
}

RandomGenerator::RandomGenerator(int start_point)
{
    idum = -1;
    for (int i = 1; i<= start_point; i++)
        (void)ran1();    // initialize the stream of RandomGenerator deviates.
}


// ----------------------------------------------------------------------

float RandomGenerator::ran1()

    /* Returns a uniform random deviate between 0.0 and 1.0  ( exclusive
       of the endpoint values ).
       Call with idum a negative integer to initialize: thereafter, do not
       alter idum between successive deviates in a sequence.
       Ref. Press et al.: Numerical recipes in C ( 2. ed. ), p. 280.
    */

{


    const long   IA = 16807;
    const long   IM = 2147483647;
    const double AM = 1.0/IM; 
    const long   IQ = 127773;
    const long   IR = 2836;
    const long   NTAB = 32;
    const double NDIV = 1+(IM-1)/NTAB;
    const double EPS = 1.2e-7;
    const double RNMX = 1.0-EPS;

    int j;
    long k;
    static long iy=0;
    static long iv[NTAB];
    float temp;


    if ( idum <= 0 || !iy )   //initialize 
        {
            if ( -(idum) < 1 ) idum = 1;  // Prevent idum = 0  
            else idum = -(idum);
            for ( j=NTAB+7; j>=0; j-- )    // Load shufflev table after 8 numbers 
                {
                    k=(idum)/IQ;
                    idum=IA*(idum-k*IQ)-IR*k;
                    if ( idum<0 ) idum += IM;
                    if ( j<NTAB ) iv[j] = idum;
                }
            iy=iv[0];
        }
    k=(idum)/IQ;
    idum=IA*(idum-k*IQ)-IR*k;
    if ( idum<0 ) idum += IM;
    j=(int)(iy/NDIV);
    iy=iv[j];
    iv[j]= idum;
    if ( (temp=AM*iy) > RNMX ) return RNMX;
    else return temp;
}

// ---------------------------------------------------------------------

float RandomGenerator::ran_uniform_0to1()

    //  Returns a uniform RandomGenerator deviate in the interval [0:1].
{
    return  ran1();
}


// ---------------------------------------------------------------------

int   RandomGenerator::ran_uniform_int( int i, int j )

    //  Returns a uniform deviate integer in the closed interval [i,j].
{
    return (int)( i-1+ceil(ran1()*(j-i+1)) );
}


// ---------------------------------------------------------------------

float RandomGenerator::ran_normal( float my, float sigma )

    //  Returns a normal RandomGenerator deviate with mean: my and 
    //  variance: sigma*sigma.
         
{
    float u1, u2, r, pi;

    pi = 4.0 * atan( 1.0 );

    u1 = ran1();
    u2 = ran1();

    r = my + sqrt( -2.0*sigma*sigma*log(u1) ) * cos( 2*pi*u2);

    return r;

}
