#include "GLBasic.h"
#include "Hom_Doc.h"
#include "Hom.h"
#include <string>

//function pointer typdefs
typedef bool (APIENTRY *PFNWGLEXTSWAPCONTROLPROC) (int);
typedef int (*PFNWGLEXTGETSWAPINTERVALPROC) (void);
 
//declare functions
PFNWGLEXTSWAPCONTROLPROC wglSwapIntervalEXT = NULL;
PFNWGLEXTGETSWAPINTERVALPROC wglGetSwapIntervalEXT = NULL;

// default pixel format descriptor
const PIXELFORMATDESCRIPTOR DefaultPFD =				// pfd Tells Windows How We Want Things To Be
{
	sizeof(PIXELFORMATDESCRIPTOR),					// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Drawing to a window
		PFD_SUPPORT_OPENGL |						// Must support OpenGL,
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,											// Select Our Color Depth [32 bit?]
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,										// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
};

// pixel format descriptor for printing opengl windows
const PIXELFORMATDESCRIPTOR PrintPFD =
{
	sizeof(PIXELFORMATDESCRIPTOR),					// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_BITMAP |						// Format Must Support Drawing to a window
		PFD_SUPPORT_OPENGL |						// Must support OpenGL,
		PFD_SUPPORT_GDI,
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,											// Select Our Color Depth [32 bit?]
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,										// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
};

bool GLBasic::VSYNCfailed = false;

GLBasic::GLBasic(CWnd* owner_in) : owner(owner_in) {
	quadric = 0;
	gl_hRC = 0;
	pDoc = Get_App_Ptr()->Get_Document_Ptr();
	viewingRect = CRect(0,0,100,100);
	sharee = false;
}

GLBasic::~GLBasic(void) {
	if(!sharee) {
		if(quadric)
			gluDeleteQuadric(quadric);
		if(gl_hRC)
			wglDeleteContext(gl_hRC);
		if(gl_hDC)
			::ReleaseDC(owner->GetSafeHwnd(), gl_hDC);
	}
}

// do pre-draw work, call draw, do post-draw work
void GLBasic::onDraw() {
	if(wglMakeCurrent(gl_hDC, gl_hRC)) {
		glClearColor(clearColor[0], clearColor[1], clearColor[2], 1);
		glClear(GL_COLOR_BUFFER_BIT);

		draw();
		SwapBuffers(gl_hDC);

	}
	wglMakeCurrent(0,0);

	// TODO
	// this reduces to
	// onDraw(gl_hDC, gl_hRC);
}
void GLBasic::onDraw(HDC hDC, HGLRC hRC) {
	if(wglMakeCurrent(hDC, hRC)) {
		//glClearColor(clearColor[0], clearColor[1], clearColor[2], 1);
		//glClear(GL_COLOR_BUFFER_BIT);
		
		draw();
		//SwapBuffers(hDC);
	}
	wglMakeCurrent(0,0);
}

void GLBasic::drawToMemory(LPVOID buffer) {
	if(wglMakeCurrent(gl_hDC, gl_hRC)) {
		glClearColor(clearColor[0], clearColor[1], clearColor[2], 1);
		glClear(GL_COLOR_BUFFER_BIT);

		draw();
		glReadBuffer(GL_BACK);
		glReadPixels(0, 0, viewingRect.Width(), viewingRect.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, buffer);
	}
	wglMakeCurrent(0,0);
}

void GLBasic::setViewport(CRect rect) {
	viewingRect = rect;
}
void GLBasic::setClearColor(float color[3]) {
	for(int i = 0; i < 3; i++)
		clearColor[i] = color[i];
}

bool GLBasic::initialize(CWnd* owner_in, bool printwindow) {
	if(owner_in) {
		owner = owner_in;
	}

	gl_hDC = GetDC(owner->m_hWnd);

	//const PIXELFORMATDESCRIPTOR* pfd = printwindow ? &PrintPFD : &DefaultPFD;

	//const PIXELFORMATDESCRIPTOR* pfd = &PrintPFD;
	const PIXELFORMATDESCRIPTOR* pfd = &DefaultPFD;

	GLuint	PixelFormat;
	// Did Windows Find A Matching Pixel Format?
	if (!(PixelFormat = ChoosePixelFormat(gl_hDC, pfd)))
	{
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}
	
	// Are We Able To Set The Pixel Format?
	if(!SetPixelFormat(gl_hDC,PixelFormat, pfd))
	{
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}
	
	// Are We Able To Get A Rendering Context?
 	if (!(gl_hRC = wglCreateContext(gl_hDC)))
	{
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}
		
	// Try To Activate The Rendering Context
	if(!wglMakeCurrent(gl_hDC, gl_hRC))
	{
		// We just made it, but can't use it. Kill the render context.
		wglDeleteContext(gl_hRC);
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	if(!VSYNCfailed && !printwindow) {
		int errNo;
		// Turn off VSync so we can render at our frame rate
		//get extensions of graphics card
		char* extensions = (char*)glGetString(GL_EXTENSIONS);

		if(!extensions) {
			VSYNCfailed = true;
			errNo = 0;
		} else {

			//is WGL_EXT_swap_control in the string? VSync switch possible?
			if (strstr(extensions,"WGL_EXT_swap_control")) {
				//get address's of the VSYNC setting function
				wglSwapIntervalEXT = (PFNWGLEXTSWAPCONTROLPROC) wglGetProcAddress("wglSwapIntervalEXT");
				if(wglSwapIntervalEXT) {
					wglSwapIntervalEXT(0); //disable
				} else {
					VSYNCfailed = true;
					errNo = 1;
				}
			} else {
				// try to get address anyway?
				wglSwapIntervalEXT = (PFNWGLEXTSWAPCONTROLPROC) wglGetProcAddress("wglSwapIntervalEXT");
				if(wglSwapIntervalEXT) {
					wglSwapIntervalEXT(0);
				} else {
					VSYNCfailed = true;
					errNo = 2;
				}
			}
		}

		if(VSYNCfailed) {
			switch(errNo) {
			case 0:
				MessageBox(NULL, "Could not disable VSYNC.  If your graphics card supports manually disabling VSYNC, please do so. "
							"Otherwise, the program may run slowly. Error 0", "ERROR", MB_OK|MB_ICONEXCLAMATION);
				break;
			case 1:
				MessageBox(NULL, "Could not disable VSYNC.  If your graphics card supports manually disabling VSYNC, please do so. "
							"Otherwise, the program may run slowly. Error 1", "ERROR", MB_OK|MB_ICONEXCLAMATION);
				break;
			case 2:
				MessageBox(NULL, "Could not disable VSYNC.  If your graphics card supports manually disabling VSYNC, please do so. "
							"Otherwise, the program may run slowly. Error 2", "ERROR", MB_OK|MB_ICONEXCLAMATION);
				break;
			}
		}
	}

	quadric = gluNewQuadric();
	init();

	wglMakeCurrent(0,0);

	return true;
}

bool GLBasic::init() {
	return true;
}

void GLBasic::setPrintPFD() {
	GLuint	PixelFormat;
	// Did Windows Find A Matching Pixel Format?
	if (!(PixelFormat = ChoosePixelFormat(gl_hDC,&PrintPFD)))
	{
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
	}
	
	// Are We Able To Set The Pixel Format?
	if(!SetPixelFormat(gl_hDC,PixelFormat,&PrintPFD))
	{
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
	}
}
void GLBasic::setDefaultPFD() {
	GLuint	PixelFormat;
	// Did Windows Find A Matching Pixel Format?
	if (!(PixelFormat = ChoosePixelFormat(gl_hDC,&DefaultPFD)))
	{
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
	}
	
	// Are We Able To Set The Pixel Format?
	if(!SetPixelFormat(gl_hDC,PixelFormat,&DefaultPFD))
	{
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
	}
}

CRect GLBasic::getViewingRect() {
	return viewingRect;
}

void GLBasic::drawCOG(void)
{
	COG = true;
}

void GLBasic::notDrawCOG(void)
{
	COG = false;
}

void GLBasic::notDrawSF(void)
{
	SF = false;
}

void GLBasic::drawSF(void)
{
	SF = true;
}

bool GLBasic::share(GLBasic* source) {

	// check that the sharer is valid
	if(!source->gl_hDC || !source->gl_hRC) return false;

	// copy sharer info
	this->gl_hDC = source->gl_hDC;
	this->gl_hRC = source->gl_hRC;
	this->gl_hWnd = source->gl_hWnd;
	this->owner = source->owner;
	this->quadric = source->quadric;

	// mark this as a sharee
	sharee = true;

	// make current and call init
	wglMakeCurrent(gl_hDC, gl_hRC);
	if(!init()) {
		wglMakeCurrent(0,0);
		return false;
	}
	wglMakeCurrent(0,0);

	return true;
}