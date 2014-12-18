//
//  Controller.cpp
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 19/09/2014.
//
//

#include "Controller.h"

#include "../Processing/ParametersDescription.h"


FUID HEController::cid (0x2AC0A888, 0x9406497F, 0xBBA6EABF, 0xC78D1372);

/** Fonction d'initialisation de l'editcontroller.
 initialise les paramètres.
 */

tresult PLUGIN_API HEController::initialize (FUnknown *context){
    tresult result = EditController::initialize(context);
    if (result == kResultTrue){
        
        
        // ----------------------------------------
        
        //      Initialisation des paramètres
        // ----------------------------------------
        // ----------------------------------------
        
        Parameter *newParam;
        
        /** initialisation d'un paramètres type range :
         new RangeParameter (   const TChar* title,
                            ParamID tag,
                            const TChar* units = 0,
                            ParamValue minPlain = 0.,
                            ParamValue maxPlain = 1.,
                            ParamValue defaultValuePlain = 0.,
                            int32 stepCount = 0,
                            int32 flags = ParameterInfo::kCanAutomate,
                            UnitID unitID = kRootUnitId);
         ajoute à parameters (ParametersContainer, attribut de EditController, liste des paramètres de l'éditeur)
         */
        
        // Initialisation des paramètres des voices.
        // --------------------------------------------
        //sin
        newParam = new RangeParameter ((const char16 *)"Sinus Intensity", kParamVoiceSinVolume, (const char16 *)"%", 0, 100, 50);
        newParam -> setPrecision(1);
        parameters.addParameter(newParam);
        // square
        newParam = new RangeParameter ((const char16 *)"Square Intensity", kParamVoiceSquareVolume, (const char16 *)"%", 0, 100, 50);
        newParam -> setPrecision(1);
        parameters.addParameter(newParam);
        // Triangular
        newParam = new RangeParameter ((const char16 *)"Triangular Intensity", kParamVoiceTriangularVolume, (const char16 *)"%", 0, 100, 50);
        newParam -> setPrecision(1);
        parameters.addParameter(newParam);
        
        // ReleaseTime
        newParam = new RangeParameter ((const char16 *)"Release", kParamVoiceRelease, (const char16 *)"s", 0, 10, 0);
        newParam -> setPrecision(1);
        parameters.addParameter(newParam);
        // Velocit
        newParam = new RangeParameter ((const char16 *)"Velocity", kParamVoiceVelocity, (const char16 *)"s", 0, 10, 1);
        newParam -> setPrecision(1);
        parameters.addParameter(newParam);
        
        // Master Volume
        newParam = new RangeParameter ((const char16 *)"Master Volume", kParamVoiceTriangularVolume, (const char16 *)"%", 0, 100, 50);
        newParam -> setPrecision(1);
        parameters.addParameter(newParam);
        
        
        // voies actives
        parameters.addParameter (new RangeParameter ((const char16 *)"Active Voices", kParamActiveVoices, 0, 0, MAX_VOICES, 0, MAX_VOICES, ParameterInfo::kIsReadOnly));
        
        parameters.addParameter (new RangeParameter ((const char16 *)"PitchNote", kKeystroke, 0, 0, MAX_VOICES, 0, MAX_VOICES, ParameterInfo::kIsReadOnly));
        
        
        
        // Initialisation des paramètres du clavier (octave)
        // -------------------------------------------------
        StringListParameter* tuningRangeParam = new StringListParameter ((const char16 *)"Tuning Range", kParamTuningRange, 0, ParameterInfo::kIsList);
        tuningRangeParam->appendString ((const char16 *)"[-1, +1] Octave");
        tuningRangeParam->appendString ((const char16 *)"[-3, +2] Tunes");
        parameters.addParameter (tuningRangeParam);
        
    }
    
    
    // set the knob mode
    // si le knob mode n'est pas défini, il est par défault changeant pour chaque type de plateforme
    setKnobMode(KnobModes::kLinearMode); // kCircularMode ne fonctionne pas...
    
    
    
    return result;
}


tresult PLUGIN_API HEController::terminate ()
{
    return EditController::terminate ();
}

//-----------------------------------------------------------------------------
// utiliser lors d'un changement des paramètres par le processeur (par exemple)
tresult PLUGIN_API HEController::setComponentState (IBStream* state)
{
    HEParametersStorage_Voice voiceParameters;
    tresult result = voiceParameters.setState (state);
    if (result == kResultTrue)
    {
        // arrangement des paramètres du générateur...
        HEController::setParamNormalized (kParamVoiceSinVolume, voiceParameters.paramVoiceSinVolume);
        HEController::setParamNormalized (kParamVoiceSquareVolume, voiceParameters.paramVoiceSquareVolume);
        HEController::setParamNormalized (kParamVoiceTriangularVolume, voiceParameters.paramVoiceTriangularVolume);
        
        HEController::setParamNormalized (kParamVoiceRelease, voiceParameters.paramVoiceReleaseTime);
        HEController::setParamNormalized (kParamVoiceVelocity, voiceParameters.paramVoiceVelocity);
        
        HEController::setParamNormalized (kParamMasterVolume, voiceParameters.paramMasterVolume);
        
        HEController::setParamNormalized (kParamTuningRange, voiceParameters.paramTuningRange);
        
        HEController::setParamNormalized (kKeystroke, voiceParameters.sPitch);
        
    }
    
    return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API HEController::setParamNormalized (ParamID tag, ParamValue value)
{
    // Attention : Exeption du Tuning Range.
    tresult res = EditController::setParamNormalized (tag, value);
    return res;
}


//-----------------------------------------------------------------------------
tresult PLUGIN_API HEController::getMidiControllerAssignment (int32 busIndex, int16 channel, CtrlNumber midiControllerNumber, ParamID& id/*out*/)
{
    if (busIndex == 0 && channel == 0)
    {
        id = 0;
        switch (midiControllerNumber)
        {
                // **TODO**
                /** on peux régler l'assignation des controles midi
                 en utilisant cette fonction de la manière suivante :
            case kPitchBend: id = kParamMasterTuning; break;
            case kCtrlVolume: id = kParamMasterVolume; break;
            case kCtrlFilterCutoff: id = kParamFilterFreq; break;
            case kCtrlFilterResonance: id = kParamFilterQ; break;
                 */
        }
        return id != 0 ? kResultTrue : kResultFalse;
    }
    return kResultFalse;
}




//-----------------------------------------------------------------------------
IPlugView* PLUGIN_API HEController::createView (FIDString name)
{
	if (strcmp (name, ViewType::kEditor) == 0)
	{
        return new HEEditor(this); // assure déjà la réciprocité;
	}
	return 0;
}



