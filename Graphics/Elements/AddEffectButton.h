//
//  AddEffectButton.h
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 04/11/2014.
//
//

#ifndef __HeyMate__AddEffect__
#define __HeyMate__AddEffect__

#include "lib/controls/ccontrol.h"

class HEAddEffectButton : public VSTGUI::CControl
{
public:
    HEAddEffectButton (VSTGUI::CRect &frameRect);
    ~HEAddEffectButton ();
    
    // dessin du bouton
    // ----------------
    virtual void draw (VSTGUI::CDrawContext * pContext);
    
    
    // From CControls
    // --------------
    virtual VSTGUI::CMouseEventResult onMouseDown (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons);
    virtual VSTGUI::CMouseEventResult onMouseUp (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons);
    virtual VSTGUI::CMouseEventResult onMouseMoved (VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons);
    
    
protected:
    
    
    
};


#endif /* defined(__HeyMate__AddEffect__) */
