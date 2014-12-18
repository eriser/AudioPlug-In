//
//  HeyMate_Macros.h
//  HeyMate
//
//  Created by Henri Aribert-Desjardins on 23/09/2014.
//
//

#ifndef __version__
#define __version__

// ----------------------------------------------------------

// Défintion des macro-variables de gestion des versions
// ----------------------------------------------------------
// ----------------------------------------------------------


// 32/64 bits version
// ------------------
#define VERSION_32BITS  1 // 0 => version 64 bits

// ici samplePrecision est :
// de type float si version 32 bits
#if VERSION_32BITS
typedef float samplePrecision;
// de type double si version 64 bits
#else
typedef double samplePrecision;
#endif





// VST Versioning
// --------------
#define MAJOR_VERSION_STR "1"       // Version Majeur du plug-in : 1.x.x
#define MAJOR_VERSION_INT 1

#define SUB_VERSION_STR "0"         // première sous version du plug-in x.1.x
#define SUB_VERSION_INT 0

#define RELEASE_NUMBER_STR "0"      // 3ème
#define RELEASE_NUMBER_INT 0

#define BUILD_NUMBER_STR "0"        // 4ème
#define BUILD_NUMBER_INT 0

// stockage du nom de version dans "FULL_VERSION_STR"
#define FULL_VERSION_STR MAJOR_VERSION_STR "." SUB_VERSION_STR "." RELEASE_NUMBER_STR "." BUILD_NUMBER_STR



#define stringOriginalFilename	"HeyMate.vst3"
#if PLATFORM_64
#define stringFileDescription	"HeyMate"
#else
#define stringFileDescription	"HeyMate"
#endif
#define stringCompanyName		"Heimat\0"
#define stringLegalCopyright	"© Heimat"
#define stringLegalTrademarks	"Hum this is my plug-in, have a nice Day"




#endif // __version__
