//
//  Voice.cpp
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 28/09/2014.
//
//


#include "Voice.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/base/futils.h"


//-----------------------------------------------------------------------------
HEVoice::HEVoice()
: parametersStorage(0),
noteId(-1),
pitch(-1),
tuning(0),
noteOnSampleOffset(0),
noteOffSampleOffset(0),
noteOnVelocity(0),
noteOffVelocity(0),
sampleRate(44100)
{
}

//-----------------------------------------------------------------------------
HEVoice::HEVoice(HEVoice const &newVoice)
: parametersStorage(newVoice.parametersStorage),
noteId(newVoice.noteId),
pitch(newVoice.pitch),
tuning(newVoice.tuning),
noteOnSampleOffset(newVoice.noteOnSampleOffset),
noteOffSampleOffset(newVoice.noteOffSampleOffset),
noteOnVelocity(newVoice.noteOnVelocity),
noteOffVelocity(newVoice.noteOffVelocity),
sampleRate(newVoice.sampleRate)
{
    setParametersStorage(newVoice.parametersStorage);
}

//-----------------------------------------------------------------------------
HEVoice::~HEVoice()
{
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// place les paramètres necessaires à la gestion de la voix.

void HEVoice::setParametersStorage (HEParametersStorage_Voice * newParametersStorage)
{
    parametersStorage = newParametersStorage;
    
    currentSinusVolume = newParametersStorage->paramVoiceSinVolume;
    currentSquareVolume = newParametersStorage->paramVoiceSquareVolume;
    currentTriangleVolume = newParametersStorage->paramVoiceTriangularVolume;
}

//-----------------------------------------------------------------------------


void HEVoice::noteOn (Steinberg::int32 pitch, double velocity, float tuning, Steinberg::int32 sampleOffset, Steinberg::int32 noteId)
{
    this->pitch = pitch;
    noteOnVelocity = velocity;
    noteOnSampleOffset = sampleOffset;
    this->noteId = noteId;
    this->tuning = tuning;
    
    currentVolume = 0;
    this->values[kVolumeMod] = 0;
    levelFromVel = 1.f + this->parametersStorage->paramVoiceVelocity * (velocity - 1.);
    
    currentSinusVolume = this->values[kSinusVolume] = this->parametersStorage->paramVoiceSinVolume;
    currentTriangleVolume = this->values[kTriangleVolume] = this->parametersStorage->paramVoiceTriangularVolume;
    currentTriangleSlope = this->values[kTriangleSlope] = this->parametersStorage->paramVoiceTriangleSlop;
    currentSquareVolume = this->values[kSquareVolume] = this->parametersStorage->paramVoiceSquareVolume;
    
    currentSinusDetune = 0.;
    if (this->parametersStorage->paramVoiceSinusDetune != 0.)
    {
        currentSinusDetune = VoiceStatics::freqTab[this->pitch] * (::pow (2.0, this->parametersStorage->paramVoiceSinusDetune * 2.0 / 12.0) - 1);
    }
    this->values[kSinusDetune] = currentSinusDetune;
    this->values[kTuningMod] = 0;
    
    this->noteOnSampleOffset++;
    
#if DEBUG_LOG
    FDebugPrint ("NoteOn :%d\n", nId);
#endif

}

//-----------------------------------------------------------------------------
// Appellé lorsque la note n'est plus jouée.
// Cette fonction est appellé par le processeur de voix lorsque celui ci envoie l'evènement noteOff.

void HEVoice::noteOff (double velocity, Steinberg::int32 sampleOffset)
{
    noteOffVelocity = velocity;
    noteOffSampleOffset = sampleOffset;
    
    this->noteOffSampleOffset++;
    
    ParamValue timeFactor;
    if (this->values[kReleaseTimeMod] == 0)
        timeFactor = 1;
    else
        timeFactor = ::pow (100., this->values[kReleaseTimeMod]);
    
    noteOffVolumeRamp = 1.0 / (timeFactor * this->sampleRate * ((this->parametersStorage->paramVoiceReleaseTime * MAX_RELEASE_TIME_SEC) + 0.005));
    if (currentVolume)
        noteOffVolumeRamp *= currentVolume;
    
    
#if DEBUG_LOG
    FDebugPrint ("NoteOff:%d\n", this->noteId);
#endif
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

bool HEVoice::process (SamplePrecision* outputBuffers[2], Steinberg::int32 numSamples)
{
    //---compute tuning-------------------------
    
    // main tuning
    ParamValue tuningInHz = 0.;
    if (this->values[kTuningMod] != 0. || this->parametersStorage->paramMasterTuning != 0 || this->tuning != 0)
    {
        tuningInHz = VoiceStatics::freqTab[this->pitch] * (::pow (2.0, (this->values[kTuningMod] * 10 + this->parametersStorage->paramMasterTuning * 2.0 / 12.0 + this->tuning)) - 1);
    }
    
    tuningInHz = 0;
    
    ParamValue triangleFreq = (VoiceStatics::freqTab[this->pitch] + tuningInHz) * M_PI_MUL_2 / this->getSampleRate () / 2.;
    if (currentTriangleF == -1)
        currentTriangleF = triangleFreq;
    // check for frequency changes and update the phase so that it is crackle free
    if (triangleFreq != currentTriangleF)
    {
        // update phase
        trianglePhase = (SamplePrecision)((currentTriangleF - triangleFreq) * n + trianglePhase);
        currentTriangleF = triangleFreq;
    }
    
    // Sinus Detune
    if (currentSinusDetune != this->values[kSinusDetune])
    {
        currentSinusDetune = VoiceStatics::freqTab[this->pitch] * (::pow (2.0, this->values[kSinusDetune] * 2.0 / 12.0) - 1);
    }
    ParamValue sinusFreq = (VoiceStatics::freqTab[this->pitch] + tuningInHz + currentSinusDetune) * M_PI_MUL_2 / this->getSampleRate ();
    if (currentSinusF == -1)
        currentSinusF = sinusFreq;
    if (sinusFreq != currentSinusF)
    {
        // update phase
        sinusPhase = (SamplePrecision)((currentSinusF - sinusFreq) * n) + sinusPhase;
        currentSinusF = sinusFreq;
    }
    
    //---calculate parameter ramps
    ParamValue volumeRamp = 0.;
    ParamValue panningLeftRamp = 0.;
    ParamValue panningRightRamp = 0.;
    ParamValue sinusVolumeRamp = 0.;
    ParamValue triangleVolumeRamp = 0.;
    ParamValue squareVolumeRamp = 0.;
    ParamValue triangleSlopeRamp = 0.;
    ParamValue rampTime = std::max<ParamValue> ((ParamValue)numSamples, (this->sampleRate * 0.005));
    
    ParamValue wantedVolume = VoiceStatics::normalizedLevel2Gain ((float)Bound (0.0, 1.0, this->parametersStorage->paramMasterVolume * levelFromVel + this->values[kVolumeMod]));
    if (wantedVolume != currentVolume)
    {
        volumeRamp = (wantedVolume - currentVolume) / rampTime;
    }
    
    if (this->values[kPanningLeft] != currentPanningLeft)
    {
        panningLeftRamp = (this->values[kPanningLeft] - currentPanningLeft) / rampTime;
    }
    if (this->values[kPanningRight] != currentPanningRight)
    {
        panningRightRamp = (this->values[kPanningRight] - currentPanningRight) / rampTime;
    }
    if (this->values[kSinusVolume] != currentSinusVolume)
    {
        sinusVolumeRamp = (this->values[kSinusVolume] - currentSinusVolume) / rampTime;
    }
    if (this->values[kSquareVolume] != currentSquareVolume)
    {
        squareVolumeRamp = (this->values[kSquareVolume] - currentSquareVolume) / rampTime;
    }
    if (this->values[kTriangleVolume] != currentTriangleVolume)
    {
        triangleVolumeRamp = (this->values[kTriangleVolume] - currentTriangleVolume) / rampTime;
    }
    if (this->values[kTriangleSlope] != currentTriangleSlope)
    {
        triangleSlopeRamp = (this->values[kTriangleSlope] - currentTriangleSlope) / rampTime;
    }
    
    for (int32 i = 0; i < numSamples; i++)
    {
        this->noteOnSampleOffset--;
        this->noteOffSampleOffset--;
        
        if (this->noteOnSampleOffset <= 0)
        {
            // we are in Release
            if (this->noteOffSampleOffset == 0)
            {
                volumeRamp = 0;
                if (currentVolume > 0)
                {
                    // ramp note off
                    currentVolume -= noteOffVolumeRamp;
                    if (currentVolume < 0.)
                        currentVolume = 0.;
                    this->noteOffSampleOffset++;
                }
                else
                {
                    this->noteOffSampleOffset = this->noteOnSampleOffset = -1;
                    return false;
                }
            }
            SamplePrecision sample;
            SamplePrecision osc = (SamplePrecision)sin (n * triangleFreq + trianglePhase);
            // square osc
            sample = (SamplePrecision)((::floor (osc) + 0.5) * currentSquareVolume);
            // triangle osc
            sample += (SamplePrecision)((osc - ::fabs(sin (n * triangleFreq + trianglePhase + 1 + currentTriangleSlope))) * currentTriangleVolume);
            // sinus osc
            sample += (SamplePrecision)(sin (n * sinusFreq + sinusPhase) * currentSinusVolume);
            sample += (SamplePrecision)(sin (n * sinusFreq * 2 + sinusPhase) * currentSinusVolume/3);
            sample += (SamplePrecision)(sin (n * sinusFreq * 4 + sinusPhase) * currentSinusVolume/10);
            
            n++;
            
            // store in output
            outputBuffers[0][i] += (SamplePrecision)(sample * currentPanningLeft * currentVolume);
            outputBuffers[1][i] += (SamplePrecision)(sample * currentPanningRight * currentVolume);
            
            // ramp parameters
            currentVolume += volumeRamp;
            currentPanningLeft += panningLeftRamp;
            currentPanningRight += panningRightRamp;
            currentSinusVolume += sinusVolumeRamp;
            currentSquareVolume += squareVolumeRamp;
            currentTriangleVolume += triangleVolumeRamp;
            currentTriangleSlope += triangleSlopeRamp;
        }
    }
    
    return true;
}



//-----------------------------------------------------------------------------
void HEVoice::reset ()
{
    noteId = -1;
    pitch = -1;
    tuning = 0;
    
    noteOnSampleOffset = -1;
    noteOffSampleOffset = -1;
    noteOnVelocity = 0;
    noteOffVelocity = 0;
    noteOffVolumeRamp = 0.005;
    
    sampleRate = 44100;
    
    noiseStep = 1; 
    noisePos = 0;
    n = 0;
    sinusPhase = trianglePhase = 0.;
    currentSinusF = currentTriangleF = -1.;
    this->values[kVolumeMod] = 0.;
    this->values[kTuningMod] = 0.;
    this->values[kReleaseTimeMod] = 0.;
    currentPanningLeft = this->values[kPanningLeft] = 1.;
    currentPanningRight = this->values[kPanningRight] = 1.;
    currentSinusVolume = this->values[kSinusVolume] = 0.5;
    currentSquareVolume = this->values[kSquareVolume] = 0.5;
    currentTriangleVolume = this->values[kTriangleVolume] = 0.5;
    noteOffVolumeRamp = 0.005;
}


//-----------------------------------------------------------------------------
float VoiceStatics::freqTab[kNumFrequencies];
const float VoiceStatics::scaleHeadRoom = (float)(pow (10.0, -12.0 / 20.0) * 0.70710677); // for 12 dB head room
const float VoiceStatics::scaleNorm2GainC1 = (float)(VoiceStatics::scaleHeadRoom * pow (10.0, 24.0 / 20.0));
const float VoiceStatics::scaleNorm2GainC2 = (float)(24.0 / 20.0 / 0.30102999566398119521373889472449); // Mathd::kLog2
Steinberg::Vst::LogScale<Steinberg::Vst::ParamValue> VoiceStatics::freqLogScale (0., 1., 80., 18000., 0.5, 1800.);

const double VoiceStatics::kNormTuningOneOctave = 12.0 / 240.0;	// full in VST 3 is +- 10 octaves
const double VoiceStatics::kNormTuningOneTune	= 1.0 / 240.0;

//-----------------------------------------------------------------------------
class VoiceStaticsOnce
{
public:
    VoiceStaticsOnce ()
    {
        // make frequency (Hz) table
        double k = 1.059463094359;	// 12th root of 2
        double a = 6.875;	// a
        a *= k;	// b
        a *= k;	// bb
        a *= k;	// c, frequency of midi note 0
        for (Steinberg::int32 i = 0; i < VoiceStatics::kNumFrequencies; i++)	// 128 midi notes
        {
            VoiceStatics::freqTab[i] = (float)a;
            a *= k;
        }
    }
};

static VoiceStaticsOnce gVoiceStaticsOnce;





