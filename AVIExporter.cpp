#include "AVIExporter.h"
using std::string;

AVIExporter::AVIExporter(string filename_in, int width_in, int height_in, int bitsPerPixel_in, DWORD codec) {
	filename = filename_in;
	FCCHandler = codec;
	frameRate = 25;
	error = "";
	width = width_in;
	height = height_in;
	bitsPerPixel = bitsPerPixel_in;
	frameNumber = 0;

	aviFile = 0;
	aviStream = 0;
	aviCompressedStream = 0;

	AVIFileInit();
}

AVIExporter::~AVIExporter(void) {

	if(aviCompressedStream) {
		AVIStreamRelease(aviCompressedStream);
	}
	if(aviStream) {
		AVIStreamRelease(aviStream);
	}
	if(aviFile) {
		AVIFileRelease(aviFile);
	}

	AVIFileExit();
}

bool AVIExporter::WriteFrame(LPVOID buffer) {
	if(AVIStreamWrite(aviCompressedStream, frameNumber++, 1, buffer, width*height*bitsPerPixel/8,0,NULL,NULL)) {
		error = "Error while writing AVI frame";
		return false;
	}
	return true;
}

string AVIExporter::FccHandlerToString(DWORD fccHandler) {
	string result = "0000";
	result[3] = (char)(((unsigned int)fccHandler & 0xff000000) >> 24);
	result[2] = (char)(((unsigned int)fccHandler & 0x00ff0000) >> 16);
	result[1] = (char)(((unsigned int)fccHandler & 0x0000ff00) >> 8);
	result[0] = (char)(fccHandler & 0x000000ff);
	return result;
}
bool AVIExporter::Initialize() {
	if(AVIFileOpen(&aviFile, filename.c_str(), OF_CREATE | OF_WRITE, NULL)) {
		error = "Could not open file for writing";
		return false;
	}

	memset(&aviStreamInfo, 0, sizeof(aviStreamInfo));
	aviStreamInfo.fccType = streamtypeVIDEO;
	aviStreamInfo.fccHandler = FCCHandler;
	aviStreamInfo.dwScale = 1;
	aviStreamInfo.dwRate = frameRate;
	aviStreamInfo.dwQuality = -1;
	aviStreamInfo.dwSuggestedBufferSize = width * height * 4;
	SetRect(&aviStreamInfo.rcFrame, 0, 0, width, height);
	strcpy(aviStreamInfo.szName, "3DSSPP Video");

	// DEBUG: finds all the codecs on a machine and shows the description of
	// each in a message box
	/*ICINFO icinfo;
	for(int i = 0; ICInfo(ICTYPE_VIDEO, i, &icinfo); i++) {
		HIC hIC;
		hIC = ICOpen(icinfo.fccType, icinfo.fccHandler, ICMODE_QUERY);
		ICGetInfo(hIC, &icinfo, sizeof(icinfo));
		if(AfxMessageBox(FccHandlerToString(icinfo.fccHandler).c_str(), MB_YESNO) == IDYES) {
			aviStreamInfo.fccHandler = icinfo.fccHandler;
			break;
		}
	}*/

	if(AVIFileCreateStream(aviFile, &aviStream, &aviStreamInfo)) {
		error = "Error while creating AVI stream";
		return false;
	}

	memset(&aviCompressOptions, 0, sizeof(AVICOMPRESSOPTIONS));
	aviCompressOptions.fccType = streamtypeVIDEO;
	aviCompressOptions.fccHandler = aviStreamInfo.fccHandler;
	aviCompressOptions.dwFlags = AVICOMPRESSF_KEYFRAMES | AVICOMPRESSF_VALID;
	aviCompressOptions.dwQuality = 9500;
	aviCompressOptions.dwKeyFrameEvery = 1;

	if(AVIMakeCompressedStream(&aviCompressedStream, aviStream, &aviCompressOptions, NULL) != AVIERR_OK) {
		// TODO is this a correct description of the codec?
		error = "Could not create compressed stream, Microsoft Video 1 codec required";
		return false;
	}

	BITMAPINFO binfo;
	memset(&binfo, 0, sizeof(binfo));
	binfo.bmiHeader.biPlanes = 1;
	binfo.bmiHeader.biWidth = width;
	binfo.bmiHeader.biHeight = height;
	binfo.bmiHeader.biCompression = BI_RGB;
	binfo.bmiHeader.biBitCount = bitsPerPixel;
	binfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	binfo.bmiHeader.biSizeImage = binfo.bmiHeader.biWidth*binfo.bmiHeader.biHeight*binfo.bmiHeader.biBitCount/8;

	if(AVIStreamSetFormat(aviCompressedStream, 0, (LPVOID)&binfo, binfo.bmiHeader.biSize)) {
		error = "Error setting AVI stream format";
		return false;
	}

	return true;

}

string AVIExporter::getError() {
	return error;
}