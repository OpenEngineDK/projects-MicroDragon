// TimerLayer
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include "DragonHUD.h"

#include <Resources/CairoSurfaceResource.h>
#include <string>
#include <Utils/Convert.h>

#include <Logging/Logger.h>

using namespace OpenEngine::Resources;
using OpenEngine::Utils::Convert;
using std::string;


DragonHUD::DragonHUD(IFrame& frame, GameState& gamestate)
  : testval(0), gamestate(gamestate) {
    frameWidth = frame.GetWidth();
    frameHeight = frame.GetHeight();

    
    float downpadding = 0.02;
    float leftpadding = 0.02;
    float rightpadding = 0.02;

    int textsize = frameHeight*0.08;

    unsigned int surfWidth = 512;
    unsigned int surfHeight = textsize*2;

    // scale texture height and width to nerest power of two
    unsigned int two = 2;
    while (surfHeight > two)
        two = two << 1;
    surfHeight = two;
//     unsigned int widthPowerOfTwo = 0;
//     while (movieWidth > (two<<widthPowerOfTwo))
//         widthPowerOfTwo++;
//     width = (two<<widthPowerOfTwo);



  CairoSurfaceResourcePtr timeRes = 
      CairoSurfaceResourcePtr(new CairoSurfaceResource(CairoSurfaceResource::CreateCairoSurface(surfWidth,surfHeight)));
  CairoSurfaceResourcePtr scoreRes = 
      CairoSurfaceResourcePtr(new CairoSurfaceResource(CairoSurfaceResource::CreateCairoSurface(surfWidth,surfHeight)));


    time = new DragonText(*timeRes, string(""));
    time->SetAlignment(RIGHT);
    time->SetTextSize(textsize);
    score = new DragonText(*scoreRes, string(""));
    score->SetTextSize(textsize);
    
    int ypos = frameHeight - (downpadding*frameHeight) - surfHeight;

    Layer* timeLayer = new Layer(frameWidth-frameWidth*rightpadding-surfWidth,ypos);
    Layer* scoreLayer = new Layer(frameWidth*leftpadding,ypos);

    timeLayer->texr = timeRes;
    scoreLayer->texr = scoreRes;

    layerNode = new LayerNode(frameWidth,frameHeight);
    layerNode->AddLayer(*timeLayer);
    layerNode->AddLayer(*scoreLayer);
}

DragonHUD::~DragonHUD() {}

void DragonHUD::Handle(RenderingEventArg arg) {
    char timestring[255];
    int timeleft = gamestate.GetTimeLeft();
    int min = timeleft / 60;
    int sec =  timeleft - min*60;
    sprintf(timestring, "%d:%.2d",min,sec);
    time->SetString(Convert::ToString(timestring));
    score->SetString(Convert::ToString(gamestate.GetScore() ));
    time->Draw();
    score->Draw();
}

LayerNode* DragonHUD::GetLayerNode() {
    return layerNode;
}



