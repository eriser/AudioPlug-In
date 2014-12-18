//
//  Keyboard.h
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 12/11/2014.
//
//
/**  Classe d'interface entre l'éditeur et les touches du clavier.
 -------------------------------------------------------------
 Cette classe est chargé de la gestion des touches :
 de leur construction et de leur allocation,
 de leurs placement,
 de leurs pitch,
 ...
*/

#ifndef __HeyMate__Keyboard__
#define __HeyMate__Keyboard__

// Paramètres du keyboard
// ---------------------
#define MAX_NBR_STROKES_ON_VIEW      24 // 2 octave max sur la vue
#define MAX_NBR_STROKES              88 // nbr de touches sur un clavier...


#include <vector>

#include "lib/cviewcontainer.h"
#include "ivstevents.h"

#include "Keystroke.h"

class HEController;
class HEEditor;
class HESlider;
class HETextLabel;


// enumération des controls interne au clavier
enum {
    kKeyboardSizeSlider = 8008,
    kKeyboardPositionSlider,
};

// pour la liaison à CControl
using namespace VSTGUI;


class HEKeyboard : public VSTGUI::CControl, public CControlListener
{
public:
    // valeurs par défault : touche C4, 1 octave
    HEKeyboard (VSTGUI::CRect &newKeyboardSize, HEEditor * _editor, HEController * _controller, int _firstStroke = 52, int nbrOfStroke = 13);
    ~HEKeyboard ();
    
    
    // Construction et dessin du clavier
    // ---------------------------------
    void buildKeyboard ();
    
    // dessin du clavier (surtout du fond...)
    // --------------------------------------
    virtual void draw (VSTGUI::CDrawContext * pContext);
    // Dessin du backGround
    void drawBackground (VSTGUI::CDrawContext * pContext);
    // mise à jour de l'affichage de la première note
    void updateLabel ();
    // dessin des bonnes touches...
    void scaleKeyboard ();

    
    
    // from CControlListener
    // ---------------------------------
    void valueChanged (CControl* pControl);
    
    // From CControls
    // --------------
    virtual VSTGUI::CMouseEventResult onMouseDown (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons);
    virtual VSTGUI::CMouseEventResult onMouseUp (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons);
    virtual VSTGUI::CMouseEventResult onMouseMoved (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons);
    
    
    // gestion des appuies des touches
    // -------------------------------
    void mouseMouvedOnKeyboard ();
    void sendNoteOn (Steinberg::Vst::Event noteOn);
    void sendNoteOff (Steinberg::Vst::Event noteOff);
    
    
    
    // fonction helpers
    // ----------------
    // détection touches noires/blanches
    bool isStrokeWhite(int strokeNbr);
    // compte le nombre de touches blanches/noir
    int getWhiteStrokesNbr (int startStroke, int endStrokes);
    int getBlackStrokesNbr (int startStroke, int endStrokes);
    
    
    CLASS_METHODS(HEKeyboard, CControl)
    
protected:
    // lien vers le controller
    HEController * controller;
    
    // lien vers l'éditeur
    HEEditor * editor;
    
    // slider pour les changement
    // du nombre de touches :
    HESlider * keyboardSizeSlider;
    // de la position du clavier
    HESlider * keyboardPositionSlider; // donne la première note
    // affichage de la première note
    HETextLabel * firstStrokeLabel;
    
    // listes des touches du clavier
    std::vector<HEKeystroke*> keystrokes;
    
    // paramètre du clavier
    int firstStroke;
    int nbrOfStrokes;
    
    // paramètres de dimentionnment
    VSTGUI::CRect backgroundRect;
    VSTGUI::CRect keyboardRect;
    double spacing;
    VSTGUI::CRect keyboardPositionSliderRect;
    VSTGUI::CRect keyboardSizeSliderRect;
    
    VSTGUI::CRect whiteStrokeSize; // à décaller pour créer chaque touches
    double interStrokeSpacing;
    VSTGUI::CRect blackStrokeSize;
    VSTGUI::CRect halfBlackStrokeSize;
    
    
};















#endif /* defined(__HeyMate__Keyboard__) */
