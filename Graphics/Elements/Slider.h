//
//  Slider.h
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 30/10/2014.
//
//

#ifndef __HeyMate__Slider__
#define __HeyMate__Slider__

// Paramètres des sliders
// ----------------------
#define SLIDER_WIDTHNESS            15 // largeur normale du slider
#define SLIDER_LONG                 200 // longueur normale

#include "lib/controls/cslider.h"


class HESlider : public VSTGUI::CSlider
{
public:
    HESlider (VSTGUI::CRect &frameRect, CControlListener * listener, int32_t tag, bool isInverse = false);
    ~HESlider();
    
    
    // dessin du slider
    // ----------------
    virtual void draw (VSTGUI::CDrawContext * pContext);
    // Dessin du backGround
    void drawBackground (VSTGUI::CDrawContext * pContext);
    // dessin des contours du bouton, uniquement pour la construction
    void drawViewStrokes (VSTGUI::CDrawContext *pContext);
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
    double handleWidth;
    double handleHeight;
    
    // rectangle de dessin
    double ecartementInterieur;
    VSTGUI::CRect backgroundRect;
    VSTGUI::CRect gradRect;
    VSTGUI::CRect slideGradRect;
    VSTGUI::CRect slideShadowRect;
    VSTGUI::CRect handleRect;
    
    
};



#endif /* defined(__HeyMate__Slider__) */
