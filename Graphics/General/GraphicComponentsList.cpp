//
//  GraphicComponentList.cpp
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 21/10/2014.
//
//

#include "GraphicComponentsList.h"

#include "../../HeyMate/HeyMate/Editors/Editor.h"


//------------------------------------------------------------------------
HEGraphicComponentsList::HEGraphicComponentsList (HEEditor * attachedEditor)
: editorAndListener(attachedEditor)
{
    
}


//------------------------------------------------------------------------
//                          Création des composants
//------------------------------------------------------------------------
//------------------------------------------------------------------------

HEKnob * HEGraphicComponentsList::createKnob (int tag, VSTGUI::CRect rectPos)
{
    // attetion la postion du Knob n'est pas directement gérée ici.
    
    // construction du knob
    HEKnob * newKnob = new HEKnob(rectPos, editorAndListener, tag);
    // ajout du composant à la vue de l'éditeur
    if ((editorAndListener->getFrame())->addView(newKnob))
    {
        // ajout du knob dans la liste
        knobList.push_back(newKnob);
        return newKnob;
    }
    return NULL;
}

//------------------------------------------------------------------------
HEKnob * HEGraphicComponentsList::createKnob (int tag, double posX, double posY)
{
    // création de la frame à la position voulue
    // makeCRect décale la position de posX et posY
    // pour que l'origin de CRect soit au bon endroit
    VSTGUI::CRect rectPos = graphicHelp::makeCRect(posX, posY, KNOB_SIZE, KNOB_SIZE);
    // construction du knob
    return createKnob(tag, rectPos);
}


//------------------------------------------------------------------------
HESlider * HEGraphicComponentsList::createSlider (int tag, VSTGUI::CRect rectPos, bool isInverse)
{
    // attetion la postion du Slider n'est pas directement gérée ici.
    
    // construction du slider
    HESlider * newSlider;
    newSlider = new HESlider(rectPos, editorAndListener, tag, isInverse);
    // ajout du composant à la vue de l'éditeur
    if ((editorAndListener->getFrame())->addView(newSlider))
    {
        // ajout du slider dans la liste
        sliderList.push_back(newSlider);
        return newSlider;
    }
    return NULL;
}

//------------------------------------------------------------------------
HESlider * HEGraphicComponentsList::createSlider (int tag, double posX, double posY, bool isVertical, bool isInverse)
{
    // création de la frame à la position voulue
    // makeCRect décale la position de posX et posY
    // pour que l'origin de CRect soit au bon endroit
    VSTGUI::CRect rectPos ;
    if (isVertical)
        rectPos = graphicHelp::makeCRect(posX, posY, SLIDER_WIDTHNESS, SLIDER_LONG);
    else
        rectPos = graphicHelp::makeCRect(posX, posY, SLIDER_LONG, SLIDER_WIDTHNESS);
    // construction du knob
    return createSlider(tag, rectPos, isInverse);
}


//------------------------------------------------------------------------
HETextLabel * HEGraphicComponentsList::createTextLabel(VSTGUI::CRect rectPos, VSTGUI::UTF8StringPtr txt, int32_t style)
{
    // construction du label
    HETextLabel * newTextLabel;
    newTextLabel = new HETextLabel(rectPos, txt, style);
    // ajout du composant à la vue de l'éditeur
    if ((editorAndListener->getFrame())->addView(newTextLabel))
    {
        // ajout du slider dans la liste
        textLabelList.push_back(newTextLabel);
        return newTextLabel;
    }
    return NULL;
}


//------------------------------------------------------------------------
HETextLabel * HEGraphicComponentsList::createTextLabel(double posX, double posY, VSTGUI::UTF8StringPtr txt, int32_t style)
{
    CRect rectPos = graphicHelp::makeCRect(posX, posY, TEXTLABEL_WIDTH, TEXTLABEL_HEIGHT);
    return createTextLabel(rectPos, txt, style);
}


//------------------------------------------------------------------------
HEOnOffButton * HEGraphicComponentsList::createOnOffButton (int tag, VSTGUI::CRect rectPos, int32_t style)
{
    // construction du label
    HEOnOffButton * newOnOffButton;
    newOnOffButton = new HEOnOffButton(rectPos, editorAndListener, tag, style);
    // ajout du composant à la vue de l'éditeur
    if ((editorAndListener->getFrame())->addView(newOnOffButton))
    {
        // ajout du slider dans la liste
        onOffButtonList.push_back(newOnOffButton);
        return newOnOffButton;
    }
    return NULL;
}

//------------------------------------------------------------------------
HEOnOffButton * HEGraphicComponentsList::createOnOffButton (int tag, double posX, double posY, int32_t style)
{
    CRect rectPos = graphicHelp::makeCRect(posX, posY, ONOFF_WIDTH, ONOFF_HEIGHT);
    return createOnOffButton(tag, rectPos, style);
}



//------------------------------------------------------------------------
HEKickButton * HEGraphicComponentsList::createKickButton (int tag, VSTGUI::CRect rectPos, bool isRound)
{
    // construction du label
    HEKickButton * newKickButton;
    newKickButton = new HEKickButton(rectPos, editorAndListener, tag, isRound);
    // ajout du composant à la vue de l'éditeur
    if ((editorAndListener->getFrame())->addView(newKickButton))
    {
        // ajout du slider dans la liste
        kickButtonList.push_back(newKickButton);
        return newKickButton;
    }
    return NULL;
}

//------------------------------------------------------------------------
HEKickButton * HEGraphicComponentsList::createKickButton (int tag, double posX, double posY, bool isRound)
{
    CRect rectPos = graphicHelp::makeCRect(posX, posY, KICK_WIDTH, KICK_HEIGHT);
    return createKickButton(tag, rectPos, isRound);
}




//------------------------------------------------------------------------
HESwitchVertical * HEGraphicComponentsList::createVerticalSwitch (int tag, VSTGUI::CRect rectPos,int nbrPosition, bool isRound)
{
    // construction du label
    HESwitchVertical * newSwitchVertical;
    newSwitchVertical = new HESwitchVertical(rectPos, editorAndListener, tag, nbrPosition, isRound);
    // ajout du composant à la vue de l'éditeur
    if ((editorAndListener->getFrame())->addView(newSwitchVertical))
    {
        // ajout du slider dans la liste
        switchVerticalList.push_back(newSwitchVertical);
        return newSwitchVertical;
    }
    return NULL;
}

//------------------------------------------------------------------------
HESwitchVertical * HEGraphicComponentsList::createVerticalSwitch (int tag, double posX, double posY, int nbrPosition, bool isRound)
{
    CRect rectPos = graphicHelp::makeCRect(posX, posY, VERTICAL_SWITCH_WIDTH, VERTICAL_SWITCH_HEIGHT_FOR_ONE_POSITION * nbrPosition);
    return createVerticalSwitch(tag, rectPos, nbrPosition, isRound);
}





//------------------------------------------------------------------------
HESwitchHorizontal * HEGraphicComponentsList::createHorizontalSwitch (int tag, VSTGUI::CRect rectPos,int nbrPosition, bool isRound)
{
    // construction du label
    HESwitchHorizontal * newSwitchHorizontal;
    newSwitchHorizontal = new HESwitchHorizontal(rectPos, editorAndListener, tag, nbrPosition, isRound);
    // ajout du composant à la vue de l'éditeur
    if ((editorAndListener->getFrame())->addView(newSwitchHorizontal))
    {
        // ajout du slider dans la liste
        switchHorizontalList.push_back(newSwitchHorizontal);
        return newSwitchHorizontal;
    }
    return NULL;
}

//------------------------------------------------------------------------
HESwitchHorizontal * HEGraphicComponentsList::createHorizontalSwitch (int tag, double posX, double posY, int nbrPosition, bool isRound)
{
    CRect rectPos = graphicHelp::makeCRect(posX, posY, HORIZONTAL_SWITCH_WIDTH_FOR_ONE_POSITION * nbrPosition, HORIZONTAL_SWITCH_HEIGHT);
    return createHorizontalSwitch(tag, rectPos, nbrPosition, isRound);
}







//------------------------------------------------------------------------
//                          Manipulation des composants
//------------------------------------------------------------------------
//------------------------------------------------------------------------
void HEGraphicComponentsList::deleteComponents()
{
    
}
