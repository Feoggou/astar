// A_star.h : main header file for the A_star application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CA_starApp:
// See A_star.cpp for the implementation of this class
//

class CA_starApp : public CWinApp
{
public:
	CA_starApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnViewPointschecked();
//	afx_msg void OnUpdateHeuristicNoheuristic(CCmdUI *pCmdUI);
};

extern CA_starApp theApp;