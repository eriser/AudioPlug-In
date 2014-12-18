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
    // sin
    kParamVoiceSinVolume = 0,
    // square
    kParamVoiceSquareVolume,
    // triangular
    kParamVoiceTriangularVolume,
    
    // Release
    kParamVoiceRelease,
    // Velocity
    kParamVoiceVelocity,
    
    // MasterVolume.
    kParamMasterVolume,
    
    // tuning Range
    kParamTuningRange,
    
    // représentant le nombre de voie active.
    kParamActiveVoices,
    
    kParamVoiceNbr,
    
    kParamReleaseTime,
    kParamNoiseVolume,
    kParamSinusVolume,
    kParamTriangleVolume,
    kParamSinusDetune,
    kParamBypassSNA,
    kParamTriangleSlop,
    kParamFilterType,
    kParamFilterFreq,
    kParamFilterQ,
    //  kParamMasterVolume,
    kParamMasterTuning,
    kParamVelToLevel,
    kParamFilterFreqModDepth,
    //  kParamTuningRange,
    //  kParamActiveVoices,
    kParamSquareVolume,
    
    
    kNumGlobalParameters,
    
    kKeystroke = 600 // tag des touches
    
} VoiceParameterIndex ;

// valeurs des paramètres des voies
// ---------------------------------
struct HEParametersStorage_Voice
{
    // sin
    double paramVoiceSinVolume;             // [0;100] %
    // square
    double paramVoiceSquareVolume;          // [0;100] %
    // triangular
    double paramVoiceTriangularVolume;      // [0;100] %
    
    // Release
    double paramVoiceReleaseTime;           // [0;10] s
    // Velocity
    double paramVoiceVelocity;              // [0;10] s
    
    // MasterVolume
    double paramMasterVolume;               // [0;100] %
    
    // Tuning Range
    Steinberg::int32 paramTuningRange;                   // [-1;+1]
    
    Steinberg::Vst::NoteExpressionSynth::BrownNoise<float>* noiseBuffer;
    
    ParamValue masterVolume;	// [0, +1]
    ParamValue masterTuning;	// [-1, +1]
    ParamValue velToLevel;		// [0, +1]
    
    ParamValue noiseVolume;		// [0, +1]
    ParamValue sinusVolume;		// [0, +1]
    ParamValue triangleVolume;	// [0, +1]
    ParamValue squareVolume;	// [0, +1]
    
    ParamValue releaseTime;		// [0, +1]
    
    ParamValue sinusDetune;		// [-1, +1]
    ParamValue triangleSlop;	// [0, +1]
    
    ParamValue filterFreq;		// [-1, +1]
    ParamValue filterQ;			// [-1, +1]
    ParamValue freqModDepth;	// [-1, +1]
    
    Steinberg::int8 filterType;			// [0, 1, 2]
    Steinberg::int8 tuningRange;			// [0, 1]
    
    Steinberg::int8 bypassSNA;				// [0, 1]
    
    
    // c'est le pitch qui est modifié par l'appel du paramètre du tag kKeystroke
    Steinberg::int16 sPitch;			///< range [0, 127] = [C-2, G8] with A3=440Hz
    
    
    // Lecture de la valeur des paramètres dans un courant de bits.
    Steinberg::tresult setState (Steinberg::IBStream* stream);
	Steinberg::tresult getState (Steinberg::IBStream* stream);
    
    
};



// -----------------------------------------------------------

//              Index et Structures du delay
// -----------------------------------------------------------
// -----------------------------------------------------------





// enum des paramètres des effets
// ------------------------------
// delay
enum{
    kParamDelayTime = kParamVoiceNbr+1,
    kParamDelayFeedBack,
    kParamDelayDepth,
    
    kParamDelayNbr
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
