#pragma once

#include "stdafx.h"
#include <vfw.h>
#include <string>

class AVIExporter {
public:

	AVIExporter(std::string filename, int width, int height, int bitsPerPixel, DWORD codec = mmioFOURCC('M','S','V','C'));
	virtual ~AVIExporter(void);

	bool WriteFrame(LPVOID buffer);

	bool Initialize();

	std::string FccHandlerToString(DWORD fccHandler);

	std::string getError();

private:

	PAVIFILE aviFile;
	PAVISTREAM aviStream;
	PAVISTREAM aviCompressedStream;
	AVISTREAMINFO aviStreamInfo;
	AVICOMPRESSOPTIONS aviCompressOptions;
	int frameRate;
	DWORD FCCHandler;
	std::string filename;
	int bitsPerPixel;
	int width;
	int height;

	int frameNumber;

	std::string error;	
};
