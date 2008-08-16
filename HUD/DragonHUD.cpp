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

    
    int surfWidth = 512;
    int surfHeight = 64;

    float downpadding = 0.01;
    float leftpadding = 0.01;
    float rightpadding = 0.1;

  CairoSurfaceResourcePtr timeRes = 
      CairoSurfaceResourcePtr(new CairoSurfaceResource(CairoSurfaceResource::CreateCairoSurface(surfWidth,surfHeight)));
  CairoSurfaceResourcePtr scoreRes = 
      CairoSurfaceResourcePtr(new CairoSurfaceResource(CairoSurfaceResource::CreateCairoSurface(surfWidth,surfHeight)));


    time = new DragonText(*timeRes, string("0"));
    score = new DragonText(*scoreRes, string("0"));

    int ypos = frameHeight - (downpadding*frameHeight) - surfHeight;
   
    Layer* timeLayer = new Layer(300,ypos);
    Layer* scoreLayer = new Layer(24,ypos);

    timeLayer->texr = timeRes;
    scoreLayer->texr = scoreRes;

    layerNode = new LayerNode(frameWidth,frameHeight);
    layerNode->AddLayer(*timeLayer);
    layerNode->AddLayer(*scoreLayer);
}

DragonHUD::~DragonHUD() {}

void DragonHUD::Handle(RenderingEventArg arg) {
    testval++;
    time->SetString(Convert::ToString(gamestate.GetTimeLeft() ));
    score->SetString(Convert::ToString(gamestate.GetScore() ));
    time->Draw();
    score->Draw();
}

LayerNode* DragonHUD::GetLayerNode() {
    return layerNode;
}



