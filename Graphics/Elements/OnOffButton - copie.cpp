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
    
    // l'handle se déplace au changment de valeur
    
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
        VSTGUI::CPoint * lightEndPoint = new VSTGUI::CPoint(slideShadowRect.left, slideShadowRect.bottom);
        
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
        VSTGUI::CPoint * gradEndPoint = new VSTGUI::CPoint(gradRect.left, gradRect.bottom);
        
        pContext->setDrawMode(kAntialias);
        pContext->fillLinearGradient(gradPath, *gradient, *gradStartPoint, *gradEndPoint, true);
        
        gradPath->forget();
        
        // postionnement de l'handle
        double handleWidth = getWidth()/1.5 - 2*safepadding - 2*ecartementInterieur;
        handleRect = graphicHelp::makeCRect(getViewSize().left + getWidth()/2,
                                            getViewSize().top + getHeight()/2,
                                            handleWidth,
                                            getHeight()- 2*safepadding - 2*ecartementInterieur);
        
        
    }
    // état du bouton Off
    else
    {
        // postionnement de l'handle
        double handleWidth = getWidth()/1.5 - 2*safepadding - 2*ecartementInterieur;
        handleRect = graphicHelp::makeCRect(gradRect.left + handleWidth/2,
                                            getViewSize().top + getHeight()/2,
                                            handleWidth,
                                            getHeight()- 2*safepadding - 2*ecartementInterieur);
        
    }
    
    
    // dessin du handle à la bonne position
    // -------------------------------------------
    pContext->setDrawMode(VSTGUI::kAntiAliasing);
    // début du dessin
    CGraphicsPath * roundHandle = pContext->createGraphicsPath();
    roundHandle->addRoundRect(handleRect, roundRectRadius);
    
    pContext->setFillColor(whiteHandle);
    pContext->drawGraphicsPath(roundHandle, CDrawContext::kPathFilled);
    
    
    
    // dessin de symbol sur l'handle... (fait à l'arrache en 2 min.)
    // --------------------------------
    pContext->setDrawMode(VSTGUI::kAntialias);
    pContext->setFillColor(MakeCColor(0,0,0,100));
    if (normValue == 1) {
        CRect onRect = graphicHelp::makeCRect(getWidth()/2, getHeight()/2, 3, handleRect.height() - 10);
        onRect.offset(getViewSize().left, getViewSize().top);
        pContext->drawRect(onRect, kDrawFilled);
    }
    else
    {
        CRect offRect = graphicHelp::makeCRect(gradRect.left + (getWidth()/1.5 - 2*safepadding - 2*ecartementInterieur)/2,
                                               getViewSize().top + getHeight()/2,
                                               handleRect.width() - 10, handleRect.width() - 10);
        pContext->setLineWidth(2);
        pContext->drawEllipse(offRect, kDrawStroked);
                                               
    }
    
    
    
    setDirty (false);
    
    
}









