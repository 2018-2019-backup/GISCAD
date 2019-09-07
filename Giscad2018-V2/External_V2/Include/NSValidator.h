/*!
 @Class			CNSValidator
 @Brief			This class provides the functionality to check file read-only,
				file/directory is present or not, to get the extension of the
				file, to check file time stamps.
 @Author		Neilsoft Ltd
 @Date			31st May 2007
 @History 		Change history
 */
#pragma once

#include "stdafx.h"
#include <iostream>
#include "NSDirectives.h"
class __declspec(dllexport) CNSValidator//__declspec(dllexport)
{
private:
	__time64_t getFileLastModified(TCHAR *szFileName);
public:
	CNSValidator(void){};
	int checkFileStamps(TCHAR *szServerFileName, TCHAR *szLocalFileName);
	int overWriteFile(TCHAR *szServerFileName, TCHAR *szLocalFileName);
	int isFilePresent(TCHAR *szFileName);
	int isDirectoryExists(TCHAR *szFileName);
	bool isFileReadOnly(TCHAR *szFileName);
	int getFileExtension(TCHAR *szFileName, TCHAR szExtension[]);
	bool createDSN(TCHAR* szMDBLoc);
public :
	enum{
		VALIDATOR_INVALID_FILE_PATH = 125,
	};
public:
	~CNSValidator(void){};
};
