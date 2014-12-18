//
//  bundle.h
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 13/10/2014.
//
//

#ifndef __HeyMate__Bundle__
#define __HeyMate__Bundle__


// Gestion des bundles,
// Spécifique iOS (et mac, mais pas encore utilisé...)
// ---------------------------------------------------


namespace VSTGUI {
	static void CreateVSTGUIBundleRef ();
	static void ReleaseVSTGUIBundleRef ();
    
    
    void* gBundleRef = 0;
    static int openCount = 0;
}




#endif /* defined(__HeyMate__Bundle__) */
