#include "Island.h"

//#include "../../setup.h"
//#include "../../Common/gltga.h"
#include "../../Common/OpenGLUtil.h"
#include "../../Common/utilities.h"

#include <Meta/OpenGL.h>
#include <Math/Math.h>
#include <iostream>
#include <Resources/ITextureResource.h>
#include <Resources/ResourceManager.h>
#include <Renderers/OpenGL/TextureLoader.h>
#include <Logging/Logger.h>
#include <Utils/Convert.h>

#include <Geometry/FaceSet.h>
#include "../../Common/FaceListUtil.h"

using OpenEngine::Math::PI;
using OpenEngine::Renderers::OpenGL::TextureLoader;
using OpenEngine::Utils::Convert;
using namespace OpenEngine::Resources;

Island* Island::instance = NULL;

Island* Island::getInstance()
{
    if( instance == NULL )
        instance = new Island();
    return instance;
}

Island::Island() {
    bRender = true;
    enabled = true;
    renderTrees = true;
    enableTexture = true;
    numberOfRenderStates = 5;
    renderState = numberOfRenderStates-1;

    unsigned char g_HeightMap[IMAGE_SIZE*IMAGE_SIZE]; // Holds The Height Map Data
    string filename = DirectoryManager::FindFileInPath("Island/Terrain5.raw");

    scale = 300.0f;
    HEIGHT_RATIO = 0.25f;
    translate = Vec3(-150,-3.75,-150);
    LoadRawFile( filename.c_str(), IMAGE_SIZE * IMAGE_SIZE, g_HeightMap); // Load raw data
    calcHeightArray(g_HeightMap);
    calcNormalArray();
    
    string texturefile = "Island/ground.tga";
    ITextureResourcePtr texture =
      ResourceManager<ITextureResource>::Create(texturefile);

    GeometryNode* heightMap = RenderHeightMap(texture); // Render The Height Map

    this->AddNode(heightMap);


    //initialize trees position
    numberOfTrees = 0;
    treePositions = new Vec3*[15*15];
    for (int xx=-150; xx<150; xx+=10)
        for (int zz=-150; zz<  0; zz+=10) {
            Vec3 pos = Vec3(xx+fmod(sin((double)(xx+1000*zz))*100000,10),0,zz+fmod(cos((double)(xx+1000*zz))*100000,10));
            pos = heightAt(pos);
            if (
                normalAt(Vec3(xx   ,0,zz   )).y>0.8 &&
                normalAt(Vec3(xx+10,0,zz   )).y>0.8 &&
                normalAt(Vec3(xx   ,0,zz+10)).y>0.8 &&
                normalAt(Vec3(xx+10,0,zz+10)).y>0.8 &&
                pos.y>2 &&
                (xx*xx+zz*zz)>50*50
                ) {
                treePositions[numberOfTrees++] = new Vec3( pos.x, pos.y, pos.z );
            }
        }
}

Island::~Island() {
    while(numberOfTrees > 0)
        delete treePositions[--numberOfTrees];
    delete treePositions;
}

void Island::Apply(IRenderingView* rv) {
    // Draw trees
    if (renderTrees)
        for(int i=0; i<numberOfTrees; i++)
            drawTrees( *treePositions[i] );

    VisitSubNodes(*rv);
}

void Island::drawTrees( Vec3 pos ) {
    glColor3f( 0.25, 0.35, 0.10 );
    glPushMatrix();
    glTranslatef( pos.x, pos.y+2, pos.z );
    glScalef(1,1.5,1);
    OpenGLUtil::GLSolidSphere( 1.0, 8, 4 );
    glPopMatrix();

    glColor3f( 0.33, 0.22, 0.11 );
    glPushMatrix();
    glTranslatef( pos.x, pos.y+0.4, pos.z );
    glScalef(1,1.5,1);
    OpenGLUtil::GLSolidCube( 0.6 );
    glPopMatrix();
}

void Island::toggleRenderState(){
    enabled = true;
    switch ( renderState ){
    case 0: //no trees
        renderTrees = false;
        break;
    case 1: //disable texture
        enableTexture = false;
        break;
    case 2: //grid layout
        cout << "setting GL_LINE render" << endl;
        bRender = false;
        break;
    case 3: //all off
        enabled = false;
        break;
    default: //render all, reset variables
        bRender = true;
        enabled = true;
        renderTrees = true;
        enableTexture = true;
        break;
    }
    renderState++;
    renderState %= numberOfRenderStates;
}

/** This Renders The Height Map As Quads */
GeometryNode* Island::RenderHeightMap(ITextureResourcePtr texture) {

    int x, z; //Create Some Variables For Readability 
    Vec3 normal[4];
    Vec3 point[4];
    Vec3 color[4];
    Vec3 tex[4];

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
                tex[i].x = z*(1.0/(float)MAP_SIZE);
                tex[i].y = x*(1.0/(float)MAP_SIZE);
            }

            Vec3 diagonalCross = (point[0]-point[2]).getCross(point[1]-point[3]);
            bool flip = (diagonalCross.x*diagonalCross.z>0);

	    FacePtr face;
	    unsigned int order[] = {0,1,2,2,3,0};
	    for (unsigned int i=0, t=0; i<6; i++, t=i%3) {
	      int j = order[i];
	      if (flip) j = (j+1)%4;

	      if(t==0) face = FacePtr(new Face(Vector<3,float>(1.0),Vector<3,float>(0.0),Vector<3,float>(0,0,1))); //@todo do this more nice

	      face->vert[t] =
		Vector<3,float>(point[j].x,  point[j].y,  point[j].z);
	      face->norm[t] =
		Vector<3,float>(normal[j].x, normal[j].y, normal[j].z);
	      face->colr[t] =
		Vector<4,float>(color[j].x, color[j].y, color[j].z, 0.0);
	      face->texc[t] = Vector<2,float>(tex[j].x, tex[j].y);

	      if (t==2) {
		face->mat->texr = texture;
		faces->Add(face);
		static int co = 0;
		//logger.info << "count: " << co++ << logger.end;
	      }
	    }
        }
    }
    //FaceListUtil::Unitize(faces);
    //FaceListUtil::OverrideSoftNormsWithHardNorm(faces);
    return new GeometryNode(faces);
}

/* Loads The .RAW File And Stores It In pHeightMap */
void Island::LoadRawFile(const char* strName, int nSize, unsigned char *pHeightMap)
{
    int result;
    FILE *pFile = NULL;

    // Open The File In Read / Binary Mode.
    pFile = fopen( strName, "rb" );

    // Check To See If We Found The File And Could Open It
    if ( pFile == NULL )
        {
            // Display Error Message And Stop The Function
            cout << "Error can't Find The Height Map: " << strName << endl;
            return;
        }

    fread( pHeightMap, 1, nSize, pFile );

    // After We Read The Data, It's A Good Idea To Check If Everything Read Fine
    result = ferror( pFile );

    // Check If We Received An Error
    if (result)
        {
            printf("Error failed To Get Data!\n");
        }

    // Close The File.
    fclose(pFile);
}

void Island::calcHeightArray(unsigned char *pHeightMap) {
    for (int i=0; i<MAP_SIZE; i++)
        for (int j=0; j<MAP_SIZE; j++) {
            heightArray[i+j*MAP_SIZE] = pHeightMap[(i*STEP_SIZE+STEP_SIZE/2)+(j*STEP_SIZE+STEP_SIZE/2)*IMAGE_SIZE];
        }
}

void Island::calcNormalArray() {
    for ( int X = 0; X < MAP_SIZE; X++ )
        for ( int Y = 0; Y < MAP_SIZE; Y++ ) {
            normalArray[X+(Y*MAP_SIZE)] = (Point(X+1,Y  )
                                           -Point(X-1,Y  ))
                .getCross(Point(X,  Y+1)
                          -Point(X,  Y-1)).normalize()*-1;
        }
}

/* This Returns The Height From A Height Map Index */
float Island::Height(int X, int Z) {
    int x = max(0,min(MAP_SIZE-1,X)); /* Error Check Our x Value */
    int z = max(0,min(MAP_SIZE-1,Z)); /* Error Check Our y Value */
    return heightArray[x + (z * MAP_SIZE)]/255.0f; /* Index Into Our Height Array And Return The Height */
}

/* This Returns Points On The Surface */
Vec3 Island::Point(int X, int Z) {
    int x = max(0,min(MAP_SIZE-1,X)); /* Error Check Our x Value */
    int z = max(0,min(MAP_SIZE-1,Z)); /* Error Check Our y Value */
    return Vec3(x*1.0/(MAP_SIZE-1),Height(x,z)*HEIGHT_RATIO,z*1.0/(MAP_SIZE-1))*scale+translate;
}

float Island::heightAt(float x, float z) {
    float floatX = (x-translate.x)/scale*(MAP_SIZE-1);
    float floatZ = (z-translate.z)/scale*(MAP_SIZE-1);
    return (
            Height(int(floatX)  ,int(floatZ)  )*(1-fmod(floatX,1.0f))*(1-fmod(floatZ,1.0f))+
            Height(int(floatX)+1,int(floatZ)  )*(  fmod(floatX,1.0f))*(1-fmod(floatZ,1.0f))+
            Height(int(floatX)  ,int(floatZ)+1)*(1-fmod(floatX,1.0f))*(  fmod(floatZ,1.0f))+
            Height(int(floatX)+1,int(floatZ)+1)*(  fmod(floatX,1.0f))*(  fmod(floatZ,1.0f))
            )*HEIGHT_RATIO*scale+translate.y;
}

Vec3 Island::heightAt(Vec3 p) {
    return Vec3(p.x,heightAt(p.x,p.z),p.z);
}

Vec3 Island::normalAt(Vec3 p) {
    float floatX = (p.x-translate.x)/scale*(MAP_SIZE-1);
    float floatZ = (p.z-translate.z)/scale*(MAP_SIZE-1);
    return (
            Normal(int(floatX)  ,int(floatZ)  )*(1-fmod(floatX,1.0f))*(1-fmod(floatZ,1.0f))+
            Normal(int(floatX)+1,int(floatZ)  )*(  fmod(floatX,1.0f))*(1-fmod(floatZ,1.0f))+
            Normal(int(floatX)  ,int(floatZ)+1)*(1-fmod(floatX,1.0f))*(  fmod(floatZ,1.0f))+
            Normal(int(floatX)+1,int(floatZ)+1)*(  fmod(floatX,1.0f))*(  fmod(floatZ,1.0f))
            ).normalize();
}

/* This Returns The Normal From A Normal Map Index */
Vec3 Island::Normal(int X, int Z) {
    int x = max(0,min(MAP_SIZE-1,X)); /* Error Check Our x Value */
    int z = max(0,min(MAP_SIZE-1,Z)); /* Error Check Our y Value */

    return normalArray[x + (z * MAP_SIZE)]; /* Index Into Our Normal Array And Return The Normal */
}

/* Sets The Color Value For A Particular Index, Depending On The Height Index */
Vec3 Island::Color(int X, int Z) {
    float fColor = Height(X,Z);
    float c = cos(pow(fColor,1.0f)*PI)*-0.5+0.5;
    //return Vec3( 0.20+0.30*c, 0.15+0.35*c, 0.10+0.40*c );
    return Vec3( 0.25+0.30*c, 0.20+0.35*c, 0.15+0.40*c ) *1.5;
}
