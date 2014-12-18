//
//  SwitchHorizontal.cpp
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 03/11/2014.
//
//

#include "SwitchHorizontal.h"
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
// HESwitchHorizontal Implémentation
// ----------------------------

//------------------------------------------------------------------------
HESwitchHorizontal::HESwitchHorizontal (VSTGUI::CRect &frameRect, CControlListener * listener, int tag, int nbrPosition, bool _isRound)
: safepadding(frameRect.getHeight()/20)
, ecartementInterieur(3)
, roundRectRadius(5)
, nbrOfChoices(nbrPosition)
, isRound(_isRound)
, CHorizontalSwitch (frameRect,
                   listener,
                   tag,
                   nbrPosition,
                   0,
                   nbrPosition,
                   new CBitmap(getWidth(),getHeight())
                   )
{
    // par sécurité
    getBackground()->forget();
    setBackground(new CBitmap(getWidth(),getHeight()));
    heightOfOneImage = getWidth();
    
    // réajustement des valeurs
    if (isRound)
    {
        ecartementInterieur++;
        roundRectRadius = 8;
    }
    
    /** Par Default avec un image
     Le switch prend la taille du bouton et la divise par le nombre de sous image donnée
     (nombre du position du switch).
     Il crée comme cela les zones de clics
     L'image est translaté en fonction de la position de clics sans tenir compte de sa taille.
     
     donc heightOfOneImage est divisé verticalement par le nombre de position
     pour avoir les zones clicables
     Dans notre cas, sans bitmap, il faut donc que height of one image soit la taille du switch
     */
    
    backgroundRect = graphicHelp::makeCRect(getWidth()/2,
                                            getHeight()/2,
                                            getWidth() - 2*safepadding,
                                            getHeight() - 2*safepadding);
    // on décale le background pour qu'il soit à l'intérieur du control
    backgroundRect.offset(getViewSize().left, getViewSize().top);
    
}

//------------------------------------------------------------------------
HESwitchHorizontal::~HESwitchHorizontal()
{
    CHorizontalSwitch::~CHorizontalSwitch();
}




//------------------------------------------------------------------------
// Tracking
// --------

//------------------------------------------------------------------------
VSTGUI::CMouseEventResult HESwitchHorizontal::onMouseDown (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons)
{
    return VSTGUI::CHorizontalSwitch::onMouseDown(where, buttons);
}

//------------------------------------------------------------------------
VSTGUI::CMouseEventResult HESwitchHorizontal::onMouseMoved (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons)
{
    return VSTGUI::CHorizontalSwitch::onMouseMoved(where, buttons);
}

//------------------------------------------------------------------------
VSTGUI::CMouseEventResult HESwitchHorizontal::onMouseUp (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons)
{
    return VSTGUI::CHorizontalSwitch::onMouseUp(where, buttons);
}









//------------------------------------------------------------------------
// Dessin du bouton
// ----------------

//------------------------------------------------------------------------
void HESwitchHorizontal::draw (VSTGUI::CDrawContext *pContext)
{
    CHorizontalSwitch::draw(pContext);
    
    // Dessin du backGround
    // --------------------
    drawBackground(pContext);
    
    // dessin du contour du bouton (à enlever en fin de construction)
    // ---------------------------
    //drawViewStrokes(pContext);
    
    // Dessin de l'handle
    // ------------------
    HESwitchHorizontal::drawHandle(pContext);
    
    // fin du dessin
    // -------------
    setDirty (false);
}




//------------------------------------------------------------------------
void HESwitchHorizontal::drawViewStrokes (VSTGUI::CDrawContext * pContext)
{
    pContext->setDrawMode(VSTGUI::kAntiAliasing);
    // début du dessin
    pContext->setLineStyle(VSTGUI::CLineStyle()); // paramètres par defaut conviennent
    pContext->setLineWidth(1);
    pContext->setFrameColor(white);
    pContext->drawRect(getViewSize(), VSTGUI::kDrawStroked); // dessin du contour
}


//------------------------------------------------------------------------
void HESwitchHorizontal::drawBackground (VSTGUI::CDrawContext * pContext)
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
void HESwitchHorizontal::drawHandle (VSTGUI::CDrawContext * pContext)
{
    double choiceZone = getWidth()-2*safepadding; // = zone de background
    double choiceWidth = choiceZone/(nbrOfChoices) - 3; // la zone de background est divié en nombre de zone de position
    double ecartementEntrePosition = (choiceZone - nbrOfChoices*choiceWidth)/(nbrOfChoices+1);
    
    // Construction des rectangles de dessin
    // -------------------------------------
    // position initiale du knob.
    // on décale l'offset lorsque le bouton s'enclenche.
    
    if (isRound) {
        gradRect = graphicHelp::makeCRect(choiceWidth/2 + safepadding + ecartementEntrePosition,
                                          getHeight()/2,
                                          getHeight() - 2*safepadding - 3*ecartementInterieur,
                                          getHeight() - 2*safepadding - 3*ecartementInterieur);
        gradRect.offset(getViewSize().left, getViewSize().top);
        
        slideShadowRect = graphicHelp::makeCRect(choiceWidth/2 + safepadding + ecartementEntrePosition,
                                                 getHeight()/2,
                                                 getHeight() - 2*safepadding - ecartementInterieur,
                                                 getHeight() - 2*safepadding - ecartementInterieur) ;
        slideShadowRect.offset(getViewSize().left, getViewSize().top);
        
        // l'handle se déplace au changment de valeur
        handleRect = graphicHelp::makeCRect(choiceWidth/2 + safepadding + ecartementEntrePosition,
                                            getHeight()/2,
                                            getHeight() - 2*safepadding - 5*ecartementInterieur,
                                            getHeight() - 2*safepadding - 5*ecartementInterieur);
        handleRect.offset(getViewSize().left, getViewSize().top);
    }
    else
    {
        
        gradRect = graphicHelp::makeCRect(choiceWidth/2 + safepadding + ecartementEntrePosition,
                                          getHeight()/2,
                                          choiceWidth-2*ecartementInterieur,
                                          getHeight() - 2*safepadding - 2*ecartementInterieur
                                          );
        gradRect.offset(getViewSize().left, getViewSize().top);
        
        slideShadowRect = graphicHelp::makeCRect(choiceWidth/2 + safepadding + ecartementEntrePosition,
                                                 getHeight()/2,
                                                 choiceWidth,
                                                 getHeight() - 2*safepadding
                                                 );
        slideShadowRect.offset(getViewSize().left, getViewSize().top);
        
        // l'handle se déplace au changment de valeur
        handleRect = graphicHelp::makeCRect(choiceWidth/2 + safepadding + ecartementEntrePosition,
                                            getHeight()/2,
                                            choiceWidth - 4*ecartementInterieur,
                                            getHeight() - 2*safepadding - 4*ecartementInterieur
                                            );
        handleRect.offset(getViewSize().left, getViewSize().top);
    }
    
    
    
    
    
    // récupération de la valeur du knob
    float normValue = getValueNormalized ();
    
    // dessin pour un nombre de position donné
    for (int i = 0; i < nbrOfChoices; i++)
    {
        // On dessine le gradRect
        // en contour blanc, si celui ci n'est pas à la position choisi
        // en dégradé si celui ci est choisi avec la shadow et l'handle
        if (floor(normValue*100) == floor(100*i/(nbrOfChoices-1)))
        {
            // cas où la position est sélectionnée
            // -----------------------------------
            // création de l'effet d'ombre
            // ---------------------------
            CGraphicsPath * lightPath = pContext->createGraphicsPath();
            if (isRound)
                lightPath->addEllipse(slideShadowRect);
            else
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
            
            
            
            // On dessine
            // ----------
            
            // dessin du handle à la bonne position
            // -------------------------------------------
            pContext->setDrawMode(VSTGUI::kAntiAliasing);
            // début du dessin
            CGraphicsPath * roundHandle = pContext->createGraphicsPath();
            if (isRound)
                roundHandle->addEllipse(handleRect);
            else
                roundHandle->addRoundRect(handleRect, roundRectRadius);
            
            pContext->setFillColor(whiteHandle);
            pContext->drawGraphicsPath(roundHandle, CDrawContext::kPathFilled);
            
            
            
        }
        else
        {
            // cas ou la position est déselectionnée
            // création du contour de l'handle
            // -------------------------------
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
        
        
        // on décale les rectangles
        // ------------------------
        double decalageVertical = choiceWidth + ecartementEntrePosition;
        
        gradRect.offset(decalageVertical, 0);
        slideShadowRect.offset(decalageVertical, 0);
        handleRect.offset(decalageVertical, 0);
        
    }
    
    
    
    
    setDirty (false);
    
    
}




