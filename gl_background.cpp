#include "gl_background.h"
#include "Hom_View.h"
#include "hom_doc.h" // Necessary
#include "Hom.h"
#include "gl_vid.hpp"

GL_Background::GL_Background()
{
   // Invalidate data, since we haven't loaded anything yet
   //lHeightPixels = lWidthPixels = -1;
   currentPicturePathName = "";
}

// Returns true if the current picture file name is different than the currently loaded one
bool GL_Background::Changed()
{
   C_Hom_Doc* pDoc = Get_App_Ptr()->Get_Document_Ptr();

   // If the current picture path is different from the document one,
   //		then the document one has been changed.

   return (currentPicturePathName != pDoc->getPicturePathName());
}

// Returns false if bitmap fails to load
bool GL_Background::Load(CView *view_pointer)
{
   HDC			hdcTemp;												// Temp DC to hold our bitmap
   HBITMAP		hbmpTemp;												// Temp bitmap
   LPPICTURE	pPicture;
   long		lWidth;													// Width(logical units)
   long		lHeight;												// Height(logical units)
   GLint		glMaxTexDim;											// Maximum texture size								
   C_Hom_Doc*	pDoc = Get_App_Ptr()->Get_Document_Ptr();

   view_ptr = view_pointer;											// Set the view pointer to the view that owns the background
   pPicture = pDoc->getBGPictureRef();

   // Set the current picturePathName, since it loaded successfully
   if(pDoc->getPicturePathName() != BG_LOADED_FROM_3DSSPP_FILE)
   {
      // Coming from a jpeg, not from a 3DSSPP file
      currentPicturePathName = pDoc->getPicturePathName();
   }
   else
   {
      // Coming from a 3DSSPP file; no path for image
      pDoc->setPicturePathName("");
      currentPicturePathName = "";
   }


   // Create a temporary device context to render bitmap to
   hdcTemp = CreateCompatibleDC(GetDC(0));
   if(!hdcTemp)	// make sure it was successful
   {
      pPicture->Release();
      AfxMessageBox("Could not create temporary device context to render bitmap");
      return false;
   }

   glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);					// Get Maximum Texture Size Supported

   pPicture->get_Width(&lWidth);													// Get IPicture width
   lWidthPixels	= MulDiv(lWidth, GetDeviceCaps(hdcTemp, LOGPIXELSX), 2540);		// Convert to pixels
   pPicture->get_Height(&lHeight);													// Get IPicture height
   lHeightPixels	= MulDiv(lHeight, GetDeviceCaps(hdcTemp, LOGPIXELSY), 2540);	// Convert to pixels

   // Resize image to closest power of two to ensure compatibility
   if (lWidthPixels <= glMaxTexDim) // Is image width less than or equal to cards limit
      lWidthPixels = 1 << (int)floor((log((double)lWidthPixels)/log(2.0f)) + 0.5f); 
   else  // Otherwise, set width to the highest power of two that the card can handle
      lWidthPixels = glMaxTexDim;

   if (lHeightPixels <= glMaxTexDim) // Is image height greater than card's limit
      lHeightPixels = 1 << (int)floor((log((double)lHeightPixels)/log(2.0f)) + 0.5f);	// Set to lowest 
   else  // Otherwise  Set height to the highest power of two that the card can handle
      lHeightPixels = glMaxTexDim;

   //	Create A Temporary Bitmap
   BITMAPINFO	bi = {0};
   DWORD* pBits;												// Pointer to bitmap bits

   // Fill the bitmap structure
   bi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);				// Set structure size
   bi.bmiHeader.biBitCount		= 32;									// 32 Bit
   bi.bmiHeader.biWidth		= lWidthPixels;					// Width that is a power of 2
   bi.bmiHeader.biHeight		= lHeightPixels;					// Height that is a power of 2
   bi.bmiHeader.biCompression	= BI_RGB;								// RGB encoding
   bi.bmiHeader.biPlanes		= 1;									// 1 Bitplane

   // Create bitmap
   hbmpTemp = CreateDIBSection(hdcTemp, &bi, DIB_RGB_COLORS,(void **)&(pBits), 0, 0);
   if(hbmpTemp == NULL)														// Ensure that it succeeded
   {
      DeleteDC(hdcTemp);												// Delete the DC
      pPicture->Release();											// Decrements IPicture reference count
      AfxMessageBox("Could not load create bitmap");
      return false;
   }

   SelectObject(hdcTemp, hbmpTemp);									// Select Handle To Our Temp DC And Our Temp Bitmap Object

   // Render IPicture to the bitmap
   pPicture->Render(hdcTemp, 0, 0, lWidthPixels, lHeightPixels, 0, lHeight, lWidth, -lHeight, 0);

   // Convert from BGR to RGB Format and set alpha to 255
   for(long i = 0; i < lWidthPixels * lHeightPixels; i++)				// For all pixels ...
   {
      BYTE* pPixel	= (BYTE*)(&(pBits[i]));							// Point to pixel
      BYTE  temp		= pPixel[0];									// Store 1st color in temp
      pPixel[0]		= pPixel[2];									// Move b to r
      pPixel[2]		= temp;											// Move temp to b
      pPixel[3]		= 255;											// Set alpha to 255
   }

   glGenTextures(1, &texture_id);								// Create the texture

   // Generate texture
   glBindTexture(GL_TEXTURE_2D, texture_id);							// Bind to texture ID
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);		// Linear filtering
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);     // Linear filtering
   glTexImage2D(GL_TEXTURE_2D, 0, 3, lWidthPixels, lHeightPixels, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBits);
   return true;
}

void GL_Background::Draw(CDC* pDC)
{
   int center_x, center_y;
   CRect windowRect;
   C_Hom_Doc*	pDoc = Get_App_Ptr()->Get_Document_Ptr();
   CSize HMimgRect;
   float width, height;

   assert(view_ptr != NULL);
   view_ptr->GetWindowRect(&windowRect);

   // Get the height and width of the image
   pDoc->getBGPictureRef()->get_Width(&HMimgRect.cx);
   pDoc->getBGPictureRef()->get_Height(&HMimgRect.cy);

   // Convert the image coordinates to logical units
   pDC->HIMETRICtoLP(&HMimgRect);

   // Set up the projection matrix so the background is not effected by a change to the camera of the hominoid
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   width = float(HMimgRect.cx);
   height = float(HMimgRect.cy);
   // Create the orthogonal view for the background
   glOrtho(-width/2,width/2, 
      -height/2, height/2, 
      -ORIGINAL_WORLD_SIZE, ORIGINAL_WORLD_SIZE);

   // Select the appropriate center
   if(!pDoc->getOpenGLPrinting())	// getOpenGLPrinting is true when in print preview or printing the opengl figure
   {
      center_x = (windowRect.Width()/2);
      center_y = (windowRect.Height()/2);
   } else {
      center_x = STICK_FIG_PRINTING_SIZE/2;
      center_y = STICK_FIG_PRINTING_SIZE/2;
   }

   // Set the viewport according to the "mPictureDisplayMode" (centered/stretched)
   switch(pDoc->getPictureDisplayMode())
   {
   case pCentered:
      glViewport( int(center_x - width/2), int(center_y - height/2), GLsizei(width), GLsizei(height));
      break;
   case pTiled:	// not supported, currently no option for this in the dialog box
      break;
   case pStretch:
      // Make the viewport the width and height of the whole window
      if (!pDoc->getOpenGLPrinting())
      {
         glViewport(0,0,windowRect.Width(),windowRect.Height());
      } else {
         glViewport(0, 0, STICK_FIG_PRINTING_SIZE, STICK_FIG_PRINTING_SIZE);
      }
      break;
   }	
   // Bind the texture
   glBindTexture(GL_TEXTURE_2D, texture_id);

   // The background color is set to white so that it doesn't blend with the texture.
   glColor3f(1.0f,1.0f,1.0f);

   // Draw the quad
   glBegin(GL_QUADS);
      glTexCoord2f(0,0);	glVertex3f(-width/2,-height/2,-ORIGINAL_WORLD_SIZE);
      glTexCoord2f(1,0);	glVertex3f(width/2,-height/2,-ORIGINAL_WORLD_SIZE);
      glTexCoord2f(1,1);	glVertex3f(width/2,height/2,-ORIGINAL_WORLD_SIZE);
      glTexCoord2f(0,1);	glVertex3f(-width/2,height/2,-ORIGINAL_WORLD_SIZE); 
 
   glEnd();


}
