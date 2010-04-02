#include "FaceListUtil.h"

//#include <Logging/Logger.h>

/** Max: returns the maximum of two floats */
float FaceListUtil::Max(float a, float b) {
    if (b > a)
        return b;
    return a;
}

/** Abs: returns the absolute value of a float */
float FaceListUtil::Abs(float f) {
    if (f < 0)
        return -f;
    return f;
}

/* Unitize: "unitize" a faseset by translating it to the origin and
 * scaling it to fit in a unit cube around the origin.   Returns the
 * scalefactor used.
 */
float FaceListUtil::Unitize(FaceSet* fset) {
    
    if (fset == NULL)
      throw Exception("cannot unitize: FaseSet == NULL");

    FaceList::iterator itr = fset->begin();
    FacePtr f = (*itr);

    // get the max/mins 
    float maxx, minx, maxy, miny, maxz, minz;
    maxx = minx = f->vert[0][0];
    maxy = miny = f->vert[0][1];
    maxz = minz = f->vert[0][2];
    for (; itr != fset->end(); itr++) {
        f = (*itr);
	for(unsigned int i=0; i<3; i++) {
	    Vector<3,float> vert = f->vert[i];
	    if (maxx < vert[0])
	      maxx = vert[0];
	    if (minx > vert[0])
	      minx = vert[0];
	    
	    if (maxy < vert[1])
		maxy = vert[1];
	    if (miny > vert[1])
	        miny = vert[1];

	    if (maxz < vert[2])
		maxz = vert[2];
	    if (minz > vert[2])
	        minz = vert[2];
      }
    }
    
    // calculate model width, height, and depth 
    float w = Abs(maxx) + Abs(minx);
    float h = Abs(maxy) + Abs(miny);
    float d = Abs(maxz) + Abs(minz);
    
    // calculate center of the model 
    float cx = (maxx + minx) / 2.0;
    float cy = (maxy + miny) / 2.0;
    float cz = (maxz + minz) / 2.0;
    
    // calculate unitizing scale factor 
    float scale = 2.0 / Max(Max(w, h), d);
    
    // translate around center then scale
    for (itr = fset->begin(); itr != fset->end(); itr++) {
        f = (*itr);
	for(unsigned int i=0; i<3; i++) {
	    Vector<3,float> vert = f->vert[i];
	    vert[0] -= cx;
	    vert[1] -= cy;
	    vert[2] -= cz;
	    vert[0] *= scale;
	    vert[1] *= scale;
	    vert[2] *= scale;
	}
    }
    return scale;
}

void FaceListUtil::OverrideSoftNormsWithHardNorm(FaceSet* fset) {
    FaceList::iterator itr;
    for (itr = fset->begin(); itr != fset->end(); itr++) {
        FacePtr f = (*itr);
	f->CalcHardNorm();
	f->norm[0] = f->norm[1] = f->norm[2] = f->hardNorm;
    }
}

