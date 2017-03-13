// BCG_GUIDoc.cpp : implementation of the CBCG_GUIDoc class
//

#include "stdafx.h"
#include "BCG_GUI.h"

#include "BCG_GUIDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBCG_GUIDoc

IMPLEMENT_DYNCREATE(CBCG_GUIDoc, CDocument)

BEGIN_MESSAGE_MAP(CBCG_GUIDoc, CDocument)
END_MESSAGE_MAP()


// CBCG_GUIDoc construction/destruction

CBCG_GUIDoc::CBCG_GUIDoc()
{
	// TODO: add one-time construction code here

}

CBCG_GUIDoc::~CBCG_GUIDoc()
{
}

BOOL CBCG_GUIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CBCG_GUIDoc serialization

void CBCG_GUIDoc::Serialize(CArchive& ar)
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


// CBCG_GUIDoc diagnostics

#ifdef _DEBUG
void CBCG_GUIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBCG_GUIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CBCG_GUIDoc commands
