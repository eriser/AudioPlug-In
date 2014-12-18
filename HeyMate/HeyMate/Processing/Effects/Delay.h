//
//  DelayProcessor.h
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 30/09/2014.
//
//

#ifndef __HeyMate__DelayProcessor__
#define __HeyMate__DelayProcessor__


#include "EffectBase.h"

class HEProcessor;

class HEDelay : public HEEffect
{
public:
    HEDelay(HEProcessor * mainProcessor); // création des effets.
    ~HEDelay(); // destructions des effets.
    
    Steinberg::tresult process (Steinberg::Vst::ProcessData& data);
    
    Steinberg::tresult setActive (Steinberg::TBool state);
    
    // passage des valeurs des paramètres
    Steinberg::tresult setState (Steinberg::IBStream* state);
    Steinberg::tresult getState (Steinberg::IBStream* state);
    
private:
    // stockage des paramètres du delay
    HEParametersStorage_Delay parametersStorage;
    
    // buffering des datas.
    float** buffer;
    Steinberg::int32 bufferPos; // position de la répétition du son.
    
};

#endif /* defined(__HeyMate__DelayProcessor__) */
