//
//  Controller.h
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 19/09/2014.
//
//

#ifndef __HeyMate__Controller__
#define __HeyMate__Controller__



#include "public.sdk/source/vst/vsteditcontroller.h"

// deux classe d'éditeur différent,
// un pour l'iphone,
// un pour les plugins
// note que les deux éditeur ont les mêmes fonctions de partage avec le controller.

#include "Editor.h"
#include "pluginterfaces/vst/ivstnoteexpression.h"

using namespace Steinberg;
using namespace Vst;

#define NUM_FILTER_TYPE 3

class HEController : public Steinberg::Vst::EditController, public Steinberg::Vst::IMidiMapping
{
public:
    
    // EditController
    tresult PLUGIN_API initialize (FUnknown* context);
    tresult PLUGIN_API terminate ();
    tresult PLUGIN_API setComponentState (IBStream* state);
    
    virtual tresult PLUGIN_API setParamNormalized (ParamID tag, ParamValue value);
    
    // Choix de l'éditeur
    IPlugView* PLUGIN_API createView (FIDString name) override;

    
    // IMidiMapping
    virtual tresult PLUGIN_API getMidiControllerAssignment (int32 busIndex, int16 channel, CtrlNumber midiControllerNumber, ParamID& id/*out*/);
    
    
    // utilisé par la séparation Effet, Controller.
    static FUnknown* createInstance (void*) { return (IEditController*) new HEController (); }
    static FUID cid;
    
    HEEditor * getEditor() {return editor;};
    void setEditor(HEEditor * attachedEditor) {editor = attachedEditor;};
    
    enum NoteExpressionTypeIds {
        kNoiseVolumeTypeID = kCustomStart,
        kFilterFreqModTypeID,
        kFilterQModTypeID,
        kSinusVolumeTypeID,
        kTriangleVolumeTypeID,
        kFilterTypeTypeID,
        kTriangleSlopeTypeID,
        kSinusDetuneTypeID,
        kReleaseTimeModTypeID,
        kTextInputTypeID,
        kSquareVolumeTypeID,
    };
    
    OBJ_METHODS (HEController, EditController)
    DEFINE_INTERFACES
    DEF_INTERFACE (IMidiMapping)
    END_DEFINE_INTERFACES (EditController)
    REFCOUNT_METHODS(EditController)
    
private:
    // lien vers le processeur pour l'envoie de note Midi.
    //HEProcessor * processor;

    // lien vers l'éditeur.
    // --------------------
    HEEditor * editor;

    
};


#endif /* defined(__HeyMate__Controller__) */
