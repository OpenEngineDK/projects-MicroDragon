#ifndef _FACE_LIST_UTIL_
#define _FACE_LIST_UTIL_

#include <Geometry/FaceSet.h>

using namespace OpenEngine::Geometry;

class FaceListUtil {
 private:
  static float Max(float a, float b);
  static float Abs(float f);
 public:
  static float Unitize(FaceSet* fset);
  static void OverrideSoftNormsWithHardNorm(FaceSet* fset);
};

#endif // _FACE_LIST_UTIL_
