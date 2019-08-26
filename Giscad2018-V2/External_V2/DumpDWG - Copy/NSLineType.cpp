//#include "StdAfx.h"
#include "..\Include\NSLineType.h"


CNSLineType::CNSLineType(void)
{
}

CNSLineType::~CNSLineType(void)
{
}


NSSTRING CNSLineType::getLineTypeName()
{
	return strLineTypeName;
}

NSSTRING CNSLineType::getAsciiDescription()
{
	return strAsciiDescription;
}

void CNSLineType::setAsciiDescription(NSSTRING strAsciiDesc)
{
	strAsciiDescription = strAsciiDesc;
}

void CNSLineType::setLineTypeName(NSSTRING strParamLineTypeName)
{
	strLineTypeName = strParamLineTypeName;
}