#include "stdafx.h"

#include "HomDocTemplate.hpp"
#include "hom.h"
#include "hom_doc.h"	// Necessary

#ifdef AFX_CORE2_SEG
#pragma code_seg(AFX_CORE2_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

//---------------------------------------------------------------------------
HomDocTemplate::HomDocTemplate(	UINT nIDResource,
											CRuntimeClass* pDocClass,
											CRuntimeClass* pFrameClass,
											CRuntimeClass* pViewClass	)
	: CMultiDocTemplate( nIDResource, pDocClass, pFrameClass, pViewClass )
{
   return;
}

//---------------------------------------------------------------------------
CDocument* HomDocTemplate::OpenDocumentFile(	LPCTSTR lpszPathName,
															BOOL bMakeVisible	)
{
	CDocument* pDocument = CreateNewDocument();
	if (pDocument == NULL)
	{
		TRACE0("CDocTemplate::CreateNewDocument returned NULL.\n");
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		return NULL;
	}
	ASSERT_VALID(pDocument);

	BOOL bAutoDelete = pDocument->m_bAutoDelete;
	pDocument->m_bAutoDelete = FALSE;   // don't destroy if something goes wrong

	if (lpszPathName == NULL)
	{
		// create a new document - with default document name
		SetDefaultTitle(pDocument);

		// avoid creating temporary compound file when starting up invisible
		if (!bMakeVisible)
			pDocument->m_bEmbedded = TRUE;

		if (!pDocument->OnNewDocument())
		{
			// user has be alerted to what failed in OnNewDocument
			TRACE0("CDocument::OnNewDocument returned FALSE.\n");

			return NULL;
		}

		// it worked, now bump untitled count
		m_nUntitledCount++;
	}
	else
	{
		// open an existing document
		CWaitCursor wait;
		if (!pDocument->OnOpenDocument(lpszPathName))
		{
			// user has be alerted to what failed in OnOpenDocument
			TRACE0("CDocument::OnOpenDocument returned FALSE.\n");
			
			// Kill pDocument; it's useless now (Added by LCW 4/2/04)
			delete pDocument;
			
			return NULL;
		}
#ifdef _MAC
		// if the document is dirty, we must have opened a stationery pad
		//  - don't change the pathname because we want to treat the document
		//  as untitled
		if (!pDocument->IsModified())
#endif
			pDocument->SetPathName(lpszPathName);
	}

   //For running as a server, we must create a frame here

	if ( !bMakeVisible )
   {
      Get_App_Ptr()->mNewWindowDataPtr = ((C_Hom_Doc*)pDocument)->getObliqueWindowDataPtr();
	   CFrameWnd* pFrame = CreateNewFrame(pDocument, NULL);
	   
	   pDocument->m_bAutoDelete = bAutoDelete;
	   if (pFrame == NULL)
	   {
	   	AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
	   	delete pDocument;       // explicit delete on error
	   	return NULL;
	   }
	   ASSERT_VALID(pFrame);
	   InitialUpdateFrame(pFrame, pDocument, bMakeVisible);
   }

	return pDocument;
}


