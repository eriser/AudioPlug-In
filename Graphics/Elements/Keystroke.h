//
//  Keystroke.h
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 12/11/2014.
//
//

#ifndef __HeyMate__Keystroke__
#define __HeyMate__Keystroke__


#include "lib/controls/ccontrol.h"

// Paramètres de la touche
// -----------------------





class HEKeyboard;
// pour la liaison à CControl
using namespace VSTGUI;


// classe des controles touches de claviers
// ----------------------------------------

class HEKeystroke : public VSTGUI::CControl
{
public:
    
    HEKeystroke (VSTGUI::CRect &newKeystrokeSize, HEKeyboard * _delegate, int _noteNbr, bool _isWhite);
    HEKeystroke (HEKeystroke * oldStroke);
    ~HEKeystroke ();
    
    
    // dessin de la touche
    // -------------------
    virtual void draw (VSTGUI::CDrawContext * pContext);
    // Dessin du backGround
    void drawBackground (VSTGUI::CDrawContext * pContext);
    // Dessin de l'handle
    void drawActiveStroke(VSTGUI::CDrawContext *pContext);
    
    
    
    // From CControls
    // --------------
    virtual VSTGUI::CMouseEventResult onMouseDown (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons);
    virtual VSTGUI::CMouseEventResult onMouseUp (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons);
    virtual VSTGUI::CMouseEventResult onMouseMoved (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons);
    
    
    // accesing the note number
    // ------------------------
    void setNote (int newNoteNbr);
    int getNote ();
    
    
    // communication avec le controleur
    // --------------------------------
    void sendNoteOn ();
    void sendNoteOff ();
    
    
    
    // fonction helpers
    // ----------------
    
    
    
    // from CControl
    // -------------
    CLASS_METHODS (HEKeystroke, CControl)
    
    
    
protected:
    // delegate des touches
    HEKeyboard * keyboard;
    
    // numéro de la touche, détérminera son pitch.
    int noteNbr;
    
    // touche blanche ou noire
    bool isWhite;
    bool isKeyboardFrontier;
    
    // touche activé?
    bool isActive;
    
    // retention de la position du clic.
    VSTGUI::CPoint oldWhere;
    
    // paramètres de dimenssionnement
    VSTGUI::CRect keystrokeSize;
    
    
    
};




#endif /* defined(__HeyMate__Keystroke__) */
