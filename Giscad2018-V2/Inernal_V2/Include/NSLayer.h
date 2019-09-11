#pragma once
#include "stdafx.h"
#include <string>
#include "..\..\Include\NSDirectives.h"

class _declspec(dllexport) CNSLayer
{
public:
	CNSLayer(void);
	virtual ~CNSLayer(void);
private:	
	NSSTRING m_strLayername;
	int m_nColor;
	bool m_bIsFrozen;
	bool m_bIsLocked;
	bool m_bIsOff;
	bool m_bPlotState;
	int m_nLineWt;
	NSSTRING m_strLinetypeName;
	NSSTRING m_strPlotStyleName;
public:
	NSSTRING getLayerName();
	int getColor();
	bool getIsFrozen();
	bool getIsLocked();
	bool getIsOff();
	bool getPlotState();
	int getLineWt();
	NSSTRING getLineTypeName();
	NSSTRING getPlotStyleName();

	void setLayername(TCHAR *pszlayerName);
	void setColor(int nColor);
	void setIsFrozen(bool bIsFrozen);
	void setIsLocked(bool bIsLocked);
	void setIsOff(bool bIsOff);
	void setPlotState(bool bPlotState);
	void setLineWt(int nLineWt);
	void setLineTypeName(TCHAR *pszLineTypename);
	void setPlotStyleName(TCHAR *pszPlotStylename);
};