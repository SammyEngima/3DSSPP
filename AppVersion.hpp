// AppVersion.hpp : header file

/*ACT************************************************************************
 New features can be compiler defined to only be compiled if the version
 number is beyond a certian point, OR the alpha flag is set.

 For example - If you're developing a version for release, and have features
 you don't wish to include in that release, you can surround the section of
 new code by (if your release version were minor version 40):

 #if (VERSION_NUMBER_MINOR > 40)
		... new code ...
 #else
		... legacy code ...
 #endif

 Using this setup, you can alter the VERSION_NUMBER_MINOR and work on the
 new features, and still easily revert back to the code for the release
 build in development.

 I recommend as soon as the release build is created, the code in its
 entirety be archived as "Final Build vX.XX" and the development copy of
 the code have all compiler defines and code that was required because of
 the older and now released version, be deleted.  This will help keep the
 code un-cluttered by unused segments, and because the old copy is archived,
 any code you need out of the released version can still be found.

//**************************************************************************/

//////// Keep these #defines updated ////////
//
// Build version
//#define _3DSSPP_ALPHA   // Define if development build
//#define _3DSSPP_BETA  // Define if version complete,
                        // waiting for verification before release

#define szBUILD_NUMBER "1"  // Increment each time a copy is sent to Chuck.

#define VERSION_NUMBER_MAJOR 7
#define VERSION_NUMBER_MINOR 0
#define szVERSION_NUMBER "7.0.0"

#define szCOPYRIGHT_YEAR "2016"

// Population
#define	szDEFAULT_POPULATION_NAME "Production 7.0.0"

/////////////////////////////////////////////

// Construct the version string
#if defined _3DSSPP_ALPHA
   // If this is an alpha build, put the build number and then
   //    "alpha" after the version number text
   #define szVERSION_TYPE "." ## szBUILD_NUMBER ## " alpha"
#elif defined _3DSSPP_BETA
   // If this is a beta, just put "beta" after the version number text
   #define szVERSION_TYPE " beta"
#else
   // Release build? leave the version number text alone
   #define szVERSION_TYPE ""
#endif

// Set szVERSION to the conglomeration of the number and the build type
#define szVERSION "" ## szVERSION_NUMBER ## "" ## szVERSION_TYPE ## ""

#define SZ_APP_TITLE "Univ. of Michigan's 3DSSPP " ## szVERSION
