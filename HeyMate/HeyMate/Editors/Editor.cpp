//
//  Editor_iOS.cpp
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 17/10/2014.
//
//

#import "Editor.h"

#import "Controller.h"

#import "../../Graphics/General/GraphicHelp.h"
#import "../Processing/ParametersDescription.h"
// import des éléments graphiques
#include "../../../Graphics/General/GraphicComponentsList.h"
// import du keybord
#include "../../../Graphics/Elements/Keyboard.h"

//------------------------------------------------------------------------
HEEditor::HEEditor ()
: VSTGUIEditor (0)
, controller (0)
, components (new HEGraphicComponentsList (this))
, keyboard (0)
, viewWidth (DEFAULT_EDITOR_WIDTH)
, viewHeight (DEFAULT_EDITOR_HEIGHT)
#if TARGET_OS_IPHONE
, viewController ([HEViewController alloc])
#endif
{
    setIdleRate (50); // 1000ms/50ms = 20Hz
    
#if TARGET_OS_IPHONE
    if (viewController)
    {
        // la vue doit être créée en mode landscape.
        // on doit donc inverser les éléments de tailles pour avoir la bonne orientation
        if (UIInterfaceOrientationIsPortrait([[UIApplication sharedApplication] statusBarOrientation]))
        {
            viewWidth = viewController.view.bounds.size.height;
            viewHeight = viewController.view.bounds.size.width;
        }
        else
        {
            viewWidth = viewController.view.bounds.size.width;
            viewHeight = viewController.view.bounds.size.height;
        }
        
    }
#endif
    
}
//------------------------------------------------------------------------
HEEditor::HEEditor (HEController * attachedController)
: VSTGUIEditor (attachedController)
, controller (attachedController)
, components (new HEGraphicComponentsList (this))
, keyboard(0)
, viewWidth (DEFAULT_EDITOR_WIDTH)
, viewHeight (DEFAULT_EDITOR_HEIGHT)
# if TARGET_OS_IPHONE
, viewController ([HEViewController alloc])
#endif
{
    setIdleRate (50); // 1000ms/50ms = 20Hz
    
#if TARGET_OS_IPHONE
    if (viewController)
    {
        // la vue doit être créée en mode landscape.
        // on doit donc inverser les éléments de tailles pour avoir la bonne orientation
        if (UIInterfaceOrientationIsPortrait([[UIApplication sharedApplication] statusBarOrientation]))
        {
            viewWidth = viewController.view.bounds.size.height;
            viewHeight = viewController.view.bounds.size.width;
        }
        else
        {
            viewWidth = viewController.view.bounds.size.width;
            viewHeight = viewController.view.bounds.size.height;
        }
        
    }
#endif
    
}

//------------------------------------------------------------------------
HEEditor::~HEEditor ()
{
    
}




//------------------------------------------------------------------------
//                      From VSTGUIEditor
//------------------------------------------------------------------------
//------------------------------------------------------------------------


//------------------------------------------------------------------------
bool HEEditor::open (void* parent, const PlatformType& platformType)
{
    if (frame) // already attached!
    {
        return false;
    }
    
    
    // Dans iOS on adapte le plug-in à la taille de l'écran
    // Sur une autre plateforme on dapte la taille du plug-in à la taille de l'arrière plan
    // resize de l'image d'arrière plan pour matcher les contours de l'application
    
    
#if TARGET_OS_IPHONE
    // resize de l'image en fonction de l'écran.
    // utilisation de l'UIKit, PARCEQUE C'EST SIMPLE !!
    background = new CBitmap ("background_iOS.png");
    CRect rectScreenSize = graphicHelp::makeCRectFromTopLeft(0, 0, viewWidth, viewHeight);
    background = graphicHelp::rescaleCBitmap(background, rectScreenSize);
    
#else
    background = new CBitmap ("background.png");
    // resize de la vue de l'éditeur en fonction de l'image de fond.
    viewWidth = background->getWidth();
    viewHeight = background->getHeight();
    
#endif
    
    // création de la fenêtre du plug-in
    CRect editorSize (0, 0, viewWidth, viewHeight);
    frame = new CFrame (editorSize, this);
    frame->CFrame::open(parent);
    
    frame->setBackgroundColor (kGreenCColor);
    
    frame->setBackground(background);
    
    
    bool res = true;
    // Ouverture des composants graphiques
    // -----------------------------------
    res = res && openGraphicalsComponents();
    
    // Ouverture du clavier graphique
    // ------------------------------
    if (WANT_GRAPHICAL_KEYBOARD==1)
        res = res && openGraphicalKeyboard();
    
    
    return res;
}


//------------------------------------------------------------------------
void PLUGIN_API HEEditor::close ()
{
    if (frame)
    {
        frame->forget();
        frame = 0;
    }
    
    // fermeture des composant graphiques
    components->deleteComponents();
}



//------------------------------------------------------------------------
//                      From CControlListener
//------------------------------------------------------------------------
//------------------------------------------------------------------------

void HEEditor::valueChanged (VSTGUI::CControl* pControl)
{
    
    switch (pControl->getTag())
    {
        case kParamMasterVolume:
            controller->beginEdit(kParamMasterVolume);
            controller->setParamNormalized (kParamMasterVolume, pControl->getValue ());
            controller->performEdit (kParamMasterVolume, pControl->getValue ());
            controller->endEdit(kParamMasterVolume);
            break;
        //------------------
        case kParamActiveVoices:
            controller->setParamNormalized (kParamActiveVoices, pControl->getValue ());
            controller->performEdit (kParamActiveVoices, pControl->getValue ());
            break;
        //------------------
        //------------------
        default:
            controller->setParamNormalized (pControl->getTag(), pControl->getValue ());
            controller->performEdit (pControl->getTag(), pControl->getValue ());
            break;
    }
    
}


//------------------------------------------------------------------------
void HEEditor::controlBeginEdit (CControl* pControl)
{
    switch (pControl->getTag ())
    {
        //------------------
        case kParamMasterVolume:
            controller->beginEdit (kParamMasterVolume);
            break;
        //------------------
        case kParamActiveVoices:
            controller->beginEdit (kParamActiveVoices);
            break;
        //------------------
        //------------------
        default:
            controller->beginEdit(pControl->getTag());
    }
}


//------------------------------------------------------------------------
void HEEditor::controlEndEdit (CControl* pControl)
{
    switch (pControl->getTag ())
    {
        //------------------
        case kParamMasterVolume: // personnalisation de chaque composant graphique possible.
            controller->endEdit (kParamMasterVolume);
        	break;
        //------------------
        case kParamActiveVoices:
            controller->endEdit (kParamActiveVoices);
            break;
        //------------------
        //------------------
        default:
            controller->endEdit(pControl->getTag());
    }
    
}






//------------------------------------------------------------------------
//                      Fonctions interne
//------------------------------------------------------------------------
//------------------------------------------------------------------------


bool HEEditor::openGraphicalsComponents ()
{
    bool res = true;
    
    
    // Placement des composants
    // ------------------------
    double colon1 = 100;
    double colon2 = 150;
    double colon3 = 200;
    double colon4 = 250;
    double colon5 = 300;
    double colon6 = 350;
    double colon7 = 400;
    double colon8 = 450;
    double colon9 = 500;
    double colon10 = 550;
    double colon11 = 600;
    double colon12 = 650;
    double colon13 = 700;
    double colon14 = 750;
    double colon15 = 800;
    double colon16 = 850;
    double colon17 = 900;
    double colon18 = 950;
    
    double line1 = 100;
    double line2 = 150;
    double line3 = 200;
    double line4 = 250;
    double line5 = 300;
    double line6 = 350;
    double line7 = 400;
    double line8 = 450;
    double line9 = 500;
    double line10 = 650;
    
    
    
    // création des knob
    // -----------------
    // tailles par défaut
    components->createKnob(kParamMasterVolume, colon1, line1);
    components->createKnob(kParamMasterVolume, colon2, line1);
    // plus grand
    CRect bigerKnob = graphicHelp::makeCRect(colon4, line1, 100, 100);
    components->createKnob(kParamMasterVolume, bigerKnob);
    // plus petit
    CRect smallerKnob = graphicHelp::makeCRect(colon6, line1, 25, 25);
    components->createKnob(kParamMasterVolume, smallerKnob);

    
    
    // création des slider
    // -------------------
    // tailles par défaut
    components->createSlider(kParamVoiceSquareVolume, colon1, line4); // slider vertical
    components->createSlider(kParamVoiceSquareVolume, colon2, line4, true, true); // slider vertical inversé
    components->createSlider(kParamVoiceSinVolume, colon5, line3, false); // slider horizontal
    components->createSlider(kParamVoiceSinVolume, colon5, line4, false, true); // slider horizontal inversé
    
    
    // création de textLabel
    // ---------------------
    components->createTextLabel(colon2, line7, "yeaaaaaaaaah");
    components->createTextLabel(colon2, line8, "Mannnnn, you're great!");
    CRect textRect = graphicHelp::makeCRect(colon2, line9, 30, 30);
    components->createTextLabel(textRect, "alllllllll right ! lorem ipsum and all.\n Of you are so fucking duck... i hate duck.", CTextLabel::kTruncateHead);
    
    
    // création des boutons
    // --------------------
    // On/Off
    components->createOnOffButton(kParamActiveVoices, colon3, line6);
    
    // kick
    components->createKickButton(kParamActiveVoices, colon4, line6);
    components->createKickButton(kParamActiveVoices, colon5, line6, false);
    CRect bigKick = graphicHelp::makeCRect(colon7, line6, 75, 75);
    components->createKickButton(kParamActiveVoices, bigKick);
    
    
    
    // création des Switch Vertical
    // ----------------------------
    components->createVerticalSwitch(kParamDelayDepth, colon9, line2, 4); // boutons rond
    components->createVerticalSwitch(kParamDelayDepth, colon10, line2, 4, false); // bouton carré
    components->createVerticalSwitch(kParamDelayDepth, colon11, line2, 6);
    
    // création de switch horizontaux
    // ------------------------------
    components->createHorizontalSwitch(kParamDelayFeedBack, colon11, line6, 4); // bouton Rond
    components->createHorizontalSwitch(kParamDelayFeedBack, colon11, line7, 4, false); // bouton carré
    components->createHorizontalSwitch(kParamDelayFeedBack, colon11, line8, 6);
    
    return res;
}



//------------------------------------------------------------------------
bool HEEditor::openGraphicalKeyboard()
{
    bool res = true;
    double keyboardHeight = 225;
    VSTGUI::CRect keyboardRect = graphicHelp::makeCRect(getViewWidth()/2,
                                                        getViewHeight() - keyboardHeight/2,
                                                        getViewWidth() - 25,
                                                        keyboardHeight);
    
    keyboard = new HEKeyboard (keyboardRect, this, controller);
    
    if (keyboard)
    {
        getFrame()->addView(keyboard);
    }
    
    return res;
}











//------------------------------------------------------------------------
//                      Fonctions de communications
//------------------------------------------------------------------------
//------------------------------------------------------------------------

void HEEditor::attachController (HEController * newController)
{
    controller = newController;
    newController->setEditor(this);
}
