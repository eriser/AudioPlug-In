//
//  vst2wrapper.cpp
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 24/10/2014.
//
//

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//                            Wrapping VST2

//#include "public.sdk/source/vst/vst2wrapper/vst2wrapper.sdk.cpp"

#include "public.sdk/source/vst/vst2wrapper/vst2wrapper.h"
#include "HeyMateIds.h"
//------------------------------------------------------------------------
::AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return (::AudioEffect*) Steinberg::Vst::Vst2Wrapper::create (GetPluginFactory (), HEProcessorId, 'HE01', audioMaster);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------