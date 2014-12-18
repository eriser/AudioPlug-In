//
//  Voice.h
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 28/09/2014.
//
// Description :
// Classe de création des voix.
//
// Losqu'une note est joué,
// le processeur des voix renvoie l'évènement noteOn/noteOff à cette classe
// qui doit prendre en charge la création de la voix.

#ifndef __HeyMate__Voice__
#define __HeyMate__Voice__

#include "ParametersDescription.h"
#include "HeyMate_VersionRuler.h" // processing entre version 32/64 bits
#include <cmath>
#include "../../common/logscale.h"

#include "Controller.h"

#ifndef M_PI
#define M_PI			3.14159265358979323846
#endif
#ifndef M_PI_MUL_2
#define M_PI_MUL_2		6.28318530717958647692
#endif
#define MAX_RELEASE_TIME_SEC	5.0
#define numValues 15

typedef float SamplePrecision;
typedef double ParamValue;


//-----------------------------------------------------------------------------
enum VoiceParameters
{
    kVolumeMod,
    kTuningMod,
    kPanningLeft,
    kPanningRight,
    kSinusVolume,
    kTriangleVolume,
    kTriangleSlope,
    kSinusDetune,
    kReleaseTimeMod,
    kSquareVolume,
    
    kNumParameters
};


class HEVoice
{
public:
    HEVoice();
    HEVoice(HEVoice const& newVoice);
    ~HEVoice();
    
    // renvoie le noteId de cette voie.
    Steinberg::int32 getNoteId () { return noteId ; };
    
    // place les paramètres necessaires à la gestion de la voie.
    void setParametersStorage (HEParametersStorage_Voice * newParametersStorage);
    
    // Sets the sampleRate.
    virtual void setSampleRate (SamplePrecision sampleRate) { this->sampleRate = sampleRate; }
    // Returns the sampleRate.
    double getSampleRate () const { return sampleRate; }
    
    // Appellé lorsque la note est jouée, ou non jouée.
    void noteOn (Steinberg::int32 pitch, double velocity, float tuning, Steinberg::int32 sampleOffset, Steinberg::int32 noteId);
    void noteOff (double velocity, Steinberg::int32 sampleOffset);
    
    
    bool process (SamplePrecision* outputBuffers[2], Steinberg::int32 numSamples);
    void reset ();
    
private:
    HEParametersStorage_Voice * parametersStorage;
    
    double frequency;
    
    // informations envoyé par par Event noteOn/noteOff
    Steinberg::int32 noteId;
    Steinberg::int32 noteOnSampleOffset;
    Steinberg::int32 noteOffSampleOffset;
    Steinberg::int32 tuning;
    Steinberg::int32 pitch;
    
    // utilisation pour la génération de voie.
    double sampleRate; // 44100.0 (qualité audio par defaut)
    double noteOnVelocity;
    double noteOffVelocity;
    ParamValue values[numValues];
    
    Steinberg::uint32 n;
    Steinberg::int32 noisePos;
    Steinberg::int32 noiseStep;
    
    SamplePrecision trianglePhase;
    SamplePrecision sinusPhase;
    ParamValue currentTriangleF;
    ParamValue currentSinusF;
    ParamValue currentVolume;
    ParamValue currentPanningLeft;
    ParamValue currentPanningRight;
    ParamValue currentSinusVolume;
    ParamValue currentSinusDetune;
    ParamValue currentSquareVolume;
    ParamValue currentTriangleVolume;
    ParamValue currentTriangleSlope;
    
    ParamValue levelFromVel;
    ParamValue noteOffVolumeRamp;
    
};



class VoiceStatics
{
public:
    //------------------------------------------------------------------------
    static double normalizedLevel2Gain (float normalized)
    {
        double level;
        if (normalized >= 0.5)
            level = scaleHeadRoom * ::pow (10, (normalized - 0.5f) * 24 / 20.0f);
        else
            level = scaleNorm2GainC1 * ::pow (normalized, scaleNorm2GainC2);
        
        return level;
    }
    
    enum {
        kNumFrequencies = 128
    };
    
    static float freqTab[kNumFrequencies];
    static const float scaleHeadRoom;
    static const float scaleNorm2GainC1;
    static const float scaleNorm2GainC2;
    static Steinberg::Vst::LogScale<Steinberg::Vst::ParamValue> freqLogScale;
    static const double kNormTuningOneOctave;
    static const double kNormTuningOneTune;
    
};


#endif /* defined(__HeyMate__Voice__) */
