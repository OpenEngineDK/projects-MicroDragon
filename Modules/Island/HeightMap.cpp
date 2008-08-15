#include "HeightMap.h"

#include "../../Common/FaceListUtil.h"

#include <Core/Exceptions.h>
#include <Geometry/FaceSet.h>
#include <Renderers/IRenderingView.h>
#include <Resources/ITextureResource.h>
#include <Scene/GeometryNode.h>

using OpenEngine::Core::Exception;
using OpenEngine::Math::PI;
using std::min;
using std::max;

HeightMap::HeightMap(ITextureResourcePtr heightMap,
		     ITextureResourcePtr texture,
		     float scale, float heightRatio, int stepSize) {

    this->scale = scale;
    HEIGHT_RATIO = heightRatio;
    IMAGE_SIZE = heightMap->GetWidth();
    // @todo: GetHeight()
    STEP_SIZE = stepSize;
    MAP_SIZE = IMAGE_SIZE/STEP_SIZE;

    width = texture->GetWidth() / STEP_SIZE;
    height = texture->GetHeight() / STEP_SIZE;


    heightArray =
      new unsigned char[(IMAGE_SIZE/STEP_SIZE)*(IMAGE_SIZE/STEP_SIZE)];
    normalArray =
      new Vector<3,float>[(IMAGE_SIZE/STEP_SIZE)*(IMAGE_SIZE/STEP_SIZE)];

    // @todo: move translate to island
    translate = Vector<3,float>(-150,-3.75,-150);

    if (heightMap->GetDepth() != 8)
      throw Exception("can only generate heightmap from 8bit textures");

    CalculateHeightArray( heightMap );
    CalculateNormalArray();

    geometry = ConstructGeometry(texture);

    this->AddNode(geometry);
}

void HeightMap::Apply(IRenderingView* rv) {
    VisitSubNodes(*rv);
}

HeightMap::~HeightMap() {
  //this->RemoveNode(geometry); //@todo: could this be made to work?
  delete geometry;

  delete heightArray;
  delete normalArray;
}

/** This Renders The Height Map As Quads */
GeometryNode* HeightMap::ConstructGeometry(ITextureResourcePtr texture) {
    int x, z; //Create Some Variables For Readability 
    Vector<3,float> normal[4];
    Vector<3,float> point[4];
    Vector<3,float> color[4];
    Vector<3,float> tex[4];

    FaceSet* faces = new FaceSet();
    for ( int X = 0; X < MAP_SIZE-1; X++ ) {
        for ( int Z = 0; Z < MAP_SIZE-1; Z++ ) {
            for (int i=0; i<4; i++) {
                if (i==0) { x = X;   z = Z;   }
                if (i==1) { x = X;   z = Z+1; }
                if (i==2) { x = X+1; z = Z+1; }
                if (i==3) { x = X+1; z = Z;   }
                point[i] = Point(x,z);
                normal[i] = Normal(x,z);
                color[i] = Color(x,z);
                tex[i][0] = z*(1.0/(float)MAP_SIZE);
                tex[i][1] = x*(1.0/(float)MAP_SIZE);
            }

            Vector<3,float> diagonalCross = (point[0]-point[2]) % (point[1]-point[3]);
            bool flip = (diagonalCross[0]*diagonalCross[2]>0);

	    FacePtr face;
	    unsigned int order[] = {0,1,2,2,3,0};
	    for (unsigned int i=0, t=0; i<6; i++, t=i%3) {
	      int j = order[i];
	      if (flip) j = (j+1)%4;

	      if(t==0) face = FacePtr(new Face(Vector<3,float>(1.0),Vector<3,float>(0.0),Vector<3,float>(0,0,1))); //@todo do this more nice

	      face->vert[t] =
		Vector<3,float>(point[j][0],  point[j][1],  point[j][2]);
	      face->norm[t] =
		Vector<3,float>(normal[j][0], normal[j][1], normal[j][2]);
	      face->colr[t] =
		Vector<4,float>(color[j][0], color[j][1], color[j][2], 0.0);
	      face->texc[t] = Vector<2,float>(tex[j][0], tex[j][1]);

	      if (t==2) {
		face->mat->texr = texture;
		faces->Add(face);
	      }
	    }
        }
    }
    //FaceListUtil::Unitize(faces);
    //FaceListUtil::OverrideSoftNormsWithHardNorm(faces);
    return new GeometryNode(faces);
}

void HeightMap::CalculateHeightArray(ITextureResourcePtr hMap) {
  int width = hMap->GetWidth();
  int height = hMap->GetHeight();
  unsigned char* data = hMap->GetData();

  for (int i=0; i<MAP_SIZE; i++)
    for (int j=0; j<MAP_SIZE; j++) {
      heightArray[i+j*MAP_SIZE] = 
	data[(i*STEP_SIZE+STEP_SIZE/2)+(j*STEP_SIZE+STEP_SIZE/2)*IMAGE_SIZE];
      //@todo: by rune
    }
}

void HeightMap::CalculateNormalArray() {
    for ( int X = 0; X < MAP_SIZE; X++ )
        for ( int Y = 0; Y < MAP_SIZE; Y++ ) {
            normalArray[X+(Y*MAP_SIZE)] = 
	      (Point(X+1,Y) - Point(X-1,Y))
                % (Point(X,Y+1) - Point(X,Y-1)).GetNormalize()*-1;
        }
}

/* This Returns The Height From A Height Map Index */
float HeightMap::Height(int x, int z) {
    x = max(0,min(MAP_SIZE-1,x)); // Error Check Our x Value
    z = max(0,min(MAP_SIZE-1,z)); // Error Check Our y Value

  //if( !((0 < x < width) && (0 < z < height)) ) return 0.0f;

    return heightArray[x + (z * MAP_SIZE)]/255.0f; /* Index Into Our Height Array And Return The Height */
}

/* This Returns Points On The Surface */
Vector<3,float> HeightMap::Point(int X, int Z) {
    int x = max(0,min(MAP_SIZE-1,X)); /* Error Check Our x Value */
    int z = max(0,min(MAP_SIZE-1,Z)); /* Error Check Our y Value */
    return Vector<3,float>(x*1.0/(MAP_SIZE-1),Height(x,z)*HEIGHT_RATIO,z*1.0/(MAP_SIZE-1))*scale+translate;
}

float HeightMap::HeightAt(float x, float z) {
    float floatX = (x-translate[0])/scale*(MAP_SIZE-1);
    float floatZ = (z-translate[2])/scale*(MAP_SIZE-1);

    float xDir = fmod(floatX,1.0f);
    float zDir = fmod(floatZ,1.0f);

    // takes the height at the four surrounding points 
    // and makes an linear interpolation in both directions
    return (
            Height( ((int)floatX), (int)floatZ )*(1-xDir)*(1-zDir)+

	    Height( ((int)floatX)+1, (int)floatZ )*xDir*(1-zDir)+

	    Height( ((int)floatX), ((int)floatZ)+1)*(1-xDir)*zDir +

	    Height( ((int)floatX)+1,((int)floatZ)+1 )*xDir*zDir

            )*HEIGHT_RATIO*scale+translate[1];
}

Vector<3,float> HeightMap::HeightAt(Vector<3,float> p) {
    return Vector<3,float>(p[0],HeightAt(p[0],p[2]),p[2]);
}

Vector<3,float> HeightMap::NormalAt(Vector<3,float> p) {
    float floatX = (p[0]-translate[0])/scale*(MAP_SIZE-1);
    float floatZ = (p[2]-translate[2])/scale*(MAP_SIZE-1);
    return (
            Normal(int(floatX)  ,int(floatZ)  )*(1-fmod(floatX,1.0f))*(1-fmod(floatZ,1.0f))+
            Normal(int(floatX)+1,int(floatZ)  )*(  fmod(floatX,1.0f))*(1-fmod(floatZ,1.0f))+
            Normal(int(floatX)  ,int(floatZ)+1)*(1-fmod(floatX,1.0f))*(  fmod(floatZ,1.0f))+
            Normal(int(floatX)+1,int(floatZ)+1)*(  fmod(floatX,1.0f))*(  fmod(floatZ,1.0f))
            ).GetNormalize();
}

/* This Returns The Normal From A Normal Map Index */
Vector<3,float> HeightMap::Normal(int X, int Z) {
    int x = max(0,min(MAP_SIZE-1,X)); /* Error Check Our x Value */
    int z = max(0,min(MAP_SIZE-1,Z)); /* Error Check Our y Value */

    return normalArray[x + (z * MAP_SIZE)]; /* Index Into Our Normal Array And Return The Normal */
}

/* Sets The Color Value For A Particular Index, Depending On The Height Index */
Vector<3,float> HeightMap::Color(int X, int Z) {
    float fColor = Height(X,Z);
    float c = cos(pow(fColor,1.0f)*PI)*-0.5+0.5;
    //return Vector<3,float>( 0.20+0.30*c, 0.15+0.35*c, 0.10+0.40*c );
    return Vector<3,float>( 0.25+0.30*c, 0.20+0.35*c, 0.15+0.40*c ) *1.5;
}
