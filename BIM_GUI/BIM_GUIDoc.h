// BIM_GUIDoc.h : interface of the CBIM_GUIDoc class
//


#pragma once


class CBIM_GUIDoc : public CDocument
{
protected: // create from serialization only
	CBIM_GUIDoc();
	DECLARE_DYNCREATE(CBIM_GUIDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CBIM_GUIDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


