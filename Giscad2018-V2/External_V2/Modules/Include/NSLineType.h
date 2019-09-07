#pragma once
#include <string>
#include "..\Include\NSDirectives.h"


class CNSLineType//_declspec(dllexport)
{
public:
	CNSLineType(void);
	virtual ~CNSLineType(void);

private:
	NSSTRING strLineTypeName;
	NSSTRING strAsciiDescription ;

public:
	NSSTRING getLineTypeName();
	NSSTRING getAsciiDescription();

	void setLineTypeName(NSSTRING strLineTypeName);
	void setAsciiDescription(NSSTRING strAsciiDescription);
};
