//
//  Parameters.h
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 22/09/2014.
//
//

#ifndef __HeyMate__Parameters__
#define __HeyMate__Parameters__

#include "base/source/fstreamer.h"
#include "brownnoise.h"
#include "ivstevents.h"

// nombre de voie maximale acceptée.
#define MAX_VOICES 128

typedef double ParamValue;


// -----------------------------------------------------------

//              Index et Structures des MidiEventFrom GUI
// -----------------------------------------------------------
// -----------------------------------------------------------

// enum des paramètres des voies
// ---------------------------------
//enum{
//    kKeystroke = 600 // tag des touches
//};
//
//
//struct HEEventFromUI_Strorage
//{
//    // c'est le pitch qui est modifié par l'appel du paramètre du tag kKeystroke
//    Steinberg::int16 sPitch;			///< range [0, 127] = [C-2, G8] with A3=440Hz
//    
//    
//    Steinberg::int16 sChannel;		///< channel index in event bus
//    float sTuning;                   ///< 1.f = +1 cent, -1.f = -1 cent
//    float sVelocity;                 ///< range [0.0, 1.0]
//    Steinberg::int32 sLength;        ///< in sample frames (optional, Note Off has to follow in any case!)
//    Steinberg::int32 sNoteId;		///< note identifier (if not available then -1)
//    
//    Steinberg::tresult setState (Steinberg::IBStream* stream);
//    Steinberg::tresult getState (Steinberg::IBStream* stream);
//    
//};





// -----------------------------------------------------------

//              Index et Structures des Voix
// -----------------------------------------------------------
// -----------------------------------------------------------



// enum des paramètres des voies
// ---------------------------------
enum{
  
    kParamVoiceSinVolume,
    kParamVoiceTriangularVolume,
    kParamVoiceSquareVolume,
    kParamVoiceRelease,
  
    kParamVoiceSinusDetune,
    kParamVoiceTriangleSlop,
    kParamVoiceVelocity,
    
    kParamActiveVoices,
    
    kParamMasterTuning,
    kParamMasterVolume,
    kParamTuningRange,
    
    kParamVoiceNumParameters,
    
    
    kKeystroke = 600 // tag des touches
    
} VoiceParameterIndex ;



// enum des paramètres des effets
// ---------------------------------
// delay
enum{
    kParamDelayTime = kParamActiveVoices+1,
    kParamDelayFeedBack,
    kParamDelayDepth,
    
    kParamDelayNbr
};



// -----------------------------------------------------------

//          Structures des valeurs des paramètres
// -----------------------------------------------------------
// -----------------------------------------------------------


// valeurs des paramètres des voies
// ---------------------------------
struct HEParametersStorage_Voice
{    
    ParamValue paramMasterVolume;	// [0, +1]
    ParamValue paramMasterTuning;	// [-1, +1]
    ParamValue paramVoiceVelocity;	// [0, +1] Velocity  =  velToLevel
    
    ParamValue paramVoiceSinVolume;         // [0, +1]
    ParamValue paramVoiceTriangularVolume;	// [0, +1]
    ParamValue paramVoiceSquareVolume;      // [0, +1]
    
    ParamValue paramVoiceReleaseTime;		// [0, +1]
    
    ParamValue paramVoiceSinusDetune;		// [-1, +1]
    ParamValue paramVoiceTriangleSlop;      // [0, +1]
    
    Steinberg::int8 paramTuningRange;		// [0, 1]
    
    // c'est le pitch qui est modifié par l'appel du paramètre du tag kKeystroke
    Steinberg::int16 sPitch;			///< range [0, 127] = [C-2, G8] with A3=440Hz
    
    
    // Lecture de la valeur des paramètres dans un courant de bits.
    Steinberg::tresult setState (Steinberg::IBStream* stream);
	Steinberg::tresult getState (Steinberg::IBStream* stream);
    
    
};

struct HEParametersStorage_Delay
{
    double paramDelayTime;
    Steinberg::int32 paramDelayFeedBack; // nombre de répétition du delay
    double paramDelayDepth;
    
    
    // Lecture de la valeur des paramètres dans un courant de bits.
    Steinberg::tresult setState (Steinberg::IBStream* stream);
    Steinberg::tresult getState (Steinberg::IBStream* stream);
    
};

#endif /* defined(__HeyMate__Parameters__) */
