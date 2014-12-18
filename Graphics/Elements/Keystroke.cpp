//
//  Keystroke.cpp
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 12/11/2014.
//
//

#include "Keystroke.h"
#include "Editor.h"

#include "Keyboard.h"

#include "lib/cframe.h"
#include "lib/cdrawcontext.h"

#include "ivstevents.h"

#include "../General/Colors.h"
#include "../General/GraphicHelp.h"

#include "ParametersDescription.h"

//------------------------------------------------------------------------
// fonction d'aide au calcul
// -------------------------

//------------------------------------------------------------------------






//------------------------------------------------------------------------
// HEKeystroke Implémentation
// ----------------------------

//------------------------------------------------------------------------
HEKeystroke::HEKeystroke (VSTGUI::CRect &newKeystrokeSize, HEKeyboard * _delegate, int _noteNbr, bool _isWhite)
: keystrokeSize(newKeystrokeSize)
, keyboard(_delegate)
, noteNbr(_noteNbr)
, isWhite(_isWhite)
, isActive(false)
, CControl(newKeystrokeSize, _delegate, kKeystroke)
{
    
}

//------------------------------------------------------------------------
HEKeystroke::HEKeystroke (HEKeystroke * oldStroke)
: keystrokeSize(oldStroke->getViewSize())
, keyboard(oldStroke->keyboard)
, noteNbr(oldStroke->noteNbr)
, isWhite(oldStroke->isWhite)
, isActive(false)
, CControl(oldStroke->getViewSize(), oldStroke->keyboard, kKeystroke)
{
    
}


//------------------------------------------------------------------------
HEKeystroke::~HEKeystroke ()
{
    
}






//------------------------------------------------------------------------
// Tracking
// --------

//------------------------------------------------------------------------
VSTGUI::CMouseEventResult HEKeystroke::onMouseDown (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons)
{
    // on indique que la touche est active.
    isActive = true;
    
    // on retien la position du clic
    oldWhere = where;
    
    
    // Initialisation du traitement en fonction de
    // Debut de l'automatisation du parametre
    // --------------------------------------
    beginEdit ();
    
    
    // On envoie une note on
    // ---------------------
    sendNoteOn();
    
    
    // l'appuis sur la touche renvoie directement à cette fonction pour les translation de clics.
    return onMouseMoved(where, buttons);
}



#include <iostream>

//------------------------------------------------------------------------
// cette fonction est appelé pour chaque fois que la souris est bougé.
VSTGUI::CMouseEventResult HEKeystroke::onMouseMoved (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons)
{
    int alphaisout = 0;
    // la souris a effectivement bougé !
    if (where != oldWhere)
    {
        // on retient la nouvelle position avant de passé à la suite.
        oldWhere = where;
        
        if (where.isInside(getViewSize()))
        {
            // Si le clic est encore dans la zone de la touche, on ne fait rien.
            
        }
        else
        {
            // Si le clic est sorti du control,
            // on demande au keyboard de passer sur une autre note,
            // on quitte
            alphaisout++;
            std::cout << alphaisout<<std::endl;
            
            keyboard->mouseMouvedOnKeyboard();
            
            // on quitte
            onMouseUp(where, buttons);
            // pour sortir de la boucle, on indique que l'évènement n'as pas lieu d'être géré à ce moment là...
            return VSTGUI::CMouseEventResult::kMouseMoveEventHandledButDontNeedMoreEvents;
            
            
        }
        
        
        
        
    }
    
    
    
    
    if (isDirty ())
        invalid ();
    
    //-- all other mouse handling stuff is done in the parent class
    return VSTGUI::CMouseEventResult::kMouseEventHandled;
}

//------------------------------------------------------------------------
VSTGUI::CMouseEventResult HEKeystroke::onMouseUp (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons)
{
    isActive = false;
    // Indiquer une note Off,
    // ----------------------
    sendNoteOff();
    
    endEdit ();
    return VSTGUI::CMouseEventResult::kMouseEventHandled;
}





//------------------------------------------------------------------------
// Envoie des events NoteOn et NoteOff
// -----------------------------------

//------------------------------------------------------------------------
void HEKeystroke::sendNoteOn()
{
    // NoteOnEvent est une structure comprenant
    // ----------------------------------------
    //int16 channel;			///< channel index in event bus
    //int16 pitch;			///< range [0, 127] = [C-2, G8] with A3=440Hz
    //float tuning;			///< 1.f = +1 cent, -1.f = -1 cent
    //float velocity;			///< range [0.0, 1.0]
    //int32 length;           ///< in sample frames (optional, Note Off has to follow in any case!)
    //int32 noteId;			///< note identifier (if not available then -1)
    
    // ne sont pas géré ici :
    // note channel,
    // note lenght // because it's optionnal (wait for noteOff signal)
    // noteId (parce que je ne sais pas ce que c'est...)
    
    
    
    
    // place dans le paramètre le numéro de la note.
    setValue(noteNbr);
    setDirty();
    if (isDirty ())
    {
        valueChanged ();
        invalid ();
    }
    
    
    
    
}



//------------------------------------------------------------------------
void HEKeystroke::sendNoteOff()
{
    // NoteOffEvent est une structure comprenant
    // -----------------------------------------
    //int16 channel;			///< channel index in event bus
    //int16 pitch;			///< range [0, 127] = [C-2, G8] with A3=440Hz
    //float velocity;			///< range [0.0, 1.0]
    //int32 noteId;			///< associated noteOn identifier (if not available then -1)
    //float tuning;			///< 1.f = +1 cent, -1.f = -1 cent
    
    
    // ne sont pas géré ici :
    // note channel,
    // noteId (parce que je ne sais pas ce que c'est...)
    
    
    
    // place dans le paramètre le numéro de la note.
    setValue(-1);
    
    setDirty();
    if (isDirty ())
    {
        valueChanged ();
        invalid ();
    }
    
}









//------------------------------------------------------------------------
// Dessin de la touche
// -------------------

//------------------------------------------------------------------------
void HEKeystroke::draw (VSTGUI::CDrawContext *pContext)
{
    // Dessin du backGround
    // --------------------
    drawBackground(pContext);
    
    if (isActive)
        drawActiveStroke(pContext);
    
    
    // fin du dessin
    // -------------
    setDirty (false);
}



//------------------------------------------------------------------------
void HEKeystroke::drawBackground (VSTGUI::CDrawContext * pContext)
{
    pContext->setDrawMode(VSTGUI::kAntiAliasing);
    // début du dessin
    CGraphicsPath * roundBackground = pContext->createGraphicsPath();
    roundBackground->addRoundRect(keystrokeSize, 10);
//    CRect squareOnTop = graphicHelp::makeCRect(keystrokeSize.left,
//                                               keystrokeSize.top,
//                                               getWidth(),
//                                               getHeight()/2);
//    roundBackground->addRect(squareOnTop);
    
    if (isWhite)
        pContext->setFillColor(white);
    else
        pContext->setFillColor(black);
    
    pContext->drawGraphicsPath(roundBackground, CDrawContext::kPathFilled);
    // contours
    //pContext->setLineWidth(0.75);
    //pContext->setFrameColor(controlBorder);
    //pContext->drawGraphicsPath(roundBackground, CDrawContext::kPathStroked);
    
    roundBackground->forget();    
    
}


void HEKeystroke::drawActiveStroke (VSTGUI::CDrawContext * pContext)
{
    
    pContext->setDrawMode(VSTGUI::kAntiAliasing);
    // début du dessin
    CGraphicsPath * activeBackground = pContext->createGraphicsPath();
    activeBackground->addRoundRect(keystrokeSize, 10);
//    CRect squareOnTop = graphicHelp::makeCRect(keystrokeSize.left,
//                                               keystrokeSize.top,
//                                               getWidth(),
//                                               getHeight()/2);
//    activeBackground->addRect(squareOnTop);
    
    VSTGUI::CColor startColor = transparent;
    VSTGUI::CColor endColor = lightBlue;
    VSTGUI::CGradient * Gradient = activeBackground->createGradient(1, 0, startColor, endColor);
    VSTGUI::CPoint * StartPoint = new VSTGUI::CPoint(getViewSize().left, getViewSize().top);
    VSTGUI::CPoint * EndPoint = new VSTGUI::CPoint(getViewSize().left, getViewSize().bottom);
    
    
    pContext->setDrawMode(kAntialias);
    pContext->fillLinearGradient(activeBackground, *Gradient, *StartPoint, *EndPoint, true);
    
    // contours
    //pContext->setLineWidth(0.75);
    //pContext->setFrameColor(controlBorder);
    //pContext->drawGraphicsPath(roundBackground, CDrawContext::kPathStroked);
    
    activeBackground->forget();
    
    
}




//------------------------------------------------------------------------
// fonction helpers
// ----------------

//------------------------------------------------------------------------

// accesing the note number
// ------------------------
void HEKeystroke::setNote (int newNoteNbr)
{
    noteNbr = newNoteNbr;
}

int HEKeystroke::getNote ()
{
    return noteNbr;
}

