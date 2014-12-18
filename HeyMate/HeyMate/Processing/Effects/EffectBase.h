//
//  EffectBase.h
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 21/10/2014.
//
//

#ifndef __HeyMate__EffectBase__
#define __HeyMate__EffectBase__

#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "../ParametersDescription.h"
#include "../../SupportingFiles/HeyMateIds.h"


class HEProcessor;

class HEEffect
{
public:
    HEEffect(HEProcessor * mainProcessor); // création des effets.
    virtual ~HEEffect(); // destructions des effets.
    
    // set/get the effect Tag
    void setEffectTag (int tag);
    int getEffectTag ();
    
    //-----------------------------------------------------------------------------
    //---------------------MUST BE OVERIDE BY THE EFFECT---------------------------
    //-----------------------------------------------------------------------------
    virtual Steinberg::tresult process (Steinberg::Vst::ProcessData& data) = 0;
    
    virtual Steinberg::tresult setActive (Steinberg::TBool state) = 0;
    
    // passage des valeurs des paramètres
    virtual Steinberg::tresult setState (Steinberg::IBStream* state) {return Steinberg::kResultFalse;};
    virtual Steinberg::tresult getState (Steinberg::IBStream* state) {return Steinberg::kResultFalse;};
    
protected:
    // besoin d'un accès vers le processeur principal.
    HEProcessor * mainProcessor;
    
    // tag de l'effet
    int tag;
    
};



#endif /* defined(__HeyMate__EffectBase__) */
