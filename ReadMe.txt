To build stdApp:
----------------

1) The "tools/options/directories/include" list should be as follows (assuming your root drive 
is C: and MS Visual C++ is installed under C:\Program Files\Microsoft Visual Studio)

	C:\Program Files\Microsoft Visual Studio\VC98\INCLUDE
	C:\Program Files\Microsoft Visual Studio\VC98\MFC\INCLUDE
	C:\Program Files\Microsoft Visual Studio\VC98\ATL\INCLUDE
	Y:\aryLib
	Y:\c4eLib
	Y:\dbsLib
	Y:\geoLib
	Y:\graLib
	Y:\homLib
	Y:\stdApp

2) Make sure "tools/options/directories/include" does not have any other project directories 
that include a "stdafx.h" in front of the c4e directories.

3) If you have built aniApp before building aniApp, make sure you "clean" the common
libraries: aryLib, c4eLib, dbsLib, geoLib and graLib so they will be rebuilt using the
directory path list for this application.

4) The resulting stdApp executable supports both 3DSSPP and 2DSSPP. You select the 
product type when creating the site file using the site file program built by
sflApp. NOTE: To restrict COM server use of this application to 2D, you must
create a different executable. See the notes at the top of SiteData.cpp in the
directory C:\Dvl\c4e97Wsp\stdApp.

5) Do not remove the "_DEBUG" preprocessor define from the release mode project settings.
It does not cause a release build. Its sole effect in this instance is to invoke some
additional code in the header files whose inclusion stems from "stdafx.hpp". This
preprocessor define has no effect on 3DSSPP source code at this time. This define is a 
workaround to a Microsoft bug introduced in MSVC 6.0 in which substantial changes were
made to the MFC (Microsoft Foundation Class) libraries by Microsoft. 


COM Automation Server Notes
---------------------------

1) Building this application will also create a file called stdApp.tlb which can
be used by other development environments like VisualBasic for buidling applications
that run 3DSSPP as an automation server.

2) The project comClientApp can be built to demonstrate running 3DSSPP as an
automation server from C++ programs. It uses the proxy classes declared and defined by
IUM3DSSPP.hpp and IUM3DSSPP.cpp for accessing the 3DSSPP interface.

3) To register 3DSSPP on the system where you wish to run it as an automation server,
simply start and exit the program in normal stand-alone mode once.


Copyright Notice
----------------

The copyright date is located in the following places:

	File: hom_doc.cpp;  Function: ReportFooter;  line 1226
	File: logon.cpp;    Function: Intro_Text_1;  line   62
	Dialog Resource: IDD_ABOUTBOX


Version Number
--------------

The version number is located in the following places:

	String Resource       : IDR_HOMTYPE
	String Resource       : AFX_IDS_APP_TITLE
	stdApp\AppVersion.hpp : szVERSION_NUMBER


Executable Name
---------------

The executable name is located under the stdApp project settings, 

	page:     "Link"
	category: "General", 
	edit box: "Output file name:"

Luke's Additions 5/11/04
------------------------

To compile a Release version in .NET 2003:

	- Go to SSPPApp's Properties page.
	- Change to Release mode by clicking the Configuration Manager button and
		changing to Release
	- Under Linker->Input->Additional Dependencies, add ..\Libs\*.lib
	- For each other project (aryLib, c4eLib, etc.):
		- Open its properties page
		- Under Configuration Properties->General, set the output
			directory to ..\Libs

	- Make sure _3DSSPP_ALPHA (in AppVersion.hpp) is undefined as well -
		we want the release, not alpha or beta.
		
	- If you have to map a drive for some reason, use
		subst y: "c:\whatever\directory"
