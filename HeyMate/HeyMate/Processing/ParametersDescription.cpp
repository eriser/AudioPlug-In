//
//  Parameters.cpp
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 22/09/2014.
//
//

#include "ParametersDescription.h"


//-----------------------------------------------------------------------------
// Setters et getters des structures des paramètres

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


using namespace Steinberg;


//-----------------------------------------------------------------------------
//tresult HEEventFromUI_Strorage::setState(IBStream *stream)
//{
//    
//    IBStreamer s (stream, kLittleEndian);
//    
//    if (!s.readInt16(sPitch)) return kResultFalse;
//    
//    sTuning = 0;             ///< 1.f = +1 cent, -1.f = -1 cent
//    sVelocity = 1;           ///< range [0.0, 1.0]
//    sLength;                 ///< in sample frames (optional, Note Off has to follow in any case!)
//    sNoteId = -1;            ///< note identifier (if not available then -1)
//    
//    return kResultTrue;
//}
//
//
//tresult HEEventFromUI_Strorage::getState(IBStream *stream)
//{
//    IBStreamer s (stream, kLittleEndian);
//    
//    // Voice parameters
//    if (!s.writeInt16 (sPitch)) return kResultFalse;
//    if (!s.writeInt16 (sChannel)) return kResultFalse;
//    if (!s.writeFloat (sTuning)) return kResultFalse;
//    if (!s.writeFloat (sVelocity)) return kResultFalse;
//    
//    if (!s.writeInt32 (sLength)) return kResultFalse;
//    if (!s.writeInt32 (sNoteId)) return kResultFalse;
//    
//    return kResultTrue;
//}
//



//-----------------------------------------------------------------------------
tresult HEParametersStorage_Voice::setState (IBStream* stream)
{
	IBStreamer s (stream, kLittleEndian);
    
    if (!s.readDouble (paramVoiceSinVolume)) return kResultFalse;
    if (!s.readDouble (paramVoiceTriangularVolume)) return kResultFalse;
    if (!s.readDouble (paramVoiceSquareVolume)) return kResultFalse;
    if (!s.readDouble (paramVoiceReleaseTime)) return kResultFalse;
    if (!s.readDouble (paramVoiceSinusDetune)) return kResultFalse;
    if (!s.readDouble (paramVoiceTriangleSlop)) return kResultFalse;
    
    if (!s.readDouble (paramMasterVolume)) return kResultFalse;
    if (!s.readDouble (paramMasterTuning)) return kResultFalse;
    if (!s.readDouble (paramVoiceVelocity)) return kResultFalse;
    if (!s.readInt8 (paramTuningRange)) return kResultFalse;
    
    if (!s.readInt16 (sPitch)) return kResultFalse;
    
	return kResultTrue;
}

//-----------------------------------------------------------------------------
tresult HEParametersStorage_Voice::getState (IBStream* stream)
{
	IBStreamer s (stream, kLittleEndian);
    
    if (!s.writeDouble (paramVoiceSinVolume)) return kResultFalse;
    if (!s.writeDouble (paramVoiceTriangularVolume)) return kResultFalse;
    if (!s.writeDouble (paramVoiceSquareVolume)) return kResultFalse;
    if (!s.writeDouble (paramVoiceReleaseTime)) return kResultFalse;
    if (!s.writeDouble (paramVoiceSinusDetune)) return kResultFalse;
    if (!s.writeDouble (paramVoiceTriangleSlop)) return kResultFalse;
    
    if (!s.writeDouble (paramMasterVolume)) return kResultFalse;
    if (!s.writeDouble (paramMasterTuning)) return kResultFalse;
    if (!s.writeDouble (paramVoiceVelocity)) return kResultFalse;
    if (!s.writeInt8 (paramTuningRange)) return kResultFalse;
    
    if (!s.writeInt16 (sPitch)) return kResultFalse;
    
	return kResultTrue;
}



//-----------------------------------------------------------------------------
tresult HEParametersStorage_Delay::setState (IBStream* stream)
{
    IBStreamer s (stream, kLittleEndian);
    
    // Delay parameters
    if (!s.readDouble (paramDelayTime)) return kResultFalse;
    if (!s.readDouble (paramDelayDepth)) return kResultFalse;
    if (!s.readInt32 (paramDelayFeedBack)) return kResultFalse;
    
    return kResultTrue;
}

//-----------------------------------------------------------------------------
tresult HEParametersStorage_Delay::getState (IBStream* stream)
{
    IBStreamer s (stream, kLittleEndian);
    
    // Delay parameters
    if (!s.writeDouble (paramDelayTime)) return kResultFalse;
    if (!s.writeDouble (paramDelayDepth)) return kResultFalse;
    if (!s.writeInt32 (paramDelayDepth)) return kResultFalse;
    
    return kResultTrue;
}
