// BIM_GUI.h : main header file for the BIM_GUI application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "../include/OSGBIM/OSGBIM.h"

// CBIM_GUIApp:
// See BIM_GUI.cpp for the implementation of this class
//

class CBIM_GUIApp : public CBCGPWinApp
{
public:
	CBIM_GUIApp();
	CViewManager* GetViewManager(){ return _viewMgr; };
	// Override from CBCGPWorkspace
	virtual void PreLoadState ();
private:
	CViewManager *_viewMgr;

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBIM_GUIApp theApp;