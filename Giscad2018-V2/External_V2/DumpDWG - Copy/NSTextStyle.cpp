
//#include "StdAfx.h"
#include <stdlib.h>
#include "..\Include\NSTextStyle.h"
#include <windows.h>

CNSTextStyle::CNSTextStyle(void)
{

}

CNSTextStyle::~CNSTextStyle(void)
{

}

NSSTRING CNSTextStyle::getTextStyleName()
{
      return m_strTextStyleName;
}

NSSTRING CNSTextStyle::getFontName()
{
	return m_strFontName;
}

void CNSTextStyle::setTextStyleName(TCHAR* pszTextStyleName)
{
	m_strTextStyleName = (NSSTRING)pszTextStyleName;
}


void CNSTextStyle::setFontName(TCHAR* pszFontName)
{
	m_strFontName = (NSSTRING)pszFontName;
}