//
//  ViewController.h
//  HeyMateApp
//
//  Created by Henri Aribert-Desjardins on 13/10/2014.
//
//

#import <UIKit/UIKit.h>

class HEEditor;

@interface HEViewController : UIViewController{
    
    HEEditor * editor; // class d'éditeur. à afficher comme vue principale
    
    
}


- (void) setEditor: (HEEditor *) attachedEditor;



@end



