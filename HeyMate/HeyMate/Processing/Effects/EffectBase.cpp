//
//  EffectBase.cpp
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 21/10/2014.
//
//

#include "EffectBase.h"



//-----------------------------------------------------------------------------
HEEffect::HEEffect(HEProcessor * settingsMainProcessor)
{
    mainProcessor = settingsMainProcessor;
}

//-----------------------------------------------------------------------------
HEEffect::~HEEffect()
{
    
}



// set/get the effect Tag
// ----------------------
void HEEffect::setEffectTag (int effectTag)
{
    tag = effectTag;
}

int HEEffect::getEffectTag ()
{
    return tag;
}



