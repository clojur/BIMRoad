// BIM_GUIView.cpp : implementation of the CBIM_GUIView class
//

#include "stdafx.h"
#include "BIM_GUI.h"

#include "BIM_GUIDoc.h"
#include "BIM_GUIView.h"

#include "../include/RouteCross/RouteCross.h"
#include "../include/OSGBIM/OSGBIM.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBIM_GUIView

IMPLEMENT_DYNCREATE(CBIM_GUIView, CView)

BEGIN_MESSAGE_MAP(CBIM_GUIView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_REGISTERED_MESSAGE(BCGM_CHANGEVISUALMANAGER, OnChangeVisualManager)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CBIM_GUIView construction/destruction

CBIM_GUIView::CBIM_GUIView()
{
	// TODO: add construction code here

}

CBIM_GUIView::~CBIM_GUIView()
{
}

BOOL CBIM_GUIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CBIM_GUIView drawing

void CBIM_GUIView::OnDraw(CDC* /*pDC*/)
{
	CBIM_GUIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


// CBIM_GUIView printing

void CBIM_GUIView::OnFilePrintPreview()
{
	BCGPPrintPreview (this);
}

BOOL CBIM_GUIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBIM_GUIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBIM_GUIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CBIM_GUIView diagnostics

#ifdef _DEBUG
void CBIM_GUIView::AssertValid() const
{
	CView::AssertValid();
}

void CBIM_GUIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBIM_GUIDoc* CBIM_GUIView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBIM_GUIDoc)));
	return (CBIM_GUIDoc*)m_pDocument;
}
#endif //_DEBUG


// CBIM_GUIView message handlers

LRESULT CBIM_GUIView::OnPrintClient(WPARAM wp, LPARAM lp)
{
	if ((lp & PRF_CLIENT) == PRF_CLIENT)
	{
		CDC* pDC = CDC::FromHandle((HDC) wp);
		ASSERT_VALID(pDC);
		
		OnDraw(pDC);
	}
	
	return 0;
}

LRESULT CBIM_GUIView::OnChangeVisualManager(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return 0;
}


int CBIM_GUIView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	CRoadSurfaceLine *RSL=new CRoadSurfaceLine;
	theApp.GetViewManager()->AddView(m_hWnd);
	theApp.GetViewManager()->RemoveAllNode(m_hWnd);
	//theApp.GetViewManager()->RemoveAllHandle(m_hWnd);
	theApp.GetViewManager()->SetSceneData(m_hWnd, RSL->GetUpdateCallBack());
	//theApp.GetViewManager()->getView(m_hWnd)->setCameraManipulator(new Contrl2DManipulator(RSL->GetUpdateCallBack()->GetModel()));
	theApp.GetViewManager()->Display(m_hWnd);

	return 0;
}
