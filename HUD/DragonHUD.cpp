// TimerLayer
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include "DragonHUD.h"

#include <string>
#include <Resources/ResourceManager.h>
#include <Utils/Convert.h>

using namespace OpenEngine::Resources;

DragonHUD::DragonHUD(IFrame& frame, GameState& gamestate, HUD& hud, TextureLoader& texLoader)
    : gamestate(gamestate), hud(hud), texLoader(texLoader) {

    frameWidth = frame.GetWidth();
    frameHeight = frame.GetHeight();

    int textsize = frameHeight*0.08;

    unsigned int surfWidth = 512;
    unsigned int surfHeight = textsize*2;

    // scale texture height and width to nerest power of two
    unsigned int two = 2;
    while (surfHeight > two)
        two = two << 1;
    surfHeight = two;

    textTool = new CairoTextTool();
    textTool->SetFontName("Monaco");
    textTool->SetFontSize(textsize);
    textTool->Shadows(true);
    textTool->SetColor(Vector<4,float>(0.5,0,0,0.9));

    timeTexture = CairoResource::Create(surfWidth,surfHeight);
    texLoader.Load(timeTexture);
    pointTexture = CairoResource::Create(surfWidth,surfHeight);
    texLoader.Load(pointTexture);

    HUD::Surface* timeSurface = hud.CreateSurface(timeTexture);
    timeSurface->SetPosition(HUD::Surface::RIGHT, 
                             HUD::Surface::BOTTOM);
    int offset = 15;
    Vector<2,int> position = timeSurface->GetPosition();
    timeSurface->SetPosition( position[0]-offset,
                               position[1]-offset);


    HUD::Surface* pointSurface = hud.CreateSurface(pointTexture);
    pointSurface->SetPosition(HUD::Surface::LEFT,
                              HUD::Surface::BOTTOM);
    position = pointSurface->GetPosition();
    pointSurface->SetPosition( position[0]+offset,
                               position[1]-offset);

    ITextureResourcePtr logo = 
        ResourceManager<ITextureResource>::Create("logo/small.png");
    logo->Load();
    HUD::Surface* logoSurface = hud.CreateSurface(logo);
    logoSurface->SetPosition(HUD::Surface::RIGHT, HUD::Surface::TOP);
}

DragonHUD::~DragonHUD() {}

void DragonHUD::Handle(ProcessEventArg arg) {
    char timestring[255];
    int timeleft = gamestate.GetTimeLeft();
    int min = timeleft / 60;
    int sec =  timeleft - min*60;
    sprintf(timestring, "%d:%.2d",min,sec);

    std::string time = Convert::ToString(timestring);
    std::string points = Convert::ToString( gamestate.GetScore() );

    textTool->SetAlignment(CairoTextTool::RIGHT);
    textTool->DrawText(time, timeTexture);
    timeTexture->RebindTexture();

    textTool->SetAlignment(CairoTextTool::LEFT);
    textTool->DrawText(points, pointTexture);
    pointTexture->RebindTexture();
}


