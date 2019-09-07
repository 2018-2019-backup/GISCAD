#include "StdAfx.h"
#include "..\Include\NSLayer.h"
#include <string>


CNSLayer::CNSLayer()
{

}
//CNSLayer::~CNSLayer()
//{
//
//}

NSSTRING CNSLayer::getLayerName()
{
	return m_strLayername;
}
int CNSLayer::getColor()
{
	return m_nColor;
}
int CNSLayer::getLineWt()
{
	return m_nLineWt ;
}
bool CNSLayer::getIsFrozen()
{
	return m_bIsFrozen;
}
bool CNSLayer::getIsLocked()
{
	return m_bIsLocked;
}
bool CNSLayer::getIsOff()
{
	return m_bIsOff;
}
bool CNSLayer::getPlotState()
{
	return m_bPlotState;
}
NSSTRING CNSLayer::getLineTypeName()
{
	return m_strLinetypeName;
}
NSSTRING CNSLayer::getPlotStyleName()
{
	return m_strPlotStyleName;
}

void CNSLayer::setLayername(TCHAR *pszlayerName)
{
	m_strLayername = (NSSTRING)pszlayerName;
}
void CNSLayer::setColor(int nColor)
{
	m_nColor = nColor;
}
void CNSLayer::setLineWt(int nLineWt)
{
	m_nLineWt = nLineWt;
}
void CNSLayer::setIsFrozen(bool bIsFrozen)
{
	m_bIsFrozen = bIsFrozen;
}
void CNSLayer::setIsLocked(bool bIsLocked)
{
	m_bIsLocked = bIsLocked;
}
void CNSLayer::setIsOff(bool bIsOff)
{
	m_bIsOff = bIsOff;
}
void CNSLayer::setPlotState(bool bPlotState)
{
	m_bPlotState = bPlotState;
}
void CNSLayer::setLineTypeName(TCHAR *pszLineTypename)
{
	m_strLinetypeName = (NSSTRING)pszLineTypename;
}
void CNSLayer::setPlotStyleName(TCHAR *pszPlotStylename)
{
	m_strPlotStyleName = pszPlotStylename;
}
