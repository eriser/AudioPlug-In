//
//  TextLabel.h
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 01/11/2014.
//
//

#ifndef __HeyMate__TextLabel__
#define __HeyMate__TextLabel__

// Paramètres des labels
// ---------------------
#define TEXTLABEL_HEIGHT            15 // largeur normale du slider
#define TEXTLABEL_WIDTH             200 // longueur normale

#include "lib/controls/ctextlabel.h"

class HETextLabel : public VSTGUI::CTextLabel
{
public:
    HETextLabel (VSTGUI::CRect& size, VSTGUI::UTF8StringPtr txt = 0, int32_t style = 0);
    ~HETextLabel ();
    
};



#endif /* defined(__HeyMate__TextLabel__) */
