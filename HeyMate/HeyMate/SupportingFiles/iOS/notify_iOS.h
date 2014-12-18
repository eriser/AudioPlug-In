/**
 * Notification management.
 */



// Possibilité de désactivée les notifications.
#define USE_NOTIFICATION    1

// désactive les notifications de debuggage
#define USE_DEBUG_MESSAGE   1
// désactive les warnings.


#ifndef _notify_h
#define _notify_h

// -----------------------------------------------------------------
// INFO_MESSAGE : write an info message into the notification file.
// -----------------------------------------------------------------
// nbr de message de debug.
    
static int nbrInfoMsg = 1;


static void INFO_MSG(const char * _infoMsg, const char * _nomFonction = NULL){
#if (USE_NOTIFICATION == 1)
    NSString * infoMsg = [[NSString alloc] initWithCString:_infoMsg encoding:NSUTF8StringEncoding];
    
    
    NSMutableString * footPrint = [[NSMutableString alloc] initWithString:@"\n"];
    [footPrint appendString:@"----------------\n"];
    if (_nomFonction){
        NSString * nomFonction = [[NSString alloc] initWithCString:_nomFonction encoding:NSUTF8StringEncoding];
        [footPrint appendString:[NSString stringWithFormat:@"Fonction : %@.\n", nomFonction]];
    }
    [footPrint appendString:[NSString stringWithFormat:@"INFO %d : %@\n----------------\n", nbrInfoMsg, infoMsg]];
    
    NSLog(@"%@", footPrint);
    nbrInfoMsg++;
    
#endif // use notification
}


// -----------------------------------------------------------------
// DEBUG_MESSAGE : write a debug message into the notification file.
// -----------------------------------------------------------------
    // nbr de message de debug.
    
static int nbrDebugMsg = 1;


static void DEBUG_MSG(const char* _debugMsg, double paramObserver = 0){
#if (USE_NOTIFICATION == 1)
#if (USE_DEBUG_MESSAGE == 1)
    
   
    
    NSMutableString * footPrint = [[NSMutableString alloc] initWithString:@"\n"];;
    
    if (_debugMsg){
         NSString * debugMsg = [[NSString alloc] initWithCString:_debugMsg encoding:NSUTF8StringEncoding];
        [footPrint appendString:[NSString stringWithFormat:@"DEBUG MSG %d : %@", nbrDebugMsg, debugMsg]];
    }
    
    if (paramObserver){
        [footPrint appendString:[NSString stringWithFormat:@"%f\n", paramObserver]];
    }
    
    NSLog(@"%@", footPrint);
    nbrDebugMsg++;
    
#endif // use debug
#endif // use notification
}
    

#endif // end inclusion ifndef.