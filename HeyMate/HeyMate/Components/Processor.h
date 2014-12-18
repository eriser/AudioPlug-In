//
//  Processor.h
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 19/09/2014.
//
// Description :
// Processeur principal,
// Gère le processing de voix et le processing des effets.
// possède un lien vers la classe le processing de voix
// et un lien vers la classe de processing d'effet (equaliseur, delay, ...).
//
// La fonction Process de cette classe delègue au autres classe de processing
// la gestion des données et leurs transformation.


#ifndef __HeyMate__Processor__
#define __HeyMate__Processor__

#include "public.sdk/source/vst/vstaudioeffect.h"

#include "../Processing/ParametersDescription.h"
#include "../Processing/Voices/VoiceProcessor.h"
#include "../Processing/Effects/EffectProcessor.h"



class HEController;

using namespace Steinberg;
using namespace Vst;


//------------------------------------------------------------------------
//------------------------------------------------------------------------




/** La partie de processing hérite de AudioEffect,
 classe de base des process VST3.
 hérite de IComposant et IAudioProcessor,
 ce qui le décrit comme étant un coposant VST de type Audio.
 */


class HEProcessor : public Steinberg::Vst::AudioEffect
{
public:
    HEProcessor ();
    
    tresult PLUGIN_API initialize (FUnknown* context);
	tresult PLUGIN_API setBusArrangements (SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts);
    
	tresult PLUGIN_API canProcessSampleSize (int32 symbolicSampleSize);
	tresult PLUGIN_API setActive (TBool state);
	tresult PLUGIN_API process (ProcessData& data);
    
    // passage des valeurs des paramètres
    tresult PLUGIN_API setState (IBStream* state);
    tresult PLUGIN_API getState (IBStream* state);
    
    
    // utilisé par la séparation Effet, Controller.
    static FUID cid;
	static FUnknown* createInstance (void*) { return (IAudioProcessor*)new HEProcessor (); }
    
    // fonction supplémentaire
    ProcessSetup getProcessSetup ();
    
    
    
private:
    // Liens vers les processeurs de voie et d'effet.
    HEVoiceProcessor * voiceProcessor;
    HEEffectProcessor * effectProcessor;
    
};

#endif /* defined(__HeyMate__Processor__) */
