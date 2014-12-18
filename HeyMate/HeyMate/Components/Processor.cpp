//
//  Processor.cpp
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 19/09/2014.
//
//



#include "Processor.h"
#include "Controller.h"

#include "../SupportingFiles/HeyMateIds.h"
// include SDK.
#include "pluginterfaces/vst/ivstparameterchanges.h"



//-----------------------------------------------------------------------------
//
//                            API's function
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
FUID HEProcessor::cid (0x6EE65CD1, 0xB83A4AF4, 0x80AA7929, 0xAEA6B8A0);

//-----------------------------------------------------------------------------
HEProcessor::HEProcessor() // création du processeur principale
: voiceProcessor(0),
effectProcessor(0) // initialisation des processeurs lors de la fonction setActive.
{
    // On crée le lien processeur controller.
    // grâce au FUID décrit dans le fichier de suport HeyMateId.h
    setControllerClass (HEController::cid);
    
    // assure la création des processeur de voiex et d'effet
    setActive(true);
    
}


//-----------------------------------------------------------------------------
tresult PLUGIN_API HEProcessor::initialize (FUnknown* context)
{
	tresult result = AudioEffect::initialize (context);
	if (result == kResultTrue)
	{
		addAudioOutput (STR16 ("Audio Output"), SpeakerArr::kStereo);
		addEventInput (STR16 ("Event Input"), 1);
	}
	return result;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HEProcessor::setBusArrangements (SpeakerArrangement* inputs, int32 numIns,
                                                  SpeakerArrangement* outputs, int32 numOuts)
{
	// On ne veux pas d'entrée audio => numIns = 0,
    // On veux sortir 1 bus audio (on ne peux pas faire vraiment mieux) => numOut = 1
    // Sortie stéréo.
	if (numIns == 0 && numOuts == 1 && outputs[0] == SpeakerArr::kStereo)
	{
		return AudioEffect::setBusArrangements (inputs, numIns, outputs, numOuts);
	}
	return kResultFalse;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HEProcessor::canProcessSampleSize (int32 symbolicSampleSize)
{
	if (symbolicSampleSize == kSample32 || symbolicSampleSize == kSample64)
	{
		return kResultTrue;
	}
	return kResultFalse;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HEProcessor::setActive (TBool state)
{
    tresult result = Steinberg::kResultTrue;
	if (state)
    {
        if (voiceProcessor == 0)
        {
            voiceProcessor = new HEVoiceProcessor (processSetup.sampleRate, this);
        }
        result = result && voiceProcessor->setActive(state);
        
        if (effectProcessor == 0)
        {
            effectProcessor = new HEEffectProcessor(this);
        }
        result = result && effectProcessor->setActive(state);
    }
    else
    {
        // si set active est négatif, on supprime les processors
        if (voiceProcessor)
        {
            delete voiceProcessor;
        }
        voiceProcessor = 0;
        
        if (effectProcessor)
        {
            delete effectProcessor;
        }
        effectProcessor = 0;
    }
    
	return result && AudioEffect::setActive (state);
}


//-----------------------------------------------------------------------------
tresult PLUGIN_API HEProcessor::setState (IBStream* state)
{
    tresult res = kResultOk;
    if (voiceProcessor)
        res = res && voiceProcessor->setState(state);
    if (effectProcessor)
        res = res && effectProcessor->setState(state);
    return res;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HEProcessor::getState (IBStream* state)
{
    tresult res = kResultOk;
    if (voiceProcessor)
        res = res && voiceProcessor->getState(state);
    if (effectProcessor)
        res = res && effectProcessor->getState(state);
    return res;
}

//-----------------------------------------------------------------------------
//
//                            Processing Part
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

tresult PLUGIN_API HEProcessor::process (ProcessData& data)
{
    tresult result;
    tresult resultVoice = kResultTrue;
    tresult resultEffect = kResultTrue;
    
    
    // Lancement des processing
    // ------------------------
	if (data.numOutputs < 1)
        // pas de sortie assigné, on ne process pas.
		result = kResultTrue;
	else {
        // lancement du process de création des sons.
        resultVoice = voiceProcessor->process (data);
        
        // on place la sortie des voix sur l'entrée des effets
        data.inputs = data.outputs;
        
        // lancement du process des effets.
        resultEffect = effectProcessor->process(data);
    }
    
    result = resultVoice && resultEffect ;
    
    
    // Balance le son (si tout c'est bien passé)
    // -----------------------------------------
    
	if (result == kResultTrue)
	{
		if (data.outputParameterChanges)
		{
			int32 index;
			IParamValueQueue* queue = data.outputParameterChanges->addParameterData (kParamActiveVoices, index);
			if (queue)
			{
				queue->addPoint (0, (ParamValue)voiceProcessor->getActiveVoices () / (ParamValue)MAX_VOICES, index);
			}
		}
		if (voiceProcessor->getActiveVoices () == 0 && data.numOutputs > 0)
		{
			data.outputs[0].silenceFlags = 0x11; // left and right channel are silent
		}
	}
    
	return result;
    
}







//-----------------------------------------------------------------------------
//
//                            Other function
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

ProcessSetup HEProcessor::getProcessSetup()
{
    return processSetup;
}









