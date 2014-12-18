//
//  bundle.cpp
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 13/10/2014.
//
//

#include "Bundle.h"



#if MAC
namespace VSTGUI {
    //------------------------------------------------------------------------
    void CreateVSTGUIBundleRef ()
    {
        openCount++;
        if (gBundleRef)
        {
            CFRetain (gBundleRef);
            return;
        }
#if TARGET_OS_IPHONE
        gBundleRef = CFBundleGetMainBundle ();
        CFRetain (gBundleRef);
#else
        Dl_info info;
        if (dladdr ((const void*)CreateVSTGUIBundleRef, &info))
        {
            if (info.dli_fname)
            {
                Steinberg::String name;
                name.assign (info.dli_fname);
                for (int i = 0; i < 3; i++)
                {
                    int delPos = name.findLast ('/');
                    if (delPos == -1)
                    {
                        fprintf (stdout, "Could not determine bundle location.\n");
                        return; // unexpected
                    }
                    name.remove (delPos, name.length () - delPos);
                }
                CFURLRef bundleUrl = CFURLCreateFromFileSystemRepresentation (0, (const UInt8*)name.text8 (), name.length (), true);
                if (bundleUrl)
                {
                    gBundleRef = CFBundleCreate (0, bundleUrl);
                    CFRelease (bundleUrl);
                }
            }
        }
#endif
    }
    
    //------------------------------------------------------------------------
    void ReleaseVSTGUIBundleRef ()
    {
        openCount--;
        if (gBundleRef)
            CFRelease (gBundleRef);
        if (openCount == 0)
            gBundleRef = 0;
    }
    
    //------------------------------------------------------------------------
} // namespace VSTGUI

#endif // MAC


