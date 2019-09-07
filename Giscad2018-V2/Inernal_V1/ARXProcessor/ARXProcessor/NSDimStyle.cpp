#include "StdAfx.h"
#include "..\..\Include\NSDimStyle.h"

CNSDimStyle::CNSDimStyle(void)
{ 
	
}

CNSDimStyle::~CNSDimStyle(void)
{

}
NSSTRING CNSDimStyle::getDimStyleName()
{
	return m_strDimStylename;
}

NSSTRING CNSDimStyle::getFont()
{
	return m_strFont;
}
NSSTRING CNSDimStyle::getLineTypeName()
{
	return m_strLineTypeName;
}
double CNSDimStyle::getTxtHt()
{
	return m_dTxtHt;
}

void CNSDimStyle::setDimStyleName(NSSTRING strDimStylename)
{
	m_strDimStylename = strDimStylename;
}

void CNSDimStyle::setFont(NSSTRING strFont)
{
	m_strFont = strFont;
}

void CNSDimStyle::setLineTypeName(NSSTRING strLineTypeName)
{
	m_strLineTypeName = strLineTypeName;
}

void CNSDimStyle::setTxtHt(double dTxtHt)
{
	m_dTxtHt = dTxtHt;
}
