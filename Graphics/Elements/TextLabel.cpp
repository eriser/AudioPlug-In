//
//  TextLabel.cpp
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 01/11/2014.
//
//

#include "TextLabel.h"

#include "Colors.h"
#include "lib/cbitmap.h"

HETextLabel::HETextLabel (VSTGUI::CRect& size, VSTGUI::UTF8StringPtr txt, int32_t style)
: VSTGUI::CTextLabel(size, txt, NULL, style)
{
    //setBackground(new VSTGUI::CBitmap(getWidth(), getHeight()));
    setBackColor(VSTGUI::MakeCColor(0,0,0,100));
    setFrameColor(transparent);
    setFontColor(white);
}

HETextLabel::~HETextLabel ()
{
    VSTGUI::CTextLabel::~CTextLabel();
}