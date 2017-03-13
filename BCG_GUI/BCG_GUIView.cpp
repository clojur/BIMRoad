// BCG_GUIView.cpp : implementation of the CBCG_GUIView class
//

#include "stdafx.h"
#include "BCG_GUI.h"

#include "BCG_GUIDoc.h"
#include "BCG_GUIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBCG_GUIView

IMPLEMENT_DYNCREATE(CBCG_GUIView, CView)

BEGIN_MESSAGE_MAP(CBCG_GUIView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_REGISTERED_MESSAGE(BCGM_CHANGEVISUALMANAGER, OnChangeVisualManager)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CBCG_GUIView construction/destruction

CBCG_GUIView::CBCG_GUIView()
{
	// TODO: add construction code here

}

CBCG_GUIView::~CBCG_GUIView()
{
}

BOOL CBCG_GUIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CBCG_GUIView drawing

void CBCG_GUIView::OnDraw(CDC* /*pDC*/)
{
	CBCG_GUIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


// CBCG_GUIView printing

void CBCG_GUIView::OnFilePrintPreview()
{
	BCGPPrintPreview (this);
}

BOOL CBCG_GUIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBCG_GUIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBCG_GUIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CBCG_GUIView diagnostics

#ifdef _DEBUG
void CBCG_GUIView::AssertValid() const
{
	CView::AssertValid();
}

void CBCG_GUIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBCG_GUIDoc* CBCG_GUIView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBCG_GUIDoc)));
	return (CBCG_GUIDoc*)m_pDocument;
}
#endif //_DEBUG


// CBCG_GUIView message handlers

LRESULT CBCG_GUIView::OnPrintClient(WPARAM wp, LPARAM lp)
{
	if ((lp & PRF_CLIENT) == PRF_CLIENT)
	{
		CDC* pDC = CDC::FromHandle((HDC) wp);
		ASSERT_VALID(pDC);
		
		OnDraw(pDC);
	}
	
	return 0;
}

LRESULT CBCG_GUIView::OnChangeVisualManager(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return 0;
}


int CBCG_GUIView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	theApp.GetViewManager()->AddView(m_hWnd);
	//theApp.GetViewManager()->SetSceneData(m_hWnd);
	theApp.GetViewManager()->Display(m_hWnd);
	return 0;
}
