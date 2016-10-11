#pragma once

class DIB
{
public:
	DIB(void);
	~DIB(void);

	DIB(CWnd* window);

	void GrabWindow(CWnd* window);

	void PutOnClipboard();

	void RenderToDC(CDC* pDC);

private:
	// printing stuff
	HDC m_hMemDC;
	HGLRC m_hMemRC;
	BITMAPINFO m_bmi;
	LPVOID m_pBitmapBits;
	HBITMAP m_hDib;
};
