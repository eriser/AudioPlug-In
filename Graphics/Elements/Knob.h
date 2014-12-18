//
//  Knob.h
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 08/10/2014.
//
//

#ifndef __HeyMate__Knob__
#define __HeyMate__Knob__

// Paramètres des knobs
// --------------------
#define KNOB_SIZE               50 // largeur normale du bouton
#define DISPLAY_VALUE           1
#define TEXT_WIDTH_PERCENT      50
#define TEXT_HEIGHT_PERCENT     30
#define FONTSIZE                18

#include "lib/controls/cknob.h"

class HETextLabel;

class HEKnob : public VSTGUI::CKnob
{
public:
    HEKnob(VSTGUI::CRect &frameRect, CControlListener * listener, int tag);
    ~HEKnob();
    
    // dessin du bouton et gestion du dessin lors des clics.
    // -----------------------------------------------------
    virtual void draw (VSTGUI::CDrawContext *pContext);
    
    // Dessin du backGround
    void drawBackground (VSTGUI::CDrawContext * pContext);
    // dessin des contours du bouton, uniquement pour la construction
    void drawViewStrokes (VSTGUI::CDrawContext *pContext);
    // Dessin de l'handle
    void drawHandle (VSTGUI::CDrawContext *pContext);
    // mise à jour de la valeur à l'intérieur.
    void updateTextLabelValue ();
    
    
    // From CControls
    // --------------
    virtual VSTGUI::CMouseEventResult onMouseDown (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons);
    virtual VSTGUI::CMouseEventResult onMouseUp (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons);
    virtual VSTGUI::CMouseEventResult onMouseMoved (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons);
    
    
    // fonction helpers
    // ----------------
    VSTGUI::CCoord getAngle();
    void valueToPoint(VSTGUI::CPoint &point, double radius);
    
private:
    // text Label, affichage de la valeur du bouton...
    HETextLabel * valueTextLabel;
    
    // variables de dimensionnement
    double backgroundWidth;
    double safepadding;
    double handleWidth;
    
    // rectangle de dessin
    double ecartementInterieur;
    VSTGUI::CRect coronaBackground;
    VSTGUI::CRect coronaExteriorShadow;
    VSTGUI::CRect coronaInteriorShadow;
    VSTGUI::CRect coronaExterior;
    VSTGUI::CRect coronaInterior;
    
    
    
};

#endif /* defined(__HeyMate__Knob__) */
