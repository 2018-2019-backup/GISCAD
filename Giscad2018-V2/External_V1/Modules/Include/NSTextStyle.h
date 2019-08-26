#pragma once
#include <string>
#include "..\Include\NSDirectives.h"

 
class CNSTextStyle//_declspec(dllexport)
{
public:
      CNSTextStyle(void);
      virtual ~CNSTextStyle(void);

private:
      NSSTRING m_strTextStyleName;
	  NSSTRING m_strFontName;
public:
      NSSTRING getTextStyleName();
      NSSTRING getFontName();

      void setTextStyleName(TCHAR* pszTextStyleName);
      void setFontName(TCHAR* strFontName);
};