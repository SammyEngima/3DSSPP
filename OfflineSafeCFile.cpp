#include "stdafx.h"
#include ".\offlinesafecfile.h"

OfflineSafeCFile::OfflineSafeCFile(void) : CFile()
{
}

OfflineSafeCFile::~OfflineSafeCFile(void)
{
}

// CFile::GetFilePath calls GetStatus just to find the path, which is unecessary.
// furthermore, when working with offline files, it caused an exception to be thrown
// note that m_strFileName is, in fact, the entire path, eg "C:\whatever\stuff\file.ext"
CString OfflineSafeCFile::GetFilePath() const
{
	return m_strFileName;
}
