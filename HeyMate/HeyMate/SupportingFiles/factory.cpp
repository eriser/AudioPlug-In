//
//  factory.cpp
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 29/09/2014.
//
// Description :
// création et lancement des composants et des classes



#include "pluginfactoryvst3.h"

#include "Processor.h"
#include "Controller.h"

//#include "HeyMateIds.h"
#include "HeyMate_VersionRuler.h"	// for versioning


#define stringPluginName "HeyMate Synth"


BEGIN_FACTORY_DEF ("Heimat", "", "hadhenri@gmail.com")

DEF_CLASS2 (INLINE_UID_FROM_FUID(HEProcessor::cid),
            PClassInfo::kManyInstances,
            kVstAudioEffectClass,
            stringPluginName,
            Vst::kDistributable,
            Vst::PlugType::kInstrumentSynth,
            FULL_VERSION_STR,
            kVstVersionString,
            HEProcessor::createInstance)

DEF_CLASS2 (INLINE_UID_FROM_FUID(HEController::cid),
            PClassInfo::kManyInstances,
            kVstComponentControllerClass,
            stringPluginName,
            0,						// not used here
            "",						// not used here
            FULL_VERSION_STR,
            kVstVersionString,
            HEController::createInstance)

END_FACTORY




bool InitModule () { return true; }
bool DeinitModule () { return true; }