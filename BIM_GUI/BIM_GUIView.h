// BIM_GUIView.h : interface of the CBIM_GUIView class
//


#pragma once

class CBIM_GUIView : public CView
{
protected: // create from serialization only
	CBIM_GUIView();
	DECLARE_DYNCREATE(CBIM_GUIView)

// Attributes
public:
	CBIM_GUIDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CBIM_GUIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg LRESULT OnPrintClient(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnChangeVisualManager(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#ifndef _DEBUG  // debug version in BIM_GUIView.cpp
inline CBIM_GUIDoc* CBIM_GUIView::GetDocument() const
   { return reinterpret_cast<CBIM_GUIDoc*>(m_pDocument); }
#endif

