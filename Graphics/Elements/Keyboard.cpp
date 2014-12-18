//
//  Keyboard.cpp
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 12/11/2014.
//
//

#include "Keyboard.h"
#include "Editor.h"
#include "Controller.h"

#include "Keystroke.h"
#include "Slider.h"
#include "TextLabel.h"

#include "lib/cframe.h"
#include "lib/cdrawcontext.h"

#include "../General/Colors.h"
#include "../General/GraphicHelp.h"


#include "../../Heymate/Heymate/Processing/ParametersDescription.h"

//------------------------------------------------------------------------
// fonction d'aide au calcul
// -------------------------

//------------------------------------------------------------------------
enum{
    Gd = 0,
    A,
    Ad,
    B,
    C,
    Cd,
    D,
    Dd,
    E,
    F,
    Fd,
    G
};







//------------------------------------------------------------------------
// HEKeyboard Implémentation
// ----------------------------

//------------------------------------------------------------------------
HEKeyboard::HEKeyboard (VSTGUI::CRect &newKeyboardSize, HEEditor * _editor, HEController * _controller, int _firstStroke, int _nbrOfStroke)
: firstStroke(_firstStroke)
, nbrOfStrokes(_nbrOfStroke)
, backgroundRect(newKeyboardSize)
, spacing(15)
, interStrokeSpacing(2)
, editor(_editor)
, controller(_controller)
, CControl (newKeyboardSize)
{
    // chargement des rectangles de construction.
    // ------------------------------------------
    // Slider horizotal
    // ----------------
    keyboardPositionSliderRect = graphicHelp::makeCRect(getWidth()/2,
                                                        spacing + (SLIDER_WIDTHNESS)/2,
                                                        getWidth()/4,
                                                        SLIDER_WIDTHNESS + 5);
    keyboardPositionSliderRect.offset(getViewSize().left, getViewSize().top);
    
    // slider vertical
    // ---------------
    keyboardSizeSliderRect = graphicHelp::makeCRect(2*spacing,
                                                    getHeight()/2,
                                                    SLIDER_WIDTHNESS + 5,
                                                    getHeight()-2*spacing);
    keyboardSizeSliderRect.offset(getViewSize().left, getViewSize().top);
    
    
    // rectangle du clavier
    // --------------------
    keyboardRect = graphicHelp::makeCRectFromTopLeft(keyboardSizeSliderRect.right + spacing,
                                                     keyboardPositionSliderRect.bottom + spacing,
                                                     getWidth() - 3*spacing - keyboardSizeSliderRect.getWidth(),
                                                     getHeight() - 2*spacing - keyboardPositionSliderRect.getHeight());
    
    
    // slider pour les changement
    // --------------------------
    // du nombre de touches :
    keyboardSizeSlider = new HESlider(keyboardSizeSliderRect, this, kKeyboardSizeSlider);
    keyboardSizeSlider->setMax(MAX_NBR_STROKES_ON_VIEW);
    keyboardSizeSlider->setMin(12);
    keyboardSizeSlider->setValue(nbrOfStrokes);
    keyboardSizeSlider->setDirty();
    
    if (keyboardSizeSlider)
        (editor->getFrame())->addView(keyboardSizeSlider);
    
    // de la position du clavier
    keyboardPositionSlider = new HESlider(keyboardPositionSliderRect, this, kKeyboardPositionSlider);
    keyboardPositionSlider->setMax(MAX_NBR_STROKES-keyboardPositionSlider->getMax());
    keyboardPositionSlider->setMin(0);
    keyboardPositionSlider->setValue(firstStroke);
    keyboardPositionSlider->setDirty();
    
    if (keyboardPositionSlider)
        (editor->getFrame())->addView(keyboardPositionSlider);
    
    
    // affichage de la première note
    // -----------------------------
    CRect labelrect = graphicHelp::makeCRectFromTopLeft(keyboardRect.left, keyboardPositionSliderRect.top, 100, SLIDER_WIDTHNESS+10);
    firstStrokeLabel = new HETextLabel(labelrect, "first stroke");
    if (firstStrokeLabel)
        (editor->getFrame())->addView(firstStrokeLabel);
    updateLabel();
    
    
    
    // construction du clavier.
    // ------------------------
    buildKeyboard();
    
    
}

//------------------------------------------------------------------------
HEKeyboard::~HEKeyboard ()
{
    if (keyboardPositionSlider)
        keyboardPositionSlider->forget();
    keyboardPositionSlider = 0;
    if (keyboardSizeSlider)
        keyboardSizeSlider->forget();
    keyboardSizeSlider = 0;
    
    
    for (std::vector<HEKeystroke*>::iterator strokesIterated = keystrokes.begin();
         strokesIterated != keystrokes.end();
         ++strokesIterated)
    {
        (*strokesIterated)->~HEKeystroke();
    }
    
    
}






//------------------------------------------------------------------------
// Tracking
// --------

//------------------------------------------------------------------------
VSTGUI::CMouseEventResult HEKeyboard::onMouseDown (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons)
{
    return VSTGUI::kMouseMoveEventHandledButDontNeedMoreEvents;
}

//------------------------------------------------------------------------
VSTGUI::CMouseEventResult HEKeyboard::onMouseMoved (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons)
{
    return VSTGUI::kMouseDownEventHandledButDontNeedMovedOrUpEvents;
}

//------------------------------------------------------------------------
VSTGUI::CMouseEventResult HEKeyboard::onMouseUp (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons)
{
    return VSTGUI::kMouseDownEventHandledButDontNeedMovedOrUpEvents;
}






//------------------------------------------------------------------------
// gestion des appuies des touches
// ---------------------------------

//------------------------------------------------------------------------
void HEKeyboard::mouseMouvedOnKeyboard()
{
    
    
}


//------------------------------------------------------------------------
void HEKeyboard::sendNoteOn(Steinberg::Vst::Event noteOn)
{
    //host->scheduleEventFromUI (noteOn);
}



//------------------------------------------------------------------------
void HEKeyboard::sendNoteOff(Steinberg::Vst::Event noteOff)
{
    
    //host->scheduleEventFromUI (noteOff);
}



//------------------------------------------------------------------------
// from CControlListener
// ---------------------------------

//------------------------------------------------------------------------

void HEKeyboard::valueChanged (CControl* pControl)
{
    switch (pControl->getTag())
    {
        case kKeyboardPositionSlider:
            firstStroke = (int) keyboardPositionSlider->getValue();
            nbrOfStrokes = (int) keyboardSizeSlider->getValue();
            //scaleKeyboard();
            
            
            // pour avoir des slider à "cran"
            keyboardPositionSlider->setValue(firstStroke);
            keyboardSizeSlider->setValue(nbrOfStrokes);
            
            // mise à jour de gui des sliders
            keyboardPositionSlider->setDirty();
            keyboardSizeSlider->setDirty();
            
            buildKeyboard();
            
            break;
            //------------------
        case kKeyboardSizeSlider:
            nbrOfStrokes = (int) keyboardSizeSlider->getValue();
            firstStroke = (int) keyboardPositionSlider->getValue();
            //scaleKeyboard();
            
            // pour avoir des slider à "cran"
            keyboardPositionSlider->setValue(firstStroke);
            keyboardSizeSlider->setValue(nbrOfStrokes);
            
            // mise à jour de gui des sliders
            keyboardPositionSlider->setDirty();
            keyboardSizeSlider->setDirty();
            
            // il faut updater le maximum de la position
            keyboardPositionSlider->setMax(MAX_NBR_STROKES - nbrOfStrokes);
            
            buildKeyboard();
            break;
            //------------------
            //------------------
            
        
            
        case kKeystroke:
            controller->beginEdit(kKeystroke);
            controller->setParamNormalized (kKeystroke, pControl->getValue ());
            controller->performEdit (kKeystroke, pControl->getValue ());
            controller->endEdit(kKeystroke);
            break;
            
        default:
            break;
    }
    
}


//------------------------------------------------------------------------
void HEKeyboard::updateLabel ()
{
    char newText [15];
    
    int note = firstStroke%12;
    int tunning = floor(firstStroke/12);
    
    switch (note)
    {
        case A: sprintf(newText, "A %d", tunning); break;
        case B: sprintf(newText, "B %d", tunning); break;
        case C: sprintf(newText, "C %d", tunning); break;
        case D: sprintf(newText, "D %d", tunning); break;
        case E: sprintf(newText, "E %d", tunning); break;
        case F: sprintf(newText, "F %d", tunning); break;
        case G: sprintf(newText, "G %d", tunning); break;
        case Ad: sprintf(newText, "A# %d", tunning); break;
        case Cd: sprintf(newText, "C# %d", tunning); break;
        case Dd: sprintf(newText, "D# %d", tunning); break;
        case Fd: sprintf(newText, "F# %d", tunning); break;
        case Gd: sprintf(newText, "G# %d", tunning); break;
        
        default:
            sprintf(newText, "first stroke");
            break;
    }
    
    firstStrokeLabel->setText(newText);
    firstStrokeLabel->setDirty();
    firstStrokeLabel->invalid();
}

//------------------------------------------------------------------------
void HEKeyboard::scaleKeyboard ()
{
    
    if (firstStroke>1)
    {
        
        if (isStrokeWhite(firstStroke))
        {
            int firstNote = firstStroke%12;
            if (firstNote == A || firstNote == B || firstNote == D || firstNote == E || firstNote == G)
            {
                firstStroke--; // si il y a une noir avant, on décale la première touche
                nbrOfStrokes++; // on incrémente le nombre de touche sur le clavier pour compenser.
            }
        }
    }
    
    
    
    if (isStrokeWhite(firstStroke + nbrOfStrokes-1)) // si la dernière touche du clavier et blanche
    {
        int lastNote = (firstStroke + nbrOfStrokes - 1)%12;
        if (lastNote == A || lastNote == C || lastNote == D || lastNote == F || lastNote == G)
        {
            nbrOfStrokes++; // on incrémente le nombre de touche sur le clavier pour compenser.
        }
    }
    
    
}





//------------------------------------------------------------------------
// Dessin du bouton
// ----------------

//------------------------------------------------------------------------
void HEKeyboard::draw (VSTGUI::CDrawContext *pContext)
{
    // Dessin du backGround
    // --------------------
    if (!getBackground())
    {
        drawBackground(pContext);
        
        // il faut alors redessiner les slider
        // pour qu'il n'apparaissent pas à l'arrière du fond...
        keyboardSizeSlider->setDirty();
        keyboardSizeSlider->draw(pContext);
        keyboardPositionSlider->setDirty();
        keyboardPositionSlider->draw(pContext);
        
        firstStrokeLabel->VSTGUI::CView::setDirty();
        firstStrokeLabel->draw(pContext);
        
        for (std::vector<HEKeystroke*>::iterator strokesIterated = keystrokes.begin();
             strokesIterated != keystrokes.end();
             ++strokesIterated)
        {
            (*strokesIterated)->setDirty();
            (*strokesIterated)->invalid();
        }
        
        
    }
    
    // dessin du fond du clavier
    // -------------------------
    pContext->setLineWidth(5);
    pContext->setFrameColor(controlBorder);
    pContext->setFillColor(black);
    pContext->drawRect(keyboardRect, CDrawStyle::kDrawStroked);
    pContext->drawRect(keyboardRect, CDrawStyle::kDrawFilled);
    
    // dessin du label
    // ---------------
    updateLabel();
    
    // fin du dessin
    // -------------
    setDirty (false);
}


//------------------------------------------------------------------------
void HEKeyboard::drawBackground (VSTGUI::CDrawContext * pContext)
{
    pContext->setDrawMode(VSTGUI::kAntiAliasing);
    // dessin du fond
    // --------------
    CGraphicsPath * roundBackground = pContext->createGraphicsPath();
    roundBackground->addRoundRect(backgroundRect, 10);
    
    pContext->setFillColor(lightBlue);
    pContext->drawGraphicsPath(roundBackground, CDrawContext::kPathFilled);
    pContext->setLineWidth(1);
    pContext->setFrameColor(controlBorder);
    pContext->drawGraphicsPath(roundBackground, CDrawContext::kPathStroked);
    
    roundBackground->forget();
    
    // dessin du contour du clavier
    // ----------------------------
    CGraphicsPath * roundKeyboard = pContext->createGraphicsPath();
    roundKeyboard->addRect(keyboardRect);
    pContext->setLineWidth(5);
    pContext->setFrameColor(controlBorder);
    pContext->drawGraphicsPath(roundKeyboard, CDrawContext::kPathStroked);
    
    roundKeyboard->forget();
    
    
    // dessin du contour des slides
    // ----------------------------
    CGraphicsPath * roundSlide = pContext->createGraphicsPath();
    roundSlide->addRect(keyboardPositionSliderRect);
    roundSlide->addRect(keyboardSizeSliderRect);
    pContext->setLineWidth(3);
    pContext->setFrameColor(controlBorder);
    pContext->drawGraphicsPath(roundSlide, CDrawContext::kPathStroked);
    
    roundSlide->forget();
    
    
    
    // rétention de l'image de fond finale
    // -----------------------------------
    CBitmap * backgroundBitmap = new CBitmap(backgroundRect.getWidth(), backgroundRect.getHeight());
    pContext->drawBitmap(backgroundBitmap, backgroundRect);
    
    setBackground(backgroundBitmap);
    
}






//------------------------------------------------------------------------
// Construction du clavier
// -----------------------

//------------------------------------------------------------------------


void HEKeyboard::buildKeyboard ()
{
    
    int whiteStrokesNbr = getWhiteStrokesNbr(firstStroke, firstStroke + nbrOfStrokes -1);
    //int blackStrokesNbr = getBlackStrokesNbr(firstStroke, firstStroke + nbrOfStrokes -1);
    
    
    // tout d'abord, on supprime toute les touches
    // -------------------------------------------
    for (std::vector<HEKeystroke*>::reverse_iterator strokesIterated = keystrokes.rbegin();
         strokesIterated != keystrokes.rend();
         ++strokesIterated)
    {
        (editor->getFrame())->removeView(*strokesIterated);
        keystrokes.pop_back();
    }
    
    
    
    
    
    // rectangle de la première touche blanche
    // ---------------------------------------
    double whiteStrokesWidth = (keyboardRect.getWidth() - (whiteStrokesNbr - 1) * interStrokeSpacing)/whiteStrokesNbr;
    whiteStrokeSize = graphicHelp::makeCRectFromTopLeft(keyboardRect.left,
                                                        keyboardRect.top,
                                                        whiteStrokesWidth,
                                                        keyboardRect.getHeight());
    
    
    // rectangle de la première touche noire
    // ---------------------------------------
    double blackStrokeHeight = whiteStrokeSize.getHeight() * 0.60;
    blackStrokeSize = graphicHelp::makeCRect(whiteStrokeSize.right + interStrokeSpacing/2,
                                             keyboardRect.top + blackStrokeHeight/2,
                                             whiteStrokesWidth * 0.75,
                                             blackStrokeHeight);
    
    // si la première touche est noire, on en affiche que la moitié... donc deux rectangle de construction
    // de même si la dernière touche est noir on en affiche que la moitié...
    halfBlackStrokeSize = graphicHelp::makeCRectFromTopLeft(keyboardRect.left,
                                                            keyboardRect.top,
                                                            (whiteStrokesWidth * 0.75 - interStrokeSpacing) / 2,
                                                            blackStrokeHeight);
    
    
    
    
    
    
    
    // création de la listes des touches du clavier
    // --------------------------------------------
    // note, il faut ajouter les notes blanches avant les notes noires
    // pour éviter un effet de débordement de touches blanches sur les noires
    HEKeystroke * keystrokeIterated;
    
    // création de touches blanches
    // ----------------------------
    for (int i = 0;
         i<nbrOfStrokes;
         i++)
    {
        if (isStrokeWhite(firstStroke + i))
        {
            // on crée la touche
            keystrokeIterated = new HEKeystroke (whiteStrokeSize, this, firstStroke + i, true);
            // on l'ajoute à la vue et dans la listes des éléments gérés
            if ((editor->getFrame())->addView(keystrokeIterated))
                keystrokes.push_back(keystrokeIterated);
            
            // on décale le rectangle de construction.
            whiteStrokeSize.offset(whiteStrokeSize.getWidth() + interStrokeSpacing, 0);
        }
    }
    
    
    
    // création de touches noires
    // --------------------------
    std::vector<HEKeystroke*>::iterator strokeIterator = keystrokes.begin();
    for (int j = 0;
         j < nbrOfStrokes;
         j++)
    {
        if (!isStrokeWhite(firstStroke + j))
        {
            // si la première touche est noire, le rectangle de construction est half
            if (j == 0 || j == nbrOfStrokes-1)
            {
                // si la première touche est noire, on en affiche que la moitié... donc deux rectangle de construction
                // de même si la dernière touche est noir on en affiche que la moitié...
                double halfBlackStrokeWidth = (whiteStrokesWidth * 0.75 - interStrokeSpacing) / 2;
                if (j==0)
                {
                    // demi touches
                    
                    halfBlackStrokeSize = graphicHelp::makeCRectFromTopLeft(keyboardRect.left,
                                                                            keyboardRect.top,
                                                                            halfBlackStrokeWidth,
                                                                            blackStrokeHeight);
                }
                if (j==nbrOfStrokes-1) {
                    // demi touches
                    halfBlackStrokeSize = graphicHelp::makeCRectFromTopLeft(keyboardRect.right -halfBlackStrokeWidth,
                                                                            keyboardRect.top,
                                                                            halfBlackStrokeWidth,
                                                                            blackStrokeHeight);
                }
                
                
                keystrokeIterated = new HEKeystroke (halfBlackStrokeSize, this, firstStroke+j, false);
                
            }
            else
            {
                
                // on crée la touche noire
                keystrokeIterated = new HEKeystroke (blackStrokeSize, this, firstStroke + j, false);
            
                blackStrokeSize.offset(whiteStrokeSize.getWidth() + interStrokeSpacing, 0);
                // si 2 touches plus loin la touches est blanche, il faut déplacer deux fois plus...
                if (isStrokeWhite(firstStroke+j+2))
                blackStrokeSize.offset(whiteStrokeSize.getWidth() + interStrokeSpacing, 0);
                
            }
            
            if ((editor->getFrame())->addView(keystrokeIterated))
            {
                // if (strokeIterator != keystrokes.end())
                keystrokes.push_back(keystrokeIterated);
            }
            
        }
        
        strokeIterator++;
    }
    
    // toutes les touches on besoin d'être redessinées.
    for (std::vector<HEKeystroke*>::iterator strokesIterated = keystrokes.begin();
         strokesIterated != keystrokes.end();
         ++strokesIterated)
    {
        (*strokesIterated)->setDirty();
        (*strokesIterated)->invalid();
    }
    
    
}






//------------------------------------------------------------------------
// fonction helpers
// ----------------

//------------------------------------------------------------------------

//------------------------------------------------------------------------
// détection touches noires/blanches
bool HEKeyboard::isStrokeWhite(int strokeNbr)
{
    int s = strokeNbr%12; // les tableau commence à 0...
    if (s==A ||
        s==B ||
        s==C ||
        s==D ||
        s==E ||
        s==F ||
        s==G)
        return true;
    else
        return false;
    
}


// compte le nombre de touches blanches/noir
int HEKeyboard::getWhiteStrokesNbr (int startStroke, int endStrokes)
{
    int nbr = 0;
    for (int i = startStroke;
         i<=endStrokes;
         i++)
    {
        if (isStrokeWhite(i))
            nbr++;
    }
    return nbr;
}

int HEKeyboard::getBlackStrokesNbr (int startStroke, int endStrokes)
{
    int nbr = 0;
    for (int i = startStroke;
         i<=endStrokes;
         i++)
    {
        if (!isStrokeWhite(i))
            nbr++;
    }
    return nbr;
}


