/*!
 @Class			CNSCadData
 @Brief			This class provides functionality to hold
				CAD information.
 @Author		Neilsoft Ltd
 @Date			07-06-2007
 @History 		Change history
 */
#pragma once
#include "string.h"
#include "..\..\Include\NSDirectives.h"

class CNSCadData
{
public:
	CNSCadData(void);
	void setSourceFileName(const TCHAR* szFileName);
	void setDestinationFileName(const TCHAR* szFileName);
	const TCHAR* getSourceFileName();
	const TCHAR* getDestinationFileName();
public:
	~CNSCadData(void);
private:
	const TCHAR* m_szSourceFileName;
	const TCHAR* m_szDestinationFileName;
};
