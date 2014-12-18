//
//  ViewController.m
//  HeyMateApp
//
//  Created by Henri Aribert-Desjardins on 13/10/2014.
//
//

#import "ViewController.h"


@interface HEViewController (){
    BOOL isViewLoad;
}

@end



@implementation HEViewController



- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    self.view.backgroundColor = [UIColor colorWithRed:0.21 green:0.67 blue:0.88 alpha:1];
    
    // Attention cette méthode est appelé deux fois !!!
    // une fois par le constructeur de UIViewController,
    // une autre fois par l'appDelegate
    
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}



- (void) setEditor:(HEEditor *)attachedEditor
{
    editor = attachedEditor ;
}


// Préférences de l'application
// ----------------------------
- (BOOL)prefersStatusBarHidden { return YES; }
- (BOOL)shouldAutorotate { return YES; }
- (NSUInteger)supportedInterfaceOrientations
{
	return UIInterfaceOrientationMaskLandscapeLeft|UIInterfaceOrientationMaskLandscapeRight;
}


@end
