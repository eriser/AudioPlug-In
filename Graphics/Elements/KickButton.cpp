//
//  KickButton.cpp
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 02/11/2014.
//
//

#include "KickButton.h"
#include "Editor.h"

#include "lib/cframe.h"
#include "lib/cdrawcontext.h"

#include "../General/Colors.h"
#include "../General/GraphicHelp.h"

//------------------------------------------------------------------------
// fonction d'aide au calcul
// -------------------------

//------------------------------------------------------------------------








//------------------------------------------------------------------------
// HEKickButton Implémentation
// ----------------------------

//------------------------------------------------------------------------
HEKickButton::HEKickButton (VSTGUI::CRect &frameRect, CControlListener * listener, int tag, bool _isRound)
: safepadding(frameRect.getWidth()/20)
, ecartementInterieur(4)
, isRound(_isRound)
, roundRectRadius(8)
, CKickButton (frameRect, listener, tag, NULL)
{
    
    backgroundRect = graphicHelp::makeCRect(getWidth()/2,
                                            getHeight()/2,
                                            getWidth() - 2*safepadding,
                                            getHeight() - 2*safepadding);
    // on décale le background pour qu'il soit à l'intérieur du control
    backgroundRect.offset(getViewSize().left, getViewSize().top);
    
    
    // position initiale du knob.
    // on décale l'offset lorsque le bouton s'enclenche.
    
    gradRect = graphicHelp::makeCRectFromTopLeft(safepadding + ecartementInterieur,
                                                 safepadding + ecartementInterieur,
                                                 getWidth() - 2*safepadding - 2*ecartementInterieur,
                                                 getHeight()- 2*safepadding - 2*ecartementInterieur);
    gradRect.offset(getViewSize().left, getViewSize().top);
    
    shadowRect = graphicHelp::makeCRectFromTopLeft(safepadding,
                                                   safepadding,
                                                   getWidth() - 2*safepadding,
                                                   getHeight()- 2*safepadding);
    shadowRect.offset(getViewSize().left, getViewSize().top);
    
    // le symbole sur l'handle change avec la valeur
    handleRect = graphicHelp::makeCRect(getWidth()/2,
                                        getHeight()/2,
                                        getWidth()- 2*safepadding - 4*ecartementInterieur,
                                        getHeight()- 2*safepadding - 4*ecartementInterieur);
    handleRect.offset(getViewSize().left, getViewSize().top);
}

//------------------------------------------------------------------------
HEKickButton::~HEKickButton()
{
    CKickButton::~CKickButton();
}




//------------------------------------------------------------------------
// Tracking
// --------

//------------------------------------------------------------------------
VSTGUI::CMouseEventResult HEKickButton::onMouseDown (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons)
{
    return VSTGUI::CKickButton::onMouseDown(where, buttons);
}

//------------------------------------------------------------------------
VSTGUI::CMouseEventResult HEKickButton::onMouseMoved (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons)
{
    return VSTGUI::CKickButton::onMouseMoved(where, buttons);
}

//------------------------------------------------------------------------
VSTGUI::CMouseEventResult HEKickButton::onMouseUp (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons)
{
    return VSTGUI::CKickButton::onMouseUp(where, buttons);
}









//------------------------------------------------------------------------
// Dessin du bouton
// ----------------

//------------------------------------------------------------------------
void HEKickButton::draw (VSTGUI::CDrawContext *pContext)
{
    
    // Dessin du backGround
    // --------------------
    drawBackground(pContext);
    
    // dessin du contour du bouton (à enlever en fin de construction)
    // ---------------------------
    //drawViewStrokes(pContext);
    
    // Dessin de l'handle
    // ------------------
    HEKickButton::drawHandle(pContext);
    
    // fin du dessin
    // -------------
    setDirty (false);
}




//------------------------------------------------------------------------
void HEKickButton::drawViewStrokes (VSTGUI::CDrawContext * pContext)
{
    pContext->setDrawMode(VSTGUI::kAntiAliasing);
    // début du dessin
    pContext->setLineStyle(VSTGUI::CLineStyle()); // paramètres par defaut conviennent
    pContext->setLineWidth(1);
    pContext->setFrameColor(white);
    pContext->drawRect(getViewSize(), VSTGUI::kDrawStroked); // dessin du contour
}


//------------------------------------------------------------------------
void HEKickButton::drawBackground (VSTGUI::CDrawContext * pContext)
{
    pContext->setDrawMode(VSTGUI::kAntiAliasing);
    // début du dessin
    CGraphicsPath * roundBackground = pContext->createGraphicsPath();
    if (isRound)
        roundBackground->addEllipse(backgroundRect);
    else
        roundBackground->addRoundRect(backgroundRect, roundRectRadius);
    
    pContext->setFillColor(black);
    pContext->drawGraphicsPath(roundBackground, CDrawContext::kPathFilled);
    pContext->drawEllipse(backgroundRect, VSTGUI::kDrawFilled);
    pContext->setLineWidth(0.75);
    pContext->setFrameColor(controlBorder);
    pContext->drawGraphicsPath(roundBackground, CDrawContext::kPathStroked);
    
    roundBackground->forget();
    
    
    
}



//------------------------------------------------------------------------
void HEKickButton::drawHandle (VSTGUI::CDrawContext * pContext)
{
    // récupération de la valeur du knob
    float normValue = getValueNormalized ();
    
    // état du bouton On
    if (normValue == 1)
    {
        // création de l'effet d'ombre
        // ---------------------------
        CGraphicsPath * lightPath = pContext->createGraphicsPath();
        if (isRound)
            lightPath->addEllipse(shadowRect);
        else
            lightPath->addRoundRect(shadowRect, roundRectRadius);
        
        VSTGUI::CColor startLightColor = lightBlue;
        VSTGUI::CColor endLightColor = SeaFoamGreen;
        startLightColor.alpha = 255/3;
        endLightColor.alpha = 255/3;
        VSTGUI::CGradient * lightGradient = lightPath->createGradient(0, 1, startLightColor, endLightColor);
        
        VSTGUI::CPoint * lightStartPoint = new VSTGUI::CPoint(shadowRect.right, shadowRect.bottom);
        VSTGUI::CPoint * lightEndPoint = new VSTGUI::CPoint(shadowRect.right, shadowRect.top);
        
        pContext->setDrawMode(kAntialias);
        pContext->fillLinearGradient(lightPath, *lightGradient, *lightStartPoint, *lightEndPoint, true);
        
        lightPath->forget();
        
        
        // création du gradient
        // --------------------
        VSTGUI::CGraphicsPath * gradPath = pContext->createGraphicsPath();
        if (isRound)
            gradPath->addEllipse(gradRect);
        else
            gradPath->addRoundRect(gradRect, roundRectRadius);
        
        VSTGUI::CColor startColor = lightBlue;
        VSTGUI::CColor endColor = SeaFoamGreen;
        VSTGUI::CGradient * gradient = gradPath->createGradient(0, 1, startColor, endColor);
        
        VSTGUI::CPoint * gradStartPoint = new VSTGUI::CPoint(gradRect.right, gradRect.bottom);
        VSTGUI::CPoint * gradEndPoint = new VSTGUI::CPoint(gradRect.right, gradRect.top);
        
        pContext->setDrawMode(kAntialias);
        pContext->fillLinearGradient(gradPath, *gradient, *gradStartPoint, *gradEndPoint, true);
        
        gradPath->forget();
        
        
    }
    // état du bouton Off
    else
    {
        // création du gradient
        // --------------------
        VSTGUI::CGraphicsPath * emptyPath = pContext->createGraphicsPath();
        if (isRound)
            emptyPath->addEllipse(gradRect);
        else
            emptyPath->addRoundRect(gradRect, roundRectRadius);
        
        pContext->setDrawMode(kAntialias);
        pContext->setLineWidth(1);
        CColor colorWhenEmpty = controlBorder;
        colorWhenEmpty.alpha = 150;
        pContext->setFrameColor(colorWhenEmpty);
        pContext->drawGraphicsPath(emptyPath, CDrawContext::kPathStroked);
        
        emptyPath->forget();
        
    }
    
    // dessin du handle
    // ----------------
    pContext->setDrawMode(VSTGUI::kAntiAliasing);
    // début du dessin
    VSTGUI::CGraphicsPath * handlePath = pContext->createGraphicsPath();
    if (isRound)
        handlePath->addEllipse(handleRect);
    else
        handlePath->addRoundRect(handleRect, roundRectRadius);
    
    pContext->setFillColor(whiteHandle);
    pContext->drawGraphicsPath(handlePath, CDrawContext::kPathFilled);
    
    handlePath->forget();
    
    setDirty (false);
    
    
}









