// A_starView.h : interface of the CA_starView class
//


#pragma once
#include "atltypes.h"
#include "afxcmn.h"


class CA_starView : public CView
{
protected: // create from serialization only
	CA_starView();
	DECLARE_DYNCREATE(CA_starView)

// Attributes
public:
	CA_starDoc* GetDocument() const;

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
	virtual ~CA_starView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnEditMakenew();
public:
	afx_msg void OnPaint();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnEditPutblock();
public:
	afx_msg void OnUpdateEditPutblock(CCmdUI *pCmdUI);
public:
//	afx_msg void OnClose();
public:
	afx_msg void OnEditFindpath();
public:
	afx_msg void OnUpdateEditFindpath(CCmdUI *pCmdUI);
public:
	afx_msg void OnEditClassicala();
public:
	afx_msg void OnFileNew();
public:
	afx_msg void OnViewPointschecked();
public:
	afx_msg void OnUpdateViewPointschecked(CCmdUI *pCmdUI);
public:
//	afx_msg void OnFileOpen();
public:
	afx_msg void OnHeuristicManhattandistance();
public:
	afx_msg void OnUpdateHeuristicManhattandistance(CCmdUI *pCmdUI);
public:
	afx_msg void OnHeuristicEuclidiandistance();
public:
	afx_msg void OnUpdateHeuristicEuclidiandistance(CCmdUI *pCmdUI);
public:
	afx_msg void OnEditPlacestartpoint();
public:
	afx_msg void OnEditPlaceendpoint();
public:
	afx_msg void OnUpdateEditPlaceendpoint(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateEditPlacestartpoint(CCmdUI *pCmdUI);
public:
	afx_msg void OnEditClearpath();
public:
	afx_msg void OnEditClearmap();
	int m_dAlgorithm;

	int m_dPlace;

	CBitmap bmpBlock, bmpDest, bmpSource;
public:
	afx_msg void OnUpdateEditClearpath(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateEditClearmap(CCmdUI *pCmdUI);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg void OnHeuristicEuclidiandistancev();
public:
	afx_msg void OnUpdateHeuristicEuclidiandistancev(CCmdUI *pCmdUI);
public:
	afx_msg void OnHeuristicDiagonalshortcut();
public:
	afx_msg void OnUpdateHeuristicDiagonalshortcut(CCmdUI *pCmdUI);
	afx_msg void OnHeuristicNoheuristic();
	afx_msg void OnUpdateHeuristicNoheuristic(CCmdUI *pCmdUI);
	afx_msg void OnViewOptions();
public:
	int m_dSquareSize;
	int m_dUserObjSizeOption;
	int m_dNeutObjSizeOption;
	int m_dUserObjSize;
	int m_dNeutObjSize;

	afx_msg void OnUpdateEditScrollmode(CCmdUI *pCmdUI);
	afx_msg void OnEditScrollmode();
private:
	CPoint m_LastPoint;
	CPoint m_ScrollPosition;
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
private:
	CPoint m_MaxScrollPosition;
public:
	afx_msg void OnAlgorithmVectors();
	afx_msg void OnUpdateEditClassicala(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAlgorithmVectors(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in A_starView.cpp
inline CA_starDoc* CA_starView::GetDocument() const
   { return reinterpret_cast<CA_starDoc*>(m_pDocument); }
#endif

