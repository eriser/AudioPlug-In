//
//  ProcessorVoice.cpp
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 28/09/2014.
//
//

// include SDK.
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"

#include "Processor.h"
#include "VoiceProcessor.h"

//-------------------------------------------------------------------------------------------------
// Constructeur de la classe de gestion des voies.
HEVoiceProcessor::HEVoiceProcessor(float sampleRate, HEProcessor * settingsMainProcessor)
{
    mainProcessor = settingsMainProcessor;
    
    
    memset (&parametersValues, 0, sizeof (parametersValues));
    
    activeVoices = 0;
    
    parametersValues.paramVoiceSinVolume = 50;
    parametersValues.paramVoiceSquareVolume = 0;
    parametersValues.paramVoiceTriangularVolume = 0;
    
    parametersValues.paramVoiceReleaseTime = 0;
    parametersValues.paramVoiceVelocity = 0;
    
    parametersValues.paramTuningRange = 1;
    
    parametersValues.paramMasterVolume = 0.5;
    parametersValues.paramMasterTuning = 0;
    parametersValues.paramVoiceVelocity = 1.;
    
    parametersValues.paramVoiceSinVolume = 1.;
    parametersValues.paramVoiceSquareVolume = 0.;
    parametersValues.paramVoiceTriangularVolume = 0.;
    parametersValues.paramVoiceReleaseTime = 0.;
    parametersValues.paramVoiceSinusDetune = 0;
    parametersValues.paramVoiceTriangleSlop = 0.5;
    
    for (int i = 0; i < maxVoices; i++)
    {
        voices[i].setParametersStorage (&parametersValues);
        voices[i].setSampleRate (sampleRate);
        voices[i].reset ();
    }
    
    setActive(true);
    
}


//-------------------------------------------------------------------------------------------------
// Destructeur.

HEVoiceProcessor::~HEVoiceProcessor ()
{
    // ici rien à détruire rien à libérer.
}



//-------------------------------------------------------------------------------------------------
// renvoie la voie correspondant à la noteId

HEVoice* HEVoiceProcessor::getVoice (Steinberg::int32 noteId)
{
    HEVoice* firstFreeVoice = 0; // création d'une voie.
    if (noteId != -1)
    {
        for (int i = 0; i < maxVoices; i++) // on cherche la voie parmi toute celle qui existe.
        {
            if (voices[i].getNoteId () == noteId)
            {
                return &voices[i];
            }
            else if (firstFreeVoice == 0 && voices[i].getNoteId () == -1) // renvoie de la première voie libre si erreur sur la note...
            {
                firstFreeVoice = &voices[i];
            }
        }
    }
    return firstFreeVoice;
}


//----------------------------------------------------------------------------------------------------
// trouve la voie correspondant à la noteId

HEVoice* HEVoiceProcessor::findVoice (Steinberg::int32 noteId)
{
    if (noteId != -1)
    {
        for (int i = 0; i < maxVoices; i++)
        {
            if (voices[i].getNoteId () == noteId)
            {
                return &voices[i];
            }
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------
Steinberg::tresult HEVoiceProcessor::setActive (Steinberg::TBool state)
{
    // faire le chose en plus de chose faite par le processor principal.
    // libération de mémoire pour les voices.
    return Steinberg::kResultTrue;
}

//-----------------------------------------------------------------------------
// passage des valeurs des paramètres
Steinberg::tresult HEVoiceProcessor::setState (Steinberg::IBStream* state)
{
    return parametersValues.setState(state);
}


Steinberg::tresult HEVoiceProcessor::getState (Steinberg::IBStream* state)
{
    return parametersValues.getState(state);
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Steinberg::tresult HEVoiceProcessor::process (Steinberg::Vst::ProcessData& data)
{
    // gestion des changements de paramètres
    // -------------------------------------
    if (data.inputParameterChanges){
        updateParametersValues(data);
    }
    
    Steinberg::tresult result = Steinberg::kResultFalse;
    
    Steinberg::Vst::SpeakerArrangement arr;
    mainProcessor->getBusArrangement(Steinberg::Vst::kOutput, 0, arr);
    Steinberg::int32 numChannels = Steinberg::Vst::SpeakerArr::getChannelCount (arr);
    
    // Lancement du processing.
    // ------------------------
    for (int i = 0; i < numChannels; i++)
        memset (data.outputs[0].channelBuffers32[i], 0, data.numSamples * sizeof (samplePrecision));
    // buffering des data de processing.
    
    Steinberg::Vst::IEventList* inputEvents = data.inputEvents; // récupère l'evènement arrivant.
    
    if (inputEvents)
    {
        Steinberg::Vst::Event e;
        Steinberg::int32 numEvents = inputEvents->getEventCount (); // gestion de tout les évènements qui arrivent
        
        for (int i = 0; i < numEvents; i++)
        {
            if (inputEvents->getEvent (i, e) == Steinberg::kResultTrue)
            {
                // Deux types d'évènement : noteOn / noteOff
                switch (e.type)
                {
                        //-----------------------
                    case  Steinberg::Vst::Event::kNoteOnEvent:
                    {
                        if (e.noteOn.noteId == -1) // for host which don't send unique noteId's
                            e.noteOn.noteId = e.noteOn.pitch;
                        HEVoice* voice = getVoice (e.noteOn.noteId);
                        if (voice)
                        {
                            // Toutes les informations de event
                            // sont envoyé à la classe de génération de la voie.
                            // On dispose donc de ces information sur l'évènement midi.
                            voice->noteOn (e.noteOn.pitch, e.noteOn.velocity, e.noteOn.tuning, e.sampleOffset, e.noteOn.noteId);
                            this->activeVoices++;
                        }
                        break;
                    }
                        //-----------------------
                    case  Steinberg::Vst::Event::kNoteOffEvent:
                    {
                        if (e.noteOff.noteId == -1) // for host which don't send unique noteId's
                            e.noteOff.noteId = e.noteOff.pitch;
                        HEVoice* voice = findVoice (e.noteOff.noteId);
                        if (voice)
                        {
                            voice->noteOff (e.noteOff.velocity, e.sampleOffset);
                        }
#if DEBUG_LOG
                        else
                        {
                            FDebugPrint ("Voice for kNoteOffEvent not found : %d\n", e.noteOff.noteId);
                        }
#endif
                        break;
                    }
                        
                } // fin switch noteOn/noteOff
            } // fin if getevent
        } // fin for boucle pour tout les events
    } // fin if input event
    
    for (int32 i = 0; i < maxVoices; i++)
    {
        if (voices[i].getNoteId () != -1)
        {
            if (!voices[i].process ((SamplePrecision**)data.outputs[0].channelBuffers32, data.numSamples))
            {
                voices[i].reset ();
                this->activeVoices--;
            }
        }
    }
    
    return result;
}






void HEVoiceProcessor::updateParametersValues (Steinberg::Vst::ProcessData& data){
    
    // On prend le nombre de paramètres
    Steinberg::int32 count = data.inputParameterChanges->getParameterCount ();
    for (int i = 0; i < count; i++)
    {
        // On prend les valeurs de chaque paramètre (queue de paramètres)
        Steinberg::Vst::IParamValueQueue* queue = data.inputParameterChanges->getParameterData (i);
        // si celui ci est bien présent.
        if (queue)
        {
            // on récupère la valeur du paramètre et son ID
            Steinberg::int32 sampleOffset;
            double value;
            Steinberg::Vst::ParamID pid = queue->getParameterId ();
            // Si cette valeur à changé, on remplace par la nouvelle valeur.
            // getPoint : /** Gets the value and offset at a given index. */
            if (queue->getPoint (queue->getPointCount () - 1, sampleOffset, value) == Steinberg::kResultTrue)
            {
                switch (pid)
                {
                    case kParamMasterVolume:
                    {
                        parametersValues.paramMasterVolume = value;
                        break;
                    }
                    case kParamMasterTuning:
                    {
                        parametersValues.paramMasterTuning = 2 * (value - 0.5);
                        break;
                    }
                    case kParamVoiceVelocity:
                    {
                        parametersValues.paramVoiceVelocity = value;
                        break;
                    }
                    case kParamVoiceSinVolume:
                    {
                        parametersValues.paramVoiceSinVolume = value;
                        break;
                    }
                    case kParamVoiceTriangularVolume:
                    {
                        parametersValues.paramVoiceTriangularVolume = value;
                        break;
                    }
                    case kParamVoiceSquareVolume:
                    {
                        parametersValues.paramVoiceSquareVolume = value;
                        break;
                    }
                    case kParamVoiceRelease:
                    {
                        parametersValues.paramVoiceReleaseTime = value;
                        break;
                    }
                    case kParamVoiceSinusDetune:
                    {
                        parametersValues.paramVoiceSinusDetune = 2 * (value - 0.5);
                        break;
                    }
                    case kParamVoiceTriangleSlop:
                    {
                        parametersValues.paramVoiceTriangleSlop = value;
                        break;
                    }
                    case kParamTuningRange:
                    {
                        parametersValues.paramTuningRange = std::min<int8> ((int8)(NUM_TUNING_RANGE * value), NUM_TUNING_RANGE - 1);
                        break;
                    }
                        
                        
                        
                    case kKeystroke:
                    {
                        addEventFromUI(data, value);
                    }
                        
                        
                } // fin switch pid
            } // fin if changement de valeur
        } // fin paramètre présent
    } // fin de la boucle for pour tout les paramètres qui on changé.
    
}




void HEVoiceProcessor::addEventFromUI (Steinberg::Vst::ProcessData& data, double value)
{
    Steinberg::Vst::Event eNote = {};
    if (value != -1)
    {
        eNote.type = Steinberg::Vst::Event::kNoteOnEvent;
        eNote.noteOn.pitch = value; // numéro de la noteMidi, peux allé jusqu'à 127.
        eNote.noteOn.velocity = 1.f;  // + rapide [0;+1], voir à modifier selon le mouvement de la souris.
        eNote.noteOn.tuning = 0.f;  // center value. [-1;+1]
    }
    else
    {
        eNote.type = Steinberg::Vst::Event::kNoteOffEvent;
        eNote.noteOff.pitch = value; //< range [0, 127] = [C-2, G8] with A3=440Hz
        eNote.noteOff.velocity = 1; // range [0.0, 1.0]
        eNote.noteOff.tuning = 0; //< 1.f = +1 cent, -1.f = -1 cent
    }
    
    
    data.inputEvents->addEvent(eNote);
    
    
}


