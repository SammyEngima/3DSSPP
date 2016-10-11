#ifndef HomDocTemplateMcr
#define HomDocTemplateMcr

class HomDocTemplate : public CMultiDocTemplate
{
public:

	HomDocTemplate(	UINT nIDResource,
							CRuntimeClass* pDocClass,
							CRuntimeClass* pFrameClass,
							CRuntimeClass* pViewClass	);

   virtual CDocument* OpenDocumentFile(	LPCTSTR lpszPathName,
														BOOL bMakeVisible	);
};

#endif

