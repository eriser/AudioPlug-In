//
//  ProcessorVoice.h
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 28/09/2014.
//
//
// Description :
// Classe de gestion des voix.
// La classe de création des voix est la classe HEVoice. qui peux changer selon le produit...
//
// Cette classe est chargé de mettre à jour les paramètres suivant les donées reçus
// et de passé à la classe de voix les évènement noteOn/noteOff

#ifndef __HeyMate__ProcessorVoice__
#define __HeyMate__ProcessorVoice__

#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivstevents.h"

#include "ParametersDescription.h"
#include "Voice.h"

//#define MAX_VOICES			64
//#define MAX_RELEASE_TIME_SEC	5.0
#define NUM_FILTER_TYPE			3
#define NUM_TUNING_RANGE		2


class HEProcessor;

// Déscription des variables de processing.
// Dans les exemples du SDK, ces variables sont déclarées dans des templates,
// ici on en fait une particularisation avec les classes spécifiques Heimat.
static int maxVoices = MAX_VOICES;


class HEVoiceProcessor
{
public:
    HEVoiceProcessor(float sampleRate, HEProcessor * settingsMainProcessor);
    ~HEVoiceProcessor ();
    
    Steinberg::tresult process (Steinberg::Vst::ProcessData& data);
    void updateParametersValues (Steinberg::Vst::ProcessData& data);
    // lorque la gui change un certain paramètre, cette fonction est chargée d'ajouter un nouvel évènement midi à la liste des évents
    void addEventFromUI (Steinberg::Vst::ProcessData& data, double value);
    
    // passage des valeurs des paramètres
    Steinberg::tresult setState (Steinberg::IBStream* state);
    Steinberg::tresult getState (Steinberg::IBStream* state);
    
    Steinberg::tresult setActive (Steinberg::TBool state);
    
    Steinberg::int32 getActiveVoices () const { return activeVoices; } // renvoie le nombre de voie active.
    
protected:
    HEProcessor * mainProcessor; 
    
    HEParametersStorage_Voice parametersValues;
    
    
    HEVoice * getVoice (Steinberg::int32 noteId);
    HEVoice * findVoice (Steinberg::int32 noteId);
    
    HEVoice voices [256]; // note : il faut à la fois changer la variable static et ici...
    
    Steinberg::int32 activeVoices;
};




#endif /* defined(__HeyMate__ProcessorVoice__) */
