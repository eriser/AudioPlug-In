//
//  Editor_iOS.h
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 17/10/2014.
//
//

#ifndef __HeyMate__Editor_iOS__
#define __HeyMate__Editor_iOS__

#include "public.sdk/source/vst/vstguieditor.h"
#include "pluginterfaces/vst/ivsteditcontroller.h"

#if TARGET_OS_IPHONE
#import "../HeyMateApp/ViewController.h"
#endif




// Paramètre de l'éditeur
// ----------------------
#define DEFAULT_EDITOR_WIDTH        350
#define DEFAULT_EDITOR_HEIGHT       120
#define WANT_GRAPHICAL_KEYBOARD     1       // 1 si oui...

class HEGraphicComponentsList;
class HEKeyboard;
class HEController;



class HEEditor : public Steinberg::Vst::VSTGUIEditor, public CControlListener
{
public:
    //------------------------------------------------------------------------
    HEEditor ();
    HEEditor (HEController * attachedController);
    virtual ~HEEditor ();
    
    
    // from VSTGUIEditor
    // ---------------------------------
    virtual bool PLUGIN_API open (void* parent, const PlatformType& platformType = kDefaultNative);
    virtual void PLUGIN_API close ();
    
    // from CControlListener
    // ---------------------------------
    void valueChanged (CControl* pControl);
    void controlBeginEdit (CControl* pControl);
    void controlEndEdit (CControl* pControl);
    
    
    
    // ------------------------------------
    // fonctions internes
    // ------------------------------------
    
    // ouverture des composants graphiques.
    // ------------------------------------
    bool openGraphicalsComponents ();
    bool openGraphicalKeyboard ();
    
    // mise-à-jour des composants graphiques.
    // ------------------------------------
    //void update (Steinberg::Vst::ParamID tag, Steinberg::Vst::ParamValue value);
    
    // ------------------------------------
    // fonctions de communication (setters / getter)
    // ------------------------------------
    // setters du controller (réciprocité assuré)
    void attachController (HEController * newController);
    
    // setter et getters de la largeur de la vue.
    double getViewWidth () {return viewWidth;} ;
    double getViewHeight () {return viewHeight;} ;
    void setViewWidth (double newWidth) {viewWidth = newWidth;} ;
    void setViewHeight (double newHeight) {viewHeight = newHeight;} ;
    
#if TARGET_OS_IPHONE
    // getters de la vue iOS
    UIViewController * getViewController () {return viewController;} ;
    void setViewController (UIViewController * newViewController)
    {viewController = (HEViewController*)newViewController;} ;
#endif
    
    
    
protected:
#if TARGET_OS_IPHONE
    // view controller
    HEViewController * viewController;
#endif
    // controller
    HEController * controller;
    
    CBitmap* background;
    
    // liste des composants graphiques
    HEGraphicComponentsList * components;
    
    // graphical keyboard
    HEKeyboard * keyboard;
    
    // utiliser au départ pour l'ouverture de l'éditeur.
    // -------------------------------------------------
    double viewWidth;
    double viewHeight;
    
};




#endif /* defined(__HeyMate__Editor_iOS__) */
