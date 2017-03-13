// BIM_GUIDoc.cpp : implementation of the CBIM_GUIDoc class
//

#include "stdafx.h"
#include "BIM_GUI.h"

#include "BIM_GUIDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBIM_GUIDoc

IMPLEMENT_DYNCREATE(CBIM_GUIDoc, CDocument)

BEGIN_MESSAGE_MAP(CBIM_GUIDoc, CDocument)
END_MESSAGE_MAP()


// CBIM_GUIDoc construction/destruction

CBIM_GUIDoc::CBIM_GUIDoc()
{
	// TODO: add one-time construction code here

}

CBIM_GUIDoc::~CBIM_GUIDoc()
{
}

BOOL CBIM_GUIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CBIM_GUIDoc serialization

void CBIM_GUIDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CBIM_GUIDoc diagnostics

#ifdef _DEBUG
void CBIM_GUIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBIM_GUIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CBIM_GUIDoc commands
