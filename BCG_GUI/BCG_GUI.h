// BCG_GUI.h : main header file for the BCG_GUI application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "../include/OSGBIM/OSGBIM.h"

// CBCG_GUIApp:
// See BCG_GUI.cpp for the implementation of this class
//

class CBCG_GUIApp : public CBCGPWinApp
{
public:
	CBCG_GUIApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	virtual BOOL SaveAllModified();
	CViewManager* GetViewManager();
// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
private:
	CViewManager *_view;
};

extern CBCG_GUIApp theApp;