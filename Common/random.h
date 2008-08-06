
//   random.h
//
//     30.5.06, pm-n.
//

#ifndef _RANDOM_H_
#define _RANDOM_H_

class  RandomGenerator {
public:
   RandomGenerator();
   RandomGenerator(int start_point);

   float ran_uniform_0to1();

   int   ran_uniform_int( int i, int j );

   float ran_normal(float my, float sigma);

private:

   long     idum;
 
   float    ran1();

};

#endif
