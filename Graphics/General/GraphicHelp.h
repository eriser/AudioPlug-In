//
//  GraphicHelp.h
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 21/10/2014.
//
//

#ifndef __HeyMate__graphicHelp__
#define __HeyMate__graphicHelp__

#include "lib/controls/ccontrol.h"

namespace graphicHelp {
 
    // fonction Helpers
    // ------------------------------------
    // repositionnement d'un CRect
    VSTGUI::CRect makeCRect (double posX, double posY, double width, double height, bool originTopLeft = false);
    VSTGUI::CRect makeCRectFromCenterPoint (double middlePosX, double middlePosY, double width, double height);
    VSTGUI::CRect makeCRectFromTopLeft (double left, double top, double width, double height);
    
#if TARGET_OS_IPHONE
    // redimenssionnement d'une image, iOS uniquement
    VSTGUI::CBitmap * rescaleCBitmap ( VSTGUI::CBitmap * toResize, VSTGUI::CRect &newSize);
#endif
    
    
} // end namespace
    
#endif /* defined(__HeyMate__graphicHelp__) */
