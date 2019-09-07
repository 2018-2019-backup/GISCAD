#pragma once
#include <string>
#include "..\Include\NSDirectives.h"

class CNSDimStyle//_declspec(dllexport)
{
public:
	CNSDimStyle(void);
	virtual ~CNSDimStyle(void);

private:
	NSSTRING m_strDimStylename;
	NSSTRING m_strFont;
	NSSTRING m_strLineTypeName;
	double m_dTxtHt;

public:
	NSSTRING getDimStyleName();
	NSSTRING getFont();
	NSSTRING getLineTypeName();
	double getTxtHt();

	void setDimStyleName(NSSTRING strDimStylename);
	void setFont(NSSTRING strFont);
	void setLineTypeName(NSSTRING strLineTypeName);
	void setTxtHt(double dTxtHt);
};
