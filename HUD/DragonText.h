#ifndef _OE_DRAGON_TEXT_SURFACE_H_
#define _OE_DRAGON_TEXT_SURFACE_H_

#include <Resources/CairoSurfaceResource.h>
#include <string>

using namespace std;
using namespace OpenEngine::Resources;

enum Alignment {
    LEFT, RIGHT
};

class DragonText {
private:
    string text;
    CairoSurfaceResource &resource;
    Alignment alignment;
    int textsize;
public: 
    DragonText(CairoSurfaceResource &res,string t);
    void SetString(string t);
    void Draw();
    void SetAlignment(Alignment al);
    void SetTextSize(int size);

};

#endif
