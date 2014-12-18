//
//  GraphicHelp.cpp
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 21/10/2014.
//
//

#include "GraphicHelp.h"

#include "lib/cbitmap.h"
#import "lib/platform/mac/cgbitmap.h"

#if TARGET_OS_IPHONE
// utiliser dans les fonction spéciale iOS
#import <UIKit/UIKit.h>
#endif

namespace graphicHelp {
    
    

    VSTGUI::CRect makeCRectFromCenterPoint (double middlePosX, double middlePosY, double width, double height)
    {
        
        // size représente les largeurs et hauteur du rectangle.
        VSTGUI::CPoint * sizePoint = new VSTGUI::CPoint(width, height);
        
        // originPoint représente le point haut gauche du rectangle comprenant le composant graphique
        VSTGUI::CPoint * originPoint = new VSTGUI::CPoint(middlePosX - width/2, middlePosY - height/2);
        
        return *new VSTGUI::CRect(*originPoint, *sizePoint);
        
    }

    VSTGUI::CRect makeCRectFromTopLeft (double left, double top, double width, double height)
    {
        
        // size représente les largeurs et hauteur du rectangle.
        VSTGUI::CPoint * sizePoint = new VSTGUI::CPoint(width, height);
        
        // originPoint représente le point haut gauche du rectangle comprenant le composant graphique
        VSTGUI::CPoint * originPoint = new VSTGUI::CPoint(left, top);
        
        return *new VSTGUI::CRect(*originPoint, *sizePoint);
    
    }



    VSTGUI::CRect makeCRect (double posX, double posY, double width, double height, bool originTopLeft)
    {
        if (originTopLeft)
            return makeCRectFromTopLeft(posX, posY, width, height);
        else
            return makeCRectFromCenterPoint(posX, posY, width, height);
    }


    
    
    
    
    
#if TARGET_OS_IPHONE
    
    // Méthodes de resize d'une image
    // utiliser pour le resize de l'image de background pour la version iOS
    // --------------------------------------------------------------------
    
    VSTGUI::CBitmap * rescaleCBitmap ( VSTGUI::CBitmap * toResize, VSTGUI::CRect &newSize)
    {
        // Passage de CRect à CGRect
        // -------------------------
        CGRect newSizeCGRect = CGRectMake(newSize.left, newSize.top, newSize.getWidth(), newSize.getHeight());
        
        // Passage de CBitmap à UIImage
        // ----------------------------
        VSTGUI::CGBitmap* cgBitmap = dynamic_cast<VSTGUI::CGBitmap *> (toResize->getPlatformBitmap());
        UIImage * newUIImage = [UIImage imageWithCGImage:cgBitmap->getCGImage()];
        
        // Dessin dans un contexte dimensionné
        // -----------------------------------
        // On crée un nouveau contexte de dessin à la bonne dimension
        //UIGraphicsBeginImageContextWithOptions(newSizeCGRect.size, NO, 1.0);
        if ([UIScreen instancesRespondToSelector:@selector(scale)])
            UIGraphicsBeginImageContextWithOptions(newSizeCGRect.size, NO, 1.0f);
        else
            UIGraphicsBeginImageContext(newSizeCGRect.size);
        // on dessine l'image à l'interieur
        [newUIImage drawInRect:newSizeCGRect];
        UIImage * destImage = UIGraphicsGetImageFromCurrentImageContext();
        // fin du dessin de l'image
        UIGraphicsEndImageContext();
        
        
        // passage de UIImage à CBitmap
        // ----------------------------
        VSTGUI::CGBitmap * resizeCGBitmap = new VSTGUI::CGBitmap(destImage.CGImage);
        VSTGUI::IPlatformBitmap * platformBitmap = dynamic_cast<VSTGUI::IPlatformBitmap *>(resizeCGBitmap);
        
        // renvoie de l'image
        // ------------------
        return new VSTGUI::CBitmap(platformBitmap); // création de CBitmap à partir de la platformBitmap
    }

#endif
    

} // end namespace