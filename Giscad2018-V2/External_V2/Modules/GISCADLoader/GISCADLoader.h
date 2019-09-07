/*!
 @Class			CGISCADLoaderApp
 @Brief			This class is inherited by the CWinApp class
 @Author		Neilsoft Ltd
 @Date			12-07-2007
 @History 		Change history
*/
#pragma once
#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif
#include "resource.h"		// main symbols

// CGISCADLoaderApp:
// See GISCADLoader.cpp for the implementation of this class
class CGISCADLoaderApp : public CWinApp
{
public:
	CGISCADLoaderApp();
// Overrides
	public:
	virtual BOOL InitInstance();
// Implementation
	DECLARE_MESSAGE_MAP()
};
extern CGISCADLoaderApp theApp;