//
//  EffectProcessor.cpp
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 29/09/2014.
//
//

#include "EffectProcessor.h"

#include "../../Components/Processor.h"

#include "Delay.h"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

HEEffectProcessor::HEEffectProcessor(HEProcessor * settingsMainProcessor)
: effects(0)
{
    // la fonction set Active de l'effet n'est pas appelé 
   // addEffect(new HEDelay(settingsMainProcessor));
    
    mainProcessor = settingsMainProcessor;
    setActive(true);
    
    
    
}

//-----------------------------------------------------------------------------
HEEffectProcessor::~HEEffectProcessor()
{
    // on détruit tout les effets de la table des effets.
    for (std::vector<HEEffect*>::iterator effectIterated = effects.begin();
         effectIterated != effects.end();
         ++effectIterated)
    {
        (*effectIterated)->~HEEffect();
    }
    
}


//-----------------------------------------------------------------------------
// Passage de l'activation aux effets.
Steinberg::tresult HEEffectProcessor::setActive (Steinberg::TBool state)
{
    Steinberg::tresult result = Steinberg::kResultTrue;
    if (state)
    {
        for (std::vector<HEEffect*>::iterator effectIterated = effects.begin();
             effectIterated != effects.end();
             ++effectIterated)
        {
            (*effectIterated)->setActive(state);
        }
        
    }
    else
    {
        
        for (std::vector<HEEffect*>::iterator effectIterated = effects.begin();
             effectIterated != effects.end();
             ++effectIterated)
        {
            HEEffect * effect = *effectIterated;
            delete effect;
            effect = 0;
        }
        
        
    }
    
    return result;
}


//-----------------------------------------------------------------------------
// passage des valeurs des paramètres
Steinberg::tresult HEEffectProcessor::setState (Steinberg::IBStream* state)
{
    Steinberg::tresult res = kResultOk;
    for (std::vector<HEEffect*>::iterator effectIterated = effects.begin();
         effectIterated != effects.end();
         ++effectIterated)
    {
        res &= (*effectIterated)->setState(state);
    }
    return res;
}


Steinberg::tresult HEEffectProcessor::getState (Steinberg::IBStream* state)
{
    Steinberg::tresult res = kResultOk;
    for (std::vector<HEEffect*>::iterator effectIterated = effects.begin();
         effectIterated != effects.end();
         ++effectIterated)
    {
        res &= (*effectIterated)->getState(state);
    }
    return res;
}





//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// process est chargé de redistribuer la fonction process aux effets suivant les effets activés.
Steinberg::tresult HEEffectProcessor::process (Steinberg::Vst::ProcessData& data)
{
    Steinberg::tresult result = Steinberg::kResultFalse;
    
    for (std::vector<HEEffect*>::iterator effectIterated = effects.begin();
         effectIterated != effects.end();
         ++effectIterated)
    {
        result = (*effectIterated)->process(data);
    }
    
    return result;
}




//-----------------------------------------------------------------------------
// ajout d'un effet à la liste des effets
// --------------------------------------
Steinberg::tresult HEEffectProcessor::addEffect (HEEffect * newEffect)
{
    // on applique à l'effet un tag pour qu'il puisse être retrouvé à la suppression..
    newEffect->setEffectTag((int)(effects.size() + 1));
    // allocation d'un nouvel élément dans le vecteur
    effects.push_back(newEffect);
    return kResultTrue;
}
//-----------------------------------------------------------------------------
// suppression d'un effet
// ----------------------
Steinberg::tresult HEEffectProcessor::deleteEffect (int effectTag)
{
    std::vector<HEEffect*>::iterator effectIterated = effects.begin();
    
    
    while ((*effectIterated)->getEffectTag() != effectTag && effectIterated != effects.end())
    {
        ++effectIterated;
    }
    
    if (&effectIterated)
    {
        (*effectIterated)->~HEEffect(); // on détruit l'objet
        effects.erase(effectIterated); // on oublie la référence.
    }
    
    
    return kResultTrue;
}

//-----------------------------------------------------------------------------
// suppression de tout les effets
// ------------------------------
Steinberg::tresult HEEffectProcessor::deleteAllEffect (int effectTag)
{
    // on détruit tout les effets de la table des effets.
    for (std::vector<HEEffect*>::iterator effectIterated = effects.begin();
         effectIterated != effects.end();
         ++effectIterated)
    {
        (*effectIterated)->~HEEffect(); // on détruit l'objet
        effects.erase(effectIterated); // on oublie la référence.
    }
    
    return kResultTrue;
}
