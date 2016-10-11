// hom.h : main header file for the HOM application
//
#ifndef   Include_C_Hom_App
#define   Include_C_Hom_App
#pragma once

#ifndef __AFXWIN_H__
   #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// C_Hom_App:
// See hom.cpp for the implementation of this class
//

#include "factors.hpp"
#include "string.hpp"
#include "quad_frm.h" //for enumerations
#include "HomDocTemplate.hpp"
#include "WindowData.hpp"

#include <map>
#include <vector>
#include <string>

class C_Hom_Doc;
class C_Main_Frame;
class C_Hom_App;

extern C_Hom_App* Get_App_Ptr();
extern C_Main_Frame* MainFramePtr();

enum E_Create_Type
{
   CT_INITIAL,
   CT_NEW,
   CT_OPEN,
   CT_OPEN_INITIAL,
   CT_IMPORT_42x
};

enum { VIEW_TOP, VIEW_FRONT, VIEW_SIDE, VIEW_OBLIQUE };

class C_Hom_App : public CWinApp
{
    public:
	    C_Hom_App();

        // Overrides
	    virtual BOOL InitInstance();  
	    virtual int  ExitInstance();
	    virtual void OnFileNew();
	    virtual void OnFileOpen();

        // Map of OpenGL view templates
        std::map<int,HomDocTemplate*> mViewTemplateMap;
		// Map of OpenGL view templates for printing
		std::map<int,HomDocTemplate*> mPrintViewTemplateMap;
        WindowData*	mNewWindowDataPtr;

		CString GetLicenseName();

    protected:
	    bool CheckForSave();	// Helper for OnFileNew, OnFileOpen

        public:
        void Create_Document( E_Create_Type CreateType );

        C_Main_Frame*		GetMainFramePtr();
        C_Hom_Doc*			Get_Document_Ptr();

        const CString& ApplicationName() const;
        //bool TwoDModeOnly() const;

		// name of file that was double clicked to open
		CString openfilename;

        //BOOL Is_Demonstration();

        //BOOL SiteData(CString szRegKey = "");
        BOOL Log_On();
        CString Intro_Text_1();
        //CString Intro_Text_2();
        //CString Intro_Text_3();
        //bool CreateMD5Hash(CString &plaintext, CString &hashed);

        //--- Configuration
        CString mApplicationName;
        bool mRunInvisible;
           
        //--- Document Template Pointers
        CMultiDocTemplate* Dib_DocTemplate_Ptr;
        CMultiDocTemplate* mStatusResultsDocTmpPtr;
        CMultiDocTemplate* mAnimationBarDocTmpPtr;

		CMultiDocTemplate* mDummyTemplatePtr;

        // Map of report templates
        std::map<int,CMultiDocTemplate*> mReportTemplateMap;

        // Map of OpenGL view templates
        //std::map<int,HomDocTemplate*> mViewTemplateMap;

		CString		LicenseName;
           
        //--- Cursors
        HCURSOR   mArrowCursorHdl;
        HCURSOR   mCrossCursorHdl;
        HCURSOR   mSelectableCursorHdl;

        BOOL Document_Exists;
           
        //--- Associated Classes ---
        friend class C_Hom_Doc;
        friend class C_Main_Frame;

        // Implementation

        // Server object for document creation
        //COleTemplateServer m_server;

        //{{AFX_MSG(C_Hom_App)
        afx_msg void OnAppAbout();
        afx_msg void OnUpdateAppAbout(CCmdUI* pCmdUI);
        afx_msg void OnLicense();
        afx_msg void OnRegistration();
        afx_msg void OnRestoreWorkspace();
        afx_msg void OnAppExit();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

    private:
        bool mRestoringDefaultWorkspace;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

/////////////////////////////////////////////////////////////////////////////
#endif

