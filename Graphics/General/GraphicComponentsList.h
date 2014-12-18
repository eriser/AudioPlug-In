//
//  GraphicComponentList.h
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 21/10/2014.
//
//

#ifndef __HeyMate__GraphicComponentList__
#define __HeyMate__GraphicComponentList__

#include <vector>

#include "../Elements/Knob.h"
#include "../Elements/Slider.h"
#include "../Elements/TextLabel.h"
#include "../Elements/OnOffButton.h"
#include "../Elements/KickButton.h"
#include "../Elements/SwitchVertical.h"
#include "../Elements/SwitchHorizontal.h"

#include "GraphicHelp.h"


class HEEditor;

class HEGraphicComponentsList
{
public:
    HEGraphicComponentsList (HEEditor * attachedEditor);
    
    // listes des composants
    // -----------------------------
    std::vector<HEKnob*> knobList;
    std::vector<HESlider*> sliderList;
    std::vector<HETextLabel*> textLabelList;
    std::vector<HEOnOffButton*> onOffButtonList;
    std::vector<HEKickButton*> kickButtonList;
    std::vector<HESwitchVertical*> switchVerticalList;
    std::vector<HESwitchHorizontal*> switchHorizontalList;
    
    // création des composants
    // -----------------------------
    // knobs
    HEKnob * createKnob (int tag, VSTGUI::CRect rect);
    HEKnob * createKnob (int tag, double posX, double posY); // création avec la taille de knob par defaut
    
    // sliders
    HESlider * createSlider (int tag, VSTGUI::CRect rectPos, bool isInverse);
    HESlider * createSlider (int tag, double posX, double posY, bool isVertical = true, bool isInverse = false); // création avec la taille du slider par defaut, à l'horizontal ou à la vertical
    
    // textLabel
    HETextLabel * createTextLabel (VSTGUI::CRect rectPos, VSTGUI::UTF8StringPtr txt = 0, int32_t style = 0);
    HETextLabel * createTextLabel (double posX, double posY, VSTGUI::UTF8StringPtr txt = 0, int32_t style = 0);
    
    // onOffButton
    HEOnOffButton * createOnOffButton (int tag, VSTGUI::CRect rectPos, int32_t style = 0);
    HEOnOffButton * createOnOffButton (int tag, double posX, double posY, int32_t style = 0);
    
    // KickButton
    HEKickButton * createKickButton (int tag, VSTGUI::CRect rectPos, bool isRound = true);
    HEKickButton * createKickButton (int tag, double posX, double posY, bool isRound = true);
    
    // Vertical Switches
    HESwitchVertical * createVerticalSwitch (int tag, VSTGUI::CRect rectPos, int nbrPosition, bool isRound = true);
    HESwitchVertical * createVerticalSwitch (int tag, double posX, double posY, int nbrPosition, bool isRound = true);
    
    // Horizontal Switches
    HESwitchHorizontal * createHorizontalSwitch (int tag, VSTGUI::CRect rectPos, int nbrPosition, bool isRound = true);
    HESwitchHorizontal * createHorizontalSwitch (int tag, double posX, double posY, int nbrPosition, bool isRound = true);
    
    // manipulation des composants
    void deleteComponents();
    
protected:
    HEEditor * editorAndListener;
    
};


#endif /* defined(__HeyMate__GraphicComponentList__) */
