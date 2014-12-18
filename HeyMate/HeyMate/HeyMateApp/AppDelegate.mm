//
//  AppDelegate.m
//  HeyMateApp
//
//  Created by Henri Aribert-Desjardins on 13/10/2014.
//
//

#import "AppDelegate.h"

#import "../Components/Processor.h"
#import "../Components/Controller.h"

#import "public.sdk/source/vst/interappaudio/AudioIO.h"
#import "public.sdk/source/vst/interappaudio/MidiIO.h"
#import "public.sdk/source/vst/interappaudio/VST3Plugin.h"
#import "public.sdk/source/vst/interappaudio/HostApp.h"

using namespace Steinberg::Vst::InterAppAudio;

//------------------------------------------------------------------------
static OSType fourCharCodeToOSType (NSString* inCode)
{
    OSType rval = 0;
    NSData* data = [inCode dataUsingEncoding: NSMacOSRomanStringEncoding];
    [data getBytes:&rval length:sizeof(rval)];
    HTONL(rval);
    return rval;
}

//------------------------------------------------------------------------
@interface HEAppDelegate ()
//------------------------------------------------------------------------
{
    VST3Plugin plugin;
    HEEditor editor;
    
    BOOL audioIOInitialized;
}
@end

//------------------------------------------------------------------------
@implementation HEAppDelegate
//------------------------------------------------------------------------

//------------------------------------------------------------------------
- (BOOL)initAudioIO
{
    // création des caractèristiques du noeud
    // --------------------------------------
    id auArray = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"AudioComponents"];
    if (auArray)
    {
        id desc = [auArray objectAtIndex:0];
        if (desc)
        {
            NSString* typeStr = [desc objectForKey:@"type"];
            NSString* subtypeStr = [desc objectForKey:@"subtype"];
            NSString* manufacturerStr = [desc objectForKey:@"manufacturer"];
            NSString* nameStr = [desc objectForKey:@"name"];
            if (typeStr && subtypeStr && manufacturerStr && nameStr)
            {
                OSType type = fourCharCodeToOSType (typeStr);
                OSType subtype = fourCharCodeToOSType (subtypeStr);
                OSType manufacturer = fourCharCodeToOSType (manufacturerStr);
                
                // création de l'instance du noeud audio
                // -------------------------------------
                AudioIO* audioIO = AudioIO::instance ();
                // initialisation du noeud
                if (audioIO->init (type, subtype, manufacturer, (__bridge CFStringRef)nameStr) == Steinberg::kResultTrue)
                {
                    // création du plugin et initialisation de celui-ci
                    if (plugin.init ())
                    {
                        // rattachement noeud audio / plugin
                        InterAppAudioHostApp::instance ()->setPlugin (&plugin);
                        audioIO->addProcessor (&plugin);
                        audioIOInitialized = YES;
                        return YES;
                    }
                }
            }
        }
    }
    return NO;
}

//------------------------------------------------------------------------
- (BOOL)createUI
{
    if (audioIOInitialized)
    {
        // détection de la taille de l'écran (une fois que celui-ci est retourné)
        CGRect screenSize = {};
        screenSize.size = [[UIScreen mainScreen] currentMode].size;
        // préférence de l'utilisateur
        [UIApplication sharedApplication].statusBarHidden = YES;
        // initialisation de la vue de l'app delegate (vu de l'appli)
        self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
        self.window.backgroundColor = [UIColor whiteColor];
        
        
        // Lancement de l'éditeur,
        // -----------------------
        // rattache le viewController de l'editcontroller à la vue principale
        self.window.rootViewController = editor.getViewController ();
        [self.window makeKeyAndVisible];
        
        // impose la réciprocité du lien entre l'editcontroller et l'éditeur
        // en rattachant à l'éditeur, variable membre de l'app delegate,
        // le controller de la classe VST3Plugin
        editor.attachController((HEController*) plugin.getEditController());
        
        
        // Lancement de la vue,
        // --------------------
        // rattachement de la vue de l'éditeur à sa vue.
        // le cast est un simple bridge
        // (sans changement de l'appartenance de la vue)
        // le dessin de l'éditeur est fait dans la vue.
        return editor.open((__bridge void *)(editor.getViewController()).view);
        
    }
    return NO;
}

//------------------------------------------------------------------------
- (void)savePluginState:(NSCoder*)coder
{
    NSData* processorState = plugin.getProcessorState ();
    NSData* controllerState = plugin.getControllerState ();
    if (processorState)
        [coder encodeObject:processorState forKey:@"VST3ProcessorState"];
    if (controllerState)
        [coder encodeObject:controllerState forKey:@"VST3ControllerState"];
}

//------------------------------------------------------------------------
- (void)restorePluginState:(NSCoder*)coder
{
    NSData* processorState = [coder decodeObjectForKey:@"VST3ProcessorState"];
    if (processorState)
    {
        plugin.setProcessorState (processorState);
    }
    NSData* controllerState = [coder decodeObjectForKey:@"VST3ControllerState"];
    if (controllerState)
    {
        plugin.setControllerState (controllerState);
    }
}



//------------------------------------------------------------------------
//------------------------------------------------------------------------

// UIApplicationDelegate methods

//------------------------------------------------------------------------
//------------------------------------------------------------------------

- (BOOL)application:(UIApplication *)application willFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    /** Lancement de la fonction initAudio
     (VSTInterAppAppDelegate.h)
     
     création des caractèristiques du noeud
     création de l'instance du noeud audio
     création du plugin et initialisation de celui-ci
     rattachement noeud audio / plugin
     */
    
    // --------------------------------
    
    /** lancement de la fonction plugin.init
     création des composant indiqué dans le factory.
     => création controller et processor.
     */
    /** On note que la classe VST3Plugin pour l'interapp audio
     effectue un "wrapping" du processeur et du controller
     pour que ceux-ci puisse être utiliser dans l'application.
     (voir VST3Plugin.mm pour plus d'info)
     */
    
    // --------------------------------
    
    return [self initAudioIO];
}

//------------------------------------------------------------------------
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    /** dans la deuxième partie du lancement de l'application,
     lancement de la fonction createUI
     (VSTInterAppAppDelegate.h)
     
     détection de la taille de l'écran
     préférence de l'utilisateur
     initialisation de la vue de l'app delegate (vu de l'appli)
     Lancement de l'éditeur,
     passage à sa fonction d'initialisation avec la taille de l'écran
     rattache le viewController de l'editcontroller à la vue principale,
     ie la vue de window de l'appDelegate ! appelle de viewDidLoad par la suite.
     
     impose la réciprocité du lien entre l'editcontroller et le plugin
     en rattachant à l'éditeur, variable membre de l'app delegate,
     le controller de la classe VST3Plugin
     */
    
    // ---------------------------------
    
    BOOL result = [self createUI];
    
    if (result)
    {
        AudioIO::instance ()->start ();
    }
    return result;
}

//------------------------------------------------------------------------
- (BOOL)application:(UIApplication *)application shouldSaveApplicationState:(NSCoder *)coder
{
    [self savePluginState:coder];
    [coder encodeBool:MidiIO::instance ().isEnabled () forKey:@"MIDI Enabled"];
    return YES;
}

//------------------------------------------------------------------------
- (BOOL)application:(UIApplication *)application shouldRestoreApplicationState:(NSCoder *)coder
{
    [self restorePluginState:coder];
    BOOL midiEnabled = [coder decodeBoolForKey:@"MIDI Enabled"];
    MidiIO::instance ().setEnabled (midiEnabled);
    return YES;
}

//------------------------------------------------------------------------
- (void)applicationDidBecomeActive:(UIApplication *)application
{
    AudioIO* audioIO = AudioIO::instance ();
    audioIO->start ();
}

//------------------------------------------------------------------------
- (void)applicationWillResignActive:(UIApplication *)application
{
    AudioIO* audioIO = AudioIO::instance ();
    if (audioIO->getInterAppAudioConnected () == false && MidiIO::instance().isEnabled () == false)
    {
        audioIO->stop ();
    }
}

@end








