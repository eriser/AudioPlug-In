//
//  SwitchHorizontal.h
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 03/11/2014.
//
//

#ifndef __HeyMate__SwitchHorizontal__
#define __HeyMate__SwitchHorizontal__


// Paramètres du boutton
// ---------------------
#define HORIZONTAL_SWITCH_HEIGHT                             35 // Hauteur de une position
#define HORIZONTAL_SWITCH_WIDTH_FOR_ONE_POSITION             40 // longueur normale


#include "lib/controls/cswitch.h"


class HESwitchHorizontal : public VSTGUI::CHorizontalSwitch
{
public:
    HESwitchHorizontal (VSTGUI::CRect &frameRect, CControlListener * listener, int tag, int nbrPosition, bool isRound = true);
    ~HESwitchHorizontal ();
    
    
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
    
    // nombre de position (accès aussi possible par getSubPixmap)
    int nbrOfChoices;
    bool isRound;
    
    // variables de dimensionnement
    double safepadding;
    double roundRectRadius;
    
    // rectangle de dessin
    double ecartementInterieur;
    
    VSTGUI::CRect backgroundRect;
    
    // tableau pour chaque position
    VSTGUI::CRect gradRect; // redéssiné pour chaque position
    VSTGUI::CRect slideShadowRect; // à décalé (en même temps que le knob)
    VSTGUI::CRect handleRect; // à décalé
    
    
    
};


#endif /* defined(__HeyMate__SwitchHorizontal__) */
