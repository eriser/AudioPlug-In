//
//  Colors.h
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 10/10/2014.
//
//

#ifndef HeyMate_Colors_h
#define HeyMate_Colors_h

#include "lib/ccolor.h"

// Constantes de Couleur
// ---------------------
// use makeCColor(0,0,0,255) as : CColor (red, green, blue, alpha)

// Couleurs de base
static const VSTGUI::CColor black = VSTGUI::MakeCColor();
static const VSTGUI::CColor white = VSTGUI::MakeCColor(255,255,255);
// tansparent
static const VSTGUI::CColor transparent = VSTGUI::MakeCColor(0,0,0,0);

// blanc légèrement transparent des handles
static const VSTGUI::CColor whiteHandle = VSTGUI::MakeCColor(255,255,255,200);
static const VSTGUI::CColor controlBorder = VSTGUI::MakeCColor(100,100,100,150);

// blu
static const VSTGUI::CColor lightBlue = VSTGUI::MakeCColor(54,169,225);
static const VSTGUI::CColor darkBlue = VSTGUI::MakeCColor(10,72,119);
// red
static const VSTGUI::CColor red = VSTGUI::MakeCColor(227,6,19);
static const VSTGUI::CColor darkRed = VSTGUI::MakeCColor(142,3,17);

// green
static const VSTGUI::CColor green = VSTGUI::MakeCColor(58,170,53);
static const VSTGUI::CColor appleGreen = VSTGUI::MakeCColor(149,193,31);
static const VSTGUI::CColor SeaFoamGreen = VSTGUI::MakeCColor(47,172,102);
static const VSTGUI::CColor darkGreen = VSTGUI::MakeCColor(0,102,51);



#endif
