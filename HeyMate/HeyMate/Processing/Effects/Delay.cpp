//
//  DelayProcessor.cpp
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 30/09/2014.
//
//

#include "Delay.h"
#include "../../Components/Processor.h"
#include <stdio.h>
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "public.sdk/source/vst/vstbus.h"

//-----------------------------------------------------------------------------

HEDelay::HEDelay(HEProcessor * settingsMainProcessor)
: HEEffect(settingsMainProcessor)
, buffer (0),
bufferPos (0)
{
    
}

//-----------------------------------------------------------------------------
HEDelay::~HEDelay()
{
    
}




//-----------------------------------------------------------------------------
// Passage de l'activation aux effets.
Steinberg::tresult HEDelay::setActive (Steinberg::TBool state)
{
    Steinberg::tresult result = Steinberg::kResultTrue;
    
    // on récupère l'arrangement des speakers.
    Steinberg::Vst::SpeakerArrangement arr;
    if (mainProcessor->getBusArrangement (Steinberg::Vst::kOutput, 0, arr) != Steinberg::kResultTrue)
        return Steinberg::kResultFalse;
    // on en compte le nombre de voix
    Steinberg::int32 numChannels = Steinberg::Vst::SpeakerArr::getChannelCount (arr);
    
    if (numChannels == 0)
        return Steinberg::kResultFalse;
    // si l'effet est activé
    if (state)
    {
        buffer = (float**)malloc (numChannels * sizeof (float*)); // libération de l'aspace pour le buffering.
        
        size_t size = (size_t)( ( ( ( mainProcessor->getProcessSetup()).sampleRate) * sizeof (float) + 0.5) * (parametersStorage.paramDelayFeedBack + 1 ) );
        // taille de l'espace necessaire pour la répétition d'un son.
        // on stocke le nombre de répétition à faire + 1, minimum de 1 repétition
        for (int channel = 0; channel < numChannels; channel++)
        {
            buffer[channel] = (float*) malloc (size);	// 1 second delay max, libération d'assez de mémoire pour cela pour chaque channel.
            memset (buffer[channel], 0, size); // initialisation à 0 de cette mémoire.
        }
        bufferPos = 0; // position du buffering à 0.
    }
    else
    {
        if (buffer)
        {
            for (int channel = 0; channel < numChannels; channel++)
            {
                delete (buffer[channel]);
            }
            delete (buffer);
            buffer = 0;
        }
    }
    return result;
}


//-----------------------------------------------------------------------------
// passage des valeurs des paramètres
Steinberg::tresult HEDelay::setState (Steinberg::IBStream* state)
{
    return parametersStorage.setState(state);
}


Steinberg::tresult HEDelay::getState (Steinberg::IBStream* state)
{
    return parametersStorage.getState(state);
}




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// process est chargé de redistribuer la fonction process aux effets suivant les effets activés.
Steinberg::tresult HEDelay::process (Steinberg::Vst::ProcessData& data)
{
    // Mise à jour des paramètres
    // --------------------------
    // On regarde d'abord si les paramètres on changé ou non.
    Steinberg::tresult result = Steinberg::kResultTrue;
    if (data.inputParameterChanges)
    {
        int32 paramChangeCount = data.inputParameterChanges->getParameterCount ();
        for (int32 index = 0; index < paramChangeCount; index++) // boucle pour checker tout les paramètres qui ont changé
        {
            IParamValueQueue* queue = data.inputParameterChanges->getParameterData (index);
            if (queue)
            {
                int32 valueChangeCount = queue->getPointCount ();
                ParamValue value;
                int32 sampleOffset;
                switch (queue->getParameterId()) {
                    case kParamDelayTime:
                        if (queue->getPoint (valueChangeCount-1, sampleOffset, value) == kResultTrue)
                            parametersStorage.paramDelayTime = value;
                        break;
                    case kParamDelayDepth:
                        if (queue->getPoint (valueChangeCount-1, sampleOffset, value) == kResultTrue)
                            parametersStorage.paramDelayDepth = value;
                        break;
                    
                    default:
                        break;
                }
            }
        }
    }
    
    
    // Processing
    // ----------
    if (data.numSamples > 0)
    {
        SpeakerArrangement arr;
        mainProcessor->getBusArrangement (kOutput, 0, arr);
        int32 numChannels = SpeakerArr::getChannelCount (arr);
        
        // on récupère le nombre d'échantillons que l'on doit posséder pour notre delay.
        int32 delayInSamples = (int32)(parametersStorage.paramDelayTime * (mainProcessor->getProcessSetup()).sampleRate);
        if (delayInSamples < 1) {
            delayInSamples = 1; // minimum de 1 sample.
        }
        
        for (int32 channel = 0; channel < numChannels; channel++) // processing pour toutes les chaines.
        {
            // direction des flux.
            // aquisition des datas de processing
            float* inputChannel = data.inputs[0].channelBuffers32[channel];
            float* outputChannel = data.outputs[0].channelBuffers32[channel];
            
            int32 tempBufferPos = bufferPos; // on prend la position du buffer, c'est à cette position que sera le delay
            
            for (int sample = 0; sample < data.numSamples; sample++) // pour tout les samples, faire :
            {
                // création de samples temporaires pour le nombre de répétition.
                
                float tempSample = inputChannel[sample];
                
                // recherche dans le tableau de buffering quelle sample relancer.
                // ce sample est directement redonner à sa sortie
                // On ajoute l'entré pour avoir l'effet de décalage et l'entrée de départ.
                outputChannel[sample] = inputChannel [sample] + buffer[channel][tempBufferPos];
                
                // on remplace l'échantillon avec l'échantillon suivant.
                buffer[channel][tempBufferPos] = tempSample;
                
                // on incrémente la position du buffer.
                tempBufferPos++;
                // on supprime l'ancien échantillon temporaire.
                if (tempBufferPos >= delayInSamples)
                    tempBufferPos = 0;
            }
        }
        
        // on incrémente la position du buffering
        bufferPos += data.numSamples;
        // on décale la position du buffer pour la prochaine entrée.
        while (delayInSamples && bufferPos >= delayInSamples)
            bufferPos -= delayInSamples;
    }	
    return kResultTrue;

    return result;
}



