
// CRunShell64.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CCRunShell64App:
// See CRunShell64.cpp for the implementation of this class
//

class CCRunShell64App : public CWinApp
{
public:
	CCRunShell64App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CCRunShell64App theApp;
