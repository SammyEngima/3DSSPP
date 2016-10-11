#pragma once
#include "afx.h"
#include "afxpriv.h"

/* OfflineSafeCFile - 6/02/2006 CJB
 * This class exists solely to workaround a bug in mfc which causes
 * a failure to load or save files which are "offline files".
 * In CFile::GetFilePath, GetStatus is called, which, among other things,
 * reads and converts file times, including 'Created' time.  Offline files
 * do not store this value and an exception is thrown during the conversion
 * since the time defaults to 0 when it is not found.
 * OfflineSafeCFile overrides this function so GetStatus is not called,
 * and the file can still be saved or opened correctly
 *
 * Note that this derives from CFile, not CMirrorFile.  CMirrorFile is a subclass
 * of CFile which writes to a new file until a Close() when it overwrites the old file,
 * or Abort() when it gets deleted with the old file still intact. However, CMirrorFile::Open
 * also causes the offline file error, and so we are sacrificing the safety precaution supplied
 * by CMirrorFile in favor of actually being able to save and load offline files
 */

class OfflineSafeCFile :
	public CFile
{
public:
	OfflineSafeCFile(void);
	~OfflineSafeCFile(void);
	CString GetFilePath() const;
};
