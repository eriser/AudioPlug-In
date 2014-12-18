//
//  KickButton.h
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 02/11/2014.
//
//

#ifndef __HeyMate__KickButton__
#define __HeyMate__KickButton__

// Paramètres du boutton
// ---------------------
#define KICK_HEIGHT            40 // largeur normale du slider
#define KICK_WIDTH             40 // longueur normale


#include "lib/controls/cbuttons.h"


class HEKickButton : public VSTGUI::CKickButton
{
public:
    HEKickButton (VSTGUI::CRect &frameRect, CControlListener * listener, int tag, bool _isRound = true);
    ~HEKickButton ();
    
    
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
    
    // bouton ronnd ou carré?
    bool isRound;
    
    // variables de dimensionnement
    double safepadding;
    double roundRectRadius;
    
    // rectangle de dessin
    double ecartementInterieur;
    VSTGUI::CRect backgroundRect;
    VSTGUI::CRect gradRect;
    VSTGUI::CRect shadowRect;
    VSTGUI::CRect handleRect;
    
    VSTGUI::CRect symbolRect;
    
};


#endif /* defined(__HeyMate__KickButton__) */
