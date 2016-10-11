// cdib.h
#ifndef   CDIB_INCLUDE 
#define   CDIB_INCLUDE

class CDib : public CObject
{
    DECLARE_SERIAL(CDib)
private:
    #ifndef WIN32
       char huge* m_lpBuf;    // DIB data buffer
    #else
       char* m_lpBuf;
    #endif
    DWORD      m_dwLength; // total buffer length, including file header
    int        m_nBits;    // number of color bits per pixel
    //pointers for internal use
    LPBITMAPFILEHEADER m_lpBMFH;
    LPBITMAPINFOHEADER m_lpBMIH;
    LPBITMAPINFO       m_lpBMI;
    LPSTR              m_lpData; 
public:
    CDib();
    CDib( CWnd*   WindowPtr );
    CDib(CDC* pDC, int nBt = 0, BOOL bCompr = FALSE);
    ~CDib();
    virtual void Serialize(CArchive &ar);
    BOOL Read(CFile* pFile);
    BOOL Write(CFile* pFile);
    CBitmap* MakeBitmap(CDC* pDC, CSize& bmSize); // bmsize returned
    BOOL Display(CDC*, CPoint origin);
    BOOL Stretch(CDC*, CPoint origin, CSize size);
    int GetColorBits();  // bits per pixel
    DWORD GetLength();
    CSize GetSize();
    void SetMonoColors(DWORD dwForeground, DWORD dwBackground);
    BOOL GetMonoColors(DWORD& dwForeground, DWORD& dwBackground);

    HANDLE BitMapInfoHdl( void );

    BOOL Is_Empty();
    void   Best_Fit_To_Printer( CDC*   PrinterDCPtr );
    void   Best_Fit_To_Window( CDC*   DCPtr, CWnd*   WindowPtr );
private:
    void Desktop_DIB( void );
    void Window_DIB( CWnd*   WindowPtr );
    void Memory_DC_To_DIB(CDC* pDC, int nBt = 0, BOOL bCompr = FALSE);
    void Best_Fit_To_DC_Rect( CDC* DCPtr, const CRect& PixelRect );
    BOOL AllocateMemory(BOOL bRealloc = FALSE);
};
#endif 
