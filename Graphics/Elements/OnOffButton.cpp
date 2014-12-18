//
//  OnOffButton.cpp
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 02/11/2014.
//
//

#include "OnOffButton.h"
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
// HEOnOffButton Implémentation
// ----------------------------

//------------------------------------------------------------------------
HEOnOffButton::HEOnOffButton (VSTGUI::CRect &frameRect, CControlListener * listener, int tag, int32_t style)
: safepadding(frameRect.getWidth()/20)
, ecartementInterieur(4)
, roundRectRadius(8)
, COnOffButton (frameRect, listener, tag, NULL, style)
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
    
    slideShadowRect = graphicHelp::makeCRectFromTopLeft(safepadding,
                                                        safepadding,
                                                        getWidth() - 2*safepadding,
                                                        getHeight()- 2*safepadding);
    slideShadowRect.offset(getViewSize().left, getViewSize().top);
    
    // le symbole sur l'handle change avec la valeur
    handleRect = graphicHelp::makeCRect(getWidth()/2,
                                        getHeight()/2,
                                        getWidth() - 2*safepadding - 4*ecartementInterieur,
                                        getHeight()- 2*safepadding - 4*ecartementInterieur);
    handleRect.offset(getViewSize().left, getViewSize().top);
    
    
    symbolOffRect = graphicHelp::makeCRect(getWidth()/2,
                                           getHeight()/2,
                                           handleRect.width() - 3.5*ecartementInterieur,
                                           handleRect.width() - 3.5*ecartementInterieur);
    symbolOffRect.offset(getViewSize().left, getViewSize().top);
    
    symbolOnRect = graphicHelp::makeCRect(getWidth()/2,
                                          getHeight()/2,
                                          3,
                                          handleRect.height() - 3*ecartementInterieur);
    symbolOnRect.offset(getViewSize().left, getViewSize().top);
    
    
    
}

//------------------------------------------------------------------------
HEOnOffButton::~HEOnOffButton()
{
    COnOffButton::~COnOffButton();
}




//------------------------------------------------------------------------
// Tracking
// --------

//------------------------------------------------------------------------
VSTGUI::CMouseEventResult HEOnOffButton::onMouseDown (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons)
{
    return VSTGUI::COnOffButton::onMouseDown(where, buttons);
}

//------------------------------------------------------------------------
VSTGUI::CMouseEventResult HEOnOffButton::onMouseMoved (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons)
{
    return VSTGUI::COnOffButton::onMouseMoved(where, buttons);
}

//------------------------------------------------------------------------
VSTGUI::CMouseEventResult HEOnOffButton::onMouseUp (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons)
{
    return VSTGUI::COnOffButton::onMouseUp(where, buttons);
}









//------------------------------------------------------------------------
// Dessin du bouton
// ----------------

//------------------------------------------------------------------------
void HEOnOffButton::draw (VSTGUI::CDrawContext *pContext)
{
    
    // Dessin du backGround
    // --------------------
    drawBackground(pContext);
    
    // dessin du contour du bouton (à enlever en fin de construction)
    // ---------------------------
    //drawViewStrokes(pContext);
    
    // Dessin de l'handle
    // ------------------
    HEOnOffButton::drawHandle(pContext);
    
    // fin du dessin
    // -------------
    setDirty (false);
}




//------------------------------------------------------------------------
void HEOnOffButton::drawViewStrokes (VSTGUI::CDrawContext * pContext)
{
    pContext->setDrawMode(VSTGUI::kAntiAliasing);
    // début du dessin
    pContext->setLineStyle(VSTGUI::CLineStyle()); // paramètres par defaut conviennent
    pContext->setLineWidth(1);
    pContext->setFrameColor(white);
    pContext->drawRect(getViewSize(), VSTGUI::kDrawStroked); // dessin du contour
}


//------------------------------------------------------------------------
void HEOnOffButton::drawBackground (VSTGUI::CDrawContext * pContext)
{
    pContext->setDrawMode(VSTGUI::kAntiAliasing);
    // début du dessin
    CGraphicsPath * roundBackground = pContext->createGraphicsPath();
    roundBackground->addRoundRect(backgroundRect, roundRectRadius);
    
    pContext->setFillColor(black);
    pContext->drawGraphicsPath(roundBackground, CDrawContext::kPathFilled);
    pContext->setLineWidth(0.75);
    pContext->setFrameColor(controlBorder);
    pContext->drawGraphicsPath(roundBackground, CDrawContext::kPathStroked);
    
    roundBackground->forget();
}



//------------------------------------------------------------------------
void HEOnOffButton::drawHandle (VSTGUI::CDrawContext * pContext)
{
    // récupération de la valeur du knob
    float normValue = getValueNormalized ();
    
    // état du bouton On
    if (normValue == 1)
    {
        // création de l'effet d'ombre
        // ---------------------------
        CGraphicsPath * lightPath = pContext->createGraphicsPath();
        lightPath->addRoundRect(slideShadowRect, roundRectRadius);
        
        VSTGUI::CColor startLightColor = lightBlue;
        VSTGUI::CColor endLightColor = SeaFoamGreen;
        startLightColor.alpha = 255/3;
        endLightColor.alpha = 255/3;
        VSTGUI::CGradient * lightGradient = lightPath->createGradient(0, 1, startLightColor, endLightColor);
        
        VSTGUI::CPoint * lightStartPoint = new VSTGUI::CPoint(slideShadowRect.right, slideShadowRect.bottom);
        VSTGUI::CPoint * lightEndPoint = new VSTGUI::CPoint(slideShadowRect.right, slideShadowRect.top);
        
        pContext->setDrawMode(kAntialias);
        pContext->fillLinearGradient(lightPath, *lightGradient, *lightStartPoint, *lightEndPoint, true);
        
        lightPath->forget();
        
        
        // création du gradient
        // --------------------
        VSTGUI::CGraphicsPath * gradPath = pContext->createGraphicsPath();
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
    CGraphicsPath * roundHandle = pContext->createGraphicsPath();
    roundHandle->addRoundRect(handleRect, roundRectRadius);
    
    pContext->setFillColor(whiteHandle);
    pContext->drawGraphicsPath(roundHandle, CDrawContext::kPathFilled);
    
    
    
    // dessin de symbol sur l'handle
    // -----------------------------
    pContext->setDrawMode(VSTGUI::kAntialias);
    pContext->setFillColor(MakeCColor(0,0,0,100));
    if (normValue == 1)
    {
        pContext->drawRect(symbolOnRect, kDrawFilled);
    }
    else
    {
        pContext->setLineWidth(2);
        pContext->drawEllipse(symbolOffRect, kDrawStroked);
    }
    
    
    
    setDirty (false);
    
    
}









