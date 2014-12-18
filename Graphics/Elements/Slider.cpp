//
//  Slider.cpp
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 30/10/2014.
//
//

#include "Slider.h"
#include "Editor.h"

#include "lib/cframe.h"
#include "lib/cdrawcontext.h"

#include "../General/Colors.h"
#include "../General/GraphicHelp.h"

//------------------------------------------------------------------------
// fonction d'aide au calcul
// -------------------------

//------------------------------------------------------------------------
// Detection de l'orientation du slider suivant le rectangle passé en argument
int32_t detectStyle (CRect &rect, bool isInverse)
{
    int res = 0;
    if (rect.height() > rect.width())
    {
        if (isInverse)
            res = res | (kVertical|kTop);
        else
            res = res | (kVertical|kBottom);
    }
    else
    {
        if (isInverse)
            res = res | (kHorizontal|kRight);
        else
            res = res | (kHorizontal|kLeft);
    }
    return res;
}







//------------------------------------------------------------------------
// HESlider Implémentation
// -----------------------

//------------------------------------------------------------------------
HESlider::HESlider (VSTGUI::CRect &frameRect, CControlListener * listener, int32_t tag, bool styleInverse)
: ecartementInterieur(2)
, VSTGUI::CSlider(frameRect,
                  listener,
                  tag,
                  *new CPoint (0, 0),
                  frameRect.getHeight(),
                  NULL,
                  NULL,
                  CPoint (0, 0),
                  detectStyle(frameRect, styleInverse))
{
    if (style & kHorizontal)
    {
        
        // l'handle peux théoriquement se déplacer dans tout le control
        rangeHandle = frameRect.getWidth();
        safepadding = getHeight()/20;
        handleWidth = 5;
        handleHeight = getHeight()-safepadding;
    }
    else
    {
        
        // l'handle peux théoriquement se déplacer dans tout le control
        rangeHandle = frameRect.getHeight();
        safepadding = getWidth()/20;
        handleWidth = getWidth()-safepadding;
        handleHeight = 5;
    }
    
    
    backgroundRect = graphicHelp::makeCRect(getWidth()/2,
                                            getHeight()/2,
                                            getWidth() - safepadding,
                                            getHeight() - safepadding);
    // on décale le background pour qu'il soit à l'intérieur du control
    backgroundRect.offset(getViewSize().left, getViewSize().top);
    
    
    setTransparency(true);
    
    
    

    
}


//------------------------------------------------------------------------
HESlider::~HESlider()
{
    VSTGUI::CSlider::~CSlider();
}







//------------------------------------------------------------------------
// Tracking
// --------

//------------------------------------------------------------------------
VSTGUI::CMouseEventResult HESlider::onMouseDown (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons)
{
    return VSTGUI::CSlider::onMouseDown(where, buttons);
}

//------------------------------------------------------------------------
VSTGUI::CMouseEventResult HESlider::onMouseMoved (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons)
{
    return VSTGUI::CSlider::onMouseMoved(where, buttons);
}

//------------------------------------------------------------------------
VSTGUI::CMouseEventResult HESlider::onMouseUp (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons)
{
    return VSTGUI::CSlider::onMouseUp(where, buttons);
}








//------------------------------------------------------------------------
// Dessin du bouton
// ----------------

//------------------------------------------------------------------------
void HESlider::draw (VSTGUI::CDrawContext *pContext)
{
    
    // Dessin du backGround
    // --------------------
    drawBackground(pContext);
    
    // dessin du contour du bouton (à enlever en fin de construction)
    // ---------------------------
    //drawViewStrokes(pContext);
    
    // Dessin de l'handle
    // ------------------
    HESlider::drawHandle(pContext);
    
    // fin du dessin
    // -------------
    setDirty (false);
}




//------------------------------------------------------------------------
void HESlider::drawViewStrokes (VSTGUI::CDrawContext * pContext)
{
    pContext->setDrawMode(VSTGUI::kAntiAliasing);
    // début du dessin
    pContext->setLineStyle(VSTGUI::CLineStyle()); // paramètres par defaut conviennent
    pContext->setLineWidth(1);
    pContext->setFrameColor(white);
    pContext->drawRect(getViewSize(), VSTGUI::kDrawStroked); // dessin du contour
    
}


//------------------------------------------------------------------------
void HESlider::drawBackground (VSTGUI::CDrawContext * pContext)
{
    pContext->setDrawMode(VSTGUI::kAntiAliasing);
    // début du dessin
    pContext->setFillColor(black);
    pContext->drawRect(backgroundRect , VSTGUI::kDrawFilled);
    pContext->setLineWidth(0.75);
    pContext->setFrameColor(controlBorder);
    pContext->drawRect(backgroundRect, VSTGUI::kDrawStroked);
    
}



//------------------------------------------------------------------------
void HESlider::drawHandle (VSTGUI::CDrawContext * pContext)
{
    // récupération de la valeur du knob
    float normValue = getValueNormalized ();
    
    
    // Création des rectangles qui serviront au dessin
    // -----------------------------------------------
    VSTGUI::CPoint * gradStartPoint;
    VSTGUI::CPoint * gradEndPoint;
    
    // calcul des nouvelles coordonée de l'handle dans les deux cas, horizontal et vertical
    if (style & kHorizontal)
    {
        // rectangle de l'handle
        // ---------------------
        double centerXHandle = normValue*(rangeHandle - 2*safepadding - handleWidth) + safepadding + handleWidth/2;
        if (style & kRight)
            centerXHandle = getWidth() - centerXHandle;
        handleRect = graphicHelp::makeCRect(centerXHandle , getHeight()/2, handleWidth, handleHeight);
        // on décale le rectangle pour qu'il soit dans le control.
        handleRect.offset (getViewSize ().left, getViewSize ().top);
        
        // rectangle du gradient fixe
        // --------------------------
        gradRect = graphicHelp::makeCRectFromTopLeft(safepadding + ecartementInterieur,
                                                     safepadding + ecartementInterieur,
                                                     getWidth() - 2*safepadding - 2*ecartementInterieur,
                                                     getHeight() - 2*safepadding - 2*ecartementInterieur);
        
        // on décale le rectangle pour qu'il soit dans le control.
        gradRect.offset (getViewSize ().left, getViewSize ().top);
        
        if (style & kRight)
        {
            gradStartPoint = new VSTGUI::CPoint(gradRect.right, gradRect.bottom);
            gradEndPoint = new VSTGUI::CPoint(gradRect.left, gradRect.bottom);
        }
        else
        {
            gradStartPoint = new VSTGUI::CPoint(gradRect.left, gradRect.bottom);
            gradEndPoint = new VSTGUI::CPoint(gradRect.right, gradRect.bottom);
        }
        
        // rectangle du gradient slider
        // ----------------------------
        slideGradRect.bottom = gradRect.bottom;
        slideGradRect.top = gradRect.top;
        
        if (style & kRight)
        {
            slideGradRect.right = gradRect.right;
            slideGradRect.left = (handleRect.left + handleRect.right)/2;
        }
        else
        {
            slideGradRect.left = gradRect.left;
            slideGradRect.right = (handleRect.left + handleRect.right)/2;
        }
        
        
        // rectangle de l'omnbre lumineuse
        // -------------------------------
        slideShadowRect.left = slideGradRect.left;
        slideShadowRect.right = slideGradRect.right;
        slideShadowRect.top = backgroundRect.top;
        slideShadowRect.bottom = backgroundRect.bottom;
        // rectangle de l'omnbre lumineuse
        // -------------------------------
        slideShadowRect.top = backgroundRect.top + 0.5;
        slideShadowRect.bottom = backgroundRect.bottom - 0.5;
        
        if (style & kRight)
        {
            slideShadowRect.right = backgroundRect.right - 0.5;
            slideShadowRect.left = slideGradRect.left;
        }
        else
        {
            slideShadowRect.right = slideGradRect.right;
            slideShadowRect.left = backgroundRect.left + 0.5;
        }
        
    }
    else
    {
        // rectangle de l'handle
        // ---------------------
        double centerYHandle = normValue*(rangeHandle - 2*safepadding - handleHeight) + safepadding + handleHeight/2 ;
        if (style & kBottom)
            centerYHandle = getHeight() - centerYHandle;
        // dans l'autre cas, on part de l'origine, pas besoin de rajouter une offset.
        
        handleRect = graphicHelp::makeCRect(getWidth()/2, centerYHandle, handleWidth, handleHeight);
        // on décale le rectangle pour qu'il soit dans le control.
        handleRect.offset (getViewSize ().left, getViewSize ().top);
        
        // rectangle du gradient fixe
        // --------------------------
        gradRect = graphicHelp::makeCRectFromTopLeft(safepadding + ecartementInterieur,
                                                     safepadding + ecartementInterieur,
                                                     getWidth() - 2*safepadding - 2*ecartementInterieur,
                                                     getHeight() - 2*safepadding - 2*ecartementInterieur);
        
        // on décale le rectangle pour qu'il soit dans le control.
        gradRect.offset (getViewSize ().left, getViewSize ().top);
        
        if (style & kBottom)
        {
            gradStartPoint = new VSTGUI::CPoint(gradRect.left, gradRect.bottom);
            gradEndPoint = new VSTGUI::CPoint(gradRect.left, gradRect.top);
        }
        else
        {
            gradStartPoint = new VSTGUI::CPoint(gradRect.left, gradRect.top);
            gradEndPoint = new VSTGUI::CPoint(gradRect.left, gradRect.bottom);
        }
        
        // rectangle du gradient slider
        // ----------------------------
        slideGradRect.left = gradRect.left;
        slideGradRect.right = gradRect.right;
        
        if (style & kBottom)
        {
            slideGradRect.bottom = gradRect.bottom;
            slideGradRect.top = (handleRect.top + handleRect.bottom)/2;
        }
        else
        {
            slideGradRect.top = gradRect.top;
            slideGradRect.bottom = (handleRect.top + handleRect.bottom)/2;
        }
        
        // rectangle de l'omnbre lumineuse
        // -------------------------------
        slideShadowRect.left = backgroundRect.left + 0.5;
        slideShadowRect.right = backgroundRect.right - 0.5;
        
        if (style & kBottom)
        {
            slideShadowRect.bottom = backgroundRect.bottom - 0.5;
            slideShadowRect.top = slideGradRect.top;
        }
        else
        {
            slideShadowRect.bottom = slideGradRect.bottom;
            slideShadowRect.top = backgroundRect.top + 0.5;
        }
        
    }
    
    // création de l'effet d'ombre
    // ---------------------------
    CGraphicsPath * lightPath = pContext->createGraphicsPath();
    lightPath->addRoundRect(slideShadowRect, 1);
    
    VSTGUI::CColor startLightColor = lightBlue;
    VSTGUI::CColor endLightColor = SeaFoamGreen;
    startLightColor.alpha = 255*normValue/3;
    endLightColor.alpha = 255*normValue/3;
    VSTGUI::CGradient * lightGradient = lightPath->createGradient(0, 1, startLightColor, endLightColor);
    
    pContext->setDrawMode(kAntialias);
    pContext->fillLinearGradient(lightPath, *lightGradient, *gradStartPoint, *gradEndPoint, true);
    
    lightPath->forget();
    
    
    // création du gradient
    // --------------------
    VSTGUI::CGraphicsPath * gradPath = pContext->createGraphicsPath();
    gradPath->addRect(slideGradRect);
    
    VSTGUI::CColor startColor = lightBlue;
    VSTGUI::CColor endColor = SeaFoamGreen;
    VSTGUI::CGradient * gradient = gradPath->createGradient(0, 1, startColor, endColor);
    
    pContext->setDrawMode(kAntialias);
    pContext->fillLinearGradient(gradPath, *gradient, *gradStartPoint, *gradEndPoint, true);
    
    gradPath->forget();
    
    
    // dessin du slider à la nouvelle position
    // ---------------------------------------
    pHandle = new CBitmap(handleWidth,handleHeight);
    pContext->setDrawMode(kAntialias);
    pContext->setFillColor(whiteHandle);
    pContext->drawRect(handleRect, VSTGUI::kDrawFilled);
    pHandle->draw (pContext, handleRect);
    
    pHandle->forget();
    
    
    
    setDirty (false);
    
    
}


