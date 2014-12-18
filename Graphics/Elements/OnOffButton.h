//
//  OnOffButton.h
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 02/11/2014.
//
//

#ifndef __HeyMate__OnOffButton__
#define __HeyMate__OnOffButton__

// Paramètres du boutton
// ---------------------
#define ONOFF_HEIGHT            50 // largeur normale du slider
#define ONOFF_WIDTH             50 // longueur normale


#include "lib/controls/cbuttons.h"


class HEOnOffButton : public VSTGUI::COnOffButton
{
public:
    HEOnOffButton (VSTGUI::CRect &frameRect, CControlListener * listener, int tag, int32_t style = 0);
    ~HEOnOffButton ();
    
    
    // dessin du bouton
    // ----------------
    virtual void draw (VSTGUI::CDrawContext * pContext);
    // Dessin du backGround
    void drawBackground (VSTGUI::CDrawContext * pContext);
    // dessin des contours du bouton, uniquement pour la construction
    void drawViewStrokes (VSTGUI::CDrawContext *pContext);
    // Dessin de l'effet de lumière.
    void drawLightEffect (VSTGUI::CDrawContext *pContext);
    // Dessin de l'handle
    void drawHandle (VSTGUI::CDrawContext *pContext);
    
    
    
    // From CControls
    // --------------
    virtual VSTGUI::CMouseEventResult onMouseDown (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons);
    virtual VSTGUI::CMouseEventResult onMouseUp (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons);
    virtual VSTGUI::CMouseEventResult onMouseMoved (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons);
    
    
    // fonction helpers
    // ----------------
    
    
    
    
    
protected:
    
    // variables de dimensionnement
    double safepadding;
    double roundRectRadius;
    
    // rectangle de dessin
    double ecartementInterieur;
    VSTGUI::CRect backgroundRect;
    VSTGUI::CRect gradRect;
    VSTGUI::CRect slideShadowRect;
    VSTGUI::CRect handleRect;
    
    VSTGUI::CRect symbolOnRect;
    VSTGUI::CRect symbolOffRect;
    
    
};


#endif /* defined(__HeyMate__OnOffButton__) */
