//
//  Knob.cpp
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 08/10/2014.
//
//

#include "Knob.h"
#include "Editor.h"

#include "lib/cframe.h"
#include "lib/cdrawcontext.h"

#include "../General/Colors.h"
#include "../General/GraphicHelp.h"

#include "TextLabel.h"

/** Helper Functions **/

//------------------------------------------------------------------------
// fonction d'aide au calcul
// -------------------------

//------------------------------------------------------------------------
// convertion d'angle
#define ToRad(deg) 		( (M_PI * (deg)) / 180.0 )
#define ToDeg(rad)		( (180.0 * (rad)) / M_PI )
#define SQR(x)			( (x) * (x) )





//------------------------------------------------------------------------
// HEKnob Implémentation
// -------------------------

//------------------------------------------------------------------------
//------------------------------------------------------------------------
HEKnob::HEKnob(VSTGUI::CRect &frameRect, CControlListener * listener, int tag)
: backgroundWidth(frameRect.getWidth()/4)
, safepadding(frameRect.getWidth()/20)
, handleWidth(2)
, VSTGUI::CKnob(frameRect,
                listener,
                tag,
                new VSTGUI::CBitmap(frameRect.getWidth(), frameRect.getHeight()),
                NULL // default, trait blanc...
                )

{
    // Angles du knob.
    setStartAngle(90);
    setRangeAngle(360);
    
    setMin(0);
    setMax(100);
    inset = 0;
    
    // construction des rectangle de dessin
    // ------------------------------------
    // note : on rentre un peux espacer les couronnes par rapport au contour strict du background
    ecartementInterieur = 4.5;
    
    coronaBackground = graphicHelp::makeCRectFromCenterPoint(getViewSize().left + getWidth()/2,
                                                             getViewSize().top + getHeight()/2,
                                                             getWidth() - 2*safepadding - backgroundWidth,
                                                             getHeight() - 2*safepadding - backgroundWidth);
    
    coronaExteriorShadow = graphicHelp::makeCRectFromCenterPoint(getViewSize().left + getWidth()/2,
                                                                 getViewSize().top + getHeight()/2,
                                                                 getWidth() - 2*safepadding,
                                                                 getHeight() - 2*safepadding);
    
    coronaInteriorShadow = graphicHelp::makeCRectFromCenterPoint(getViewSize().left + getWidth()/2,
                                                                 getViewSize().top + getHeight()/2,
                                                                 getWidth() - 2*safepadding - 2*backgroundWidth,
                                                                 getHeight() - 2*safepadding - 2*backgroundWidth);
    
    coronaExterior = graphicHelp::makeCRectFromCenterPoint(getViewSize().left + getWidth()/2,
                                                           getViewSize().top + getHeight()/2,
                                                           getWidth() - 2*safepadding - ecartementInterieur,
                                                           getHeight() - 2*safepadding - ecartementInterieur);
    
    coronaInterior = graphicHelp::makeCRectFromCenterPoint(getViewSize().left + getWidth()/2,
                                                           getViewSize().top + getHeight()/2,
                                                           getWidth() - 2*safepadding - 2*backgroundWidth + ecartementInterieur,
                                                           getHeight() - 2*safepadding - 2*backgroundWidth + ecartementInterieur);
    
#if (DISPLAY_VALUE==1)
    CRect textLabelRect = graphicHelp::makeCRect(getWidth()/2, getHeight()/2, coronaInterior.width(), coronaInterior.height());
    textLabelRect.offset(getViewSize().left, getViewSize().top);
    valueTextLabel = new HETextLabel (textLabelRect);
    if (valueTextLabel)
        (((HEEditor*)listener)->getFrame())->addView(valueTextLabel);
    
    updateTextLabelValue();
#endif
    
    
    // A Préciser absoluement.
    setTransparency(false);
    
    
    
}

//------------------------------------------------------------------------
HEKnob::~HEKnob()
{
    CKnob::~CKnob();
}







//------------------------------------------------------------------------
// Tracking
// --------

//------------------------------------------------------------------------
VSTGUI::CMouseEventResult HEKnob::onMouseDown (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons)
{
    return VSTGUI::CKnob::onMouseDown(where, buttons);
}

//------------------------------------------------------------------------
VSTGUI::CMouseEventResult HEKnob::onMouseMoved (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons)
{
    return VSTGUI::CKnob::onMouseMoved(where, buttons);
}

//------------------------------------------------------------------------
VSTGUI::CMouseEventResult HEKnob::onMouseUp (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons)
{
    return VSTGUI::CKnob::onMouseUp(where, buttons);
}








//------------------------------------------------------------------------
// Dessin du bouton
// ----------------

//------------------------------------------------------------------------
 void HEKnob::draw (VSTGUI::CDrawContext *pContext)
{
    
    // Dessin du backGround
    // --------------------
    drawBackground(pContext);
    
    // dessin du contour du bouton (à enlever en fin de construction)
    // ---------------------------
    //drawViewStrokes(pContext);
    
    // Dessin de l'handle
    // ------------------
    HEKnob::drawHandle(pContext);
    
#if (DISPLAY_VALUE==1)
    // Mise à jour de la valeur du champs de text
    // ------------------------------------------
    updateTextLabelValue();
#endif
    
    // fin du dessin
    // -------------
    setDirty (false);
}


//------------------------------------------------------------------------
void HEKnob::drawViewStrokes (VSTGUI::CDrawContext * pContext)
{
    pContext->setDrawMode(VSTGUI::kAntiAliasing);
    // début du dessin
    pContext->setLineStyle(VSTGUI::CLineStyle()); // paramètres par defaut conviennent
    pContext->setLineWidth(1);
    pContext->setFrameColor(white);
    pContext->drawRect(getViewSize(), VSTGUI::kDrawStroked); // dessin du contour
    
}

//------------------------------------------------------------------------
void HEKnob::drawBackground (VSTGUI::CDrawContext * pContext)
{
    pContext->setDrawMode(VSTGUI::kAntiAliasing);
    // début du dessin
    pContext->setLineStyle(VSTGUI::CLineStyle());
    pContext->setLineWidth(backgroundWidth);
    pContext->setFrameColor(black);
    pContext->drawEllipse(coronaBackground);
    // dessin du contour
    pContext->setLineWidth(0.75);
    pContext->setFrameColor(controlBorder);
    pContext->drawEllipse(coronaExteriorShadow, VSTGUI::kDrawStroked);
    pContext->drawEllipse(coronaInteriorShadow, VSTGUI::kDrawStroked);
}



//------------------------------------------------------------------------
void HEKnob::drawHandle(VSTGUI::CDrawContext *pContext)
{
    
    CPoint whereCoronaExt;
    valueToPoint(whereCoronaExt, getWidth()/2 - safepadding);
    
    // -----------------------------------
    // Dessin de l'arc gradient
    // -----------------------------------
    // -----------------------------------
    
    CCoord startAngle = getStartAngle();
    CCoord endAngle = getAngle();
    
    // création de l'effet d'ombre
    // ---------------------------
    CGraphicsPath * lightPath = pContext->createGraphicsPath();
    lightPath->addArc(coronaExteriorShadow, startAngle, endAngle, true);
    CPoint lineToShadow;
    valueToPoint(lineToShadow, coronaInteriorShadow.getWidth()/2);
    lightPath->addLine(lineToShadow);
    lightPath->addArc(coronaInteriorShadow, endAngle, startAngle, false);
    
    VSTGUI::CColor startLightColor = lightBlue;
    VSTGUI::CColor endLightColor = SeaFoamGreen;
    startLightColor.alpha = 255*getValueNormalized()/2;
    endLightColor.alpha = 255*getValueNormalized()/2;
    VSTGUI::CGradient * lightGradient = lightPath->createGradient(1, 0, startLightColor, endLightColor);
    VSTGUI::CPoint * lightStartPoint = new VSTGUI::CPoint(coronaBackground.left, coronaBackground.top);
    VSTGUI::CPoint * lightEndPoint = new VSTGUI::CPoint(coronaBackground.left, coronaBackground.bottom);
    
    pContext->setDrawMode(kAntialias);
    pContext->fillLinearGradient(lightPath, *lightGradient, *lightStartPoint, *lightEndPoint, true);
    lightPath->forget();
    
    // dessin de la courrone
    // ---------------------
    VSTGUI::CGraphicsPath * arcGrad =  pContext->createGraphicsPath();
    
    // add exterior arc clockwise
    arcGrad->addArc(coronaExterior, startAngle, endAngle, true);
    // add line between the two arc
    CPoint lineTo;
    valueToPoint(lineTo, coronaInterior.getWidth()/2);
    arcGrad->addLine(lineTo);
    // add interior arc non-clockwise
    arcGrad->addArc(coronaInterior, endAngle, startAngle, false);
    
    // dessin du gradient
    // ------------------
    VSTGUI::CColor startColor = SeaFoamGreen;
    VSTGUI::CColor endColor = lightBlue;
    VSTGUI::CGradient * gradient = arcGrad->createGradient(0, 1, startColor, endColor);
    // direction du gradient
    VSTGUI::CPoint * startPoint = new VSTGUI::CPoint(coronaExterior.left, coronaExterior.top);
    VSTGUI::CPoint * endPoint = new VSTGUI::CPoint(coronaExterior.left, coronaExterior.bottom);
    
    // Remplissage du chemin dessiné
    // -----------------------------
    pContext->fillLinearGradient(arcGrad, *gradient, *startPoint, *endPoint, true);
    arcGrad->forget();
    
    
    
    
    
    // -----------------------------------
    // Dessin de la ligne blanche
    // -----------------------------------
    // -----------------------------------
    // nouveau calcul des triangles semblable,
    // pour ne pas dessiner jusqu'au centre du bouton
    // depuis l'exterieur de la couronne
    CPoint whereLineStart;
    valueToPoint(whereLineStart, getWidth()/2);
    CPoint whereLineEnd;
    valueToPoint(whereLineEnd, coronaInteriorShadow.getWidth()/2 - safepadding);
    
    
    // dessin du trait
    // ---------------
    pContext->setDrawMode (kAntiAliasing);
    // debut du dessin
    pContext->setLineWidth (handleWidth);
    pContext->setLineStyle (CLineStyle (CLineStyle::kLineCapRound));
    pContext->setFrameColor (whiteHandle); //couleur de l'handle
    pContext->moveTo (whereLineStart);
    pContext->lineTo (whereLineEnd);
    
}


//------------------------------------------------------------------------
void HEKnob::updateTextLabelValue ()
{
    char newText [4];
    sprintf(newText, "%.0f", getValue());
    valueTextLabel->setText(newText);
}


//------------------------------------------------------------------------
// fonctions d'aides
// -----------------

//------------------------------------------------------------------------
// get the angle made by the handle and the line at "getStartAngle"
CCoord HEKnob::getAngle()
{
    float alpha = getValueNormalized();
    alpha = startAngle + alpha*rangeAngle;
    
    return (CCoord) alpha;
}

//------------------------------------------------------------------------
// get the point pointed by the value at the radius.
void HEKnob::valueToPoint(VSTGUI::CPoint &point, double radius)
{
    double alpha = getValueNormalized();
    alpha = startAngle + alpha*rangeAngle; // pourcentage de l'angle final
    
    CPoint center (getViewSize ().width () / 2, getViewSize ().height () / 2);
    center.offset (getViewSize ().left, getViewSize ().top);
    
    
    point.h = center.x + cos (ToRad(alpha)) * radius;
    point.v = center.y + sin (ToRad(alpha)) * radius;
}




