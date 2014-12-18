//
//  AppDelegate.h
//  HeyMateApp
//
//  Created by Henri Aribert-Desjardins on 13/10/2014.
//
//

#import <UIKit/UIKit.h>


#import "public.sdk/source/vst/interappaudio/VSTInterAppAudioAppDelegateBase.h"

/** La reconstruction de la classe helper assure :
 - la création du noeud audio,
 - la création des classes indiquées dans le fichier factory
 - la création de l'UI comme partie intégrante du view controller
 */

//------------------------------------------------------------------------
@interface HEAppDelegate : UIResponder <UIApplicationDelegate>
//------------------------------------------------------------------------
@property (strong, nonatomic) UIWindow *window;

- (BOOL)application:(UIApplication *)application willFinishLaunchingWithOptions:(NSDictionary *)launchOptions;
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions;
- (BOOL)application:(UIApplication *)application shouldSaveApplicationState:(NSCoder *)coder;
- (BOOL)application:(UIApplication *)application shouldRestoreApplicationState:(NSCoder *)coder;
- (void)applicationDidBecomeActive:(UIApplication *)application;
- (void)applicationWillResignActive:(UIApplication *)application;

@end