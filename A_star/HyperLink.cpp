// HyperLink.cpp : implementation file
//

#include "stdafx.h"
#include "A_star.h"
#include "HyperLink.h"

WNDPROC OldWndProc;
HCURSOR m_hCursor;
HWND hHyperLinkWnd = 0;
BOOL bClicked = 0;
BOOL bMoved = 0;
// CHyperLink

IMPLEMENT_DYNAMIC(CHyperLink, CStatic)

CHyperLink::CHyperLink()
{
	m_hCursor = LoadCursor(0, MAKEINTRESOURCE(IDC_HAND)); 
}

CHyperLink::~CHyperLink()
{
}


BEGIN_MESSAGE_MAP(CHyperLink, CStatic)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CHyperLink message handlers

LRESULT CALLBACK NewWndProcedure(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
/*	if (uMessage==WM_SETCURSOR)
	{
		//let's see if the point is in the right rectangle:
//		SetCursor(m_hCursor);
	}*/

	if (uMessage == WM_LBUTTONDOWN)
	{
		int xPos = GET_X_LPARAM(lParam); 
		int yPos = GET_Y_LPARAM(lParam); 

		RECT rect;
		GetWindowRect(hHyperLinkWnd, &rect);
		CWnd::FromHandle(hWnd)->ScreenToClient(&rect);
		if ( xPos > rect.left && xPos < rect.right && yPos < rect.bottom && yPos > rect.top)
			SetCursor(m_hCursor);
		bClicked = TRUE;
		InvalidateRect(hWnd, &rect, 1);

		SHELLEXECUTEINFO sei;
		ZeroMemory(&sei,sizeof(SHELLEXECUTEINFO));
		sei.cbSize = sizeof( SHELLEXECUTEINFO );
		sei.lpVerb = TEXT( "open" );
		sei.lpFile = L"mailto:fio_244@yahoo.com?subject=Subiect";	
		sei.nShow = SW_SHOWNORMAL;

		ShellExecuteEx(&sei);

		return 0;
	}

	if (uMessage == WM_LBUTTONUP)
	{
		int xPos = GET_X_LPARAM(lParam); 
		int yPos = GET_Y_LPARAM(lParam); 

		RECT rect;
		GetWindowRect(hHyperLinkWnd, &rect);
		CWnd::FromHandle(hWnd)->ScreenToClient(&rect);
		if ( xPos > rect.left && xPos < rect.right && yPos < rect.bottom && yPos > rect.top)
			SetCursor(m_hCursor);

		return 0;
	}

	BOOL rValue = CallWindowProc(OldWndProc, hWnd, uMessage, wParam, lParam);

	if (uMessage == WM_MOUSEMOVE)
	{
		int xPos = GET_X_LPARAM(lParam); 
		int yPos = GET_Y_LPARAM(lParam); 

		RECT rect;
		GetWindowRect(hHyperLinkWnd, &rect);
		CWnd::FromHandle(hWnd)->ScreenToClient(&rect);
		if ( xPos > rect.left && xPos < rect.right && yPos < rect.bottom && yPos > rect.top)
		{
			SetCursor(m_hCursor);
			if (bMoved == FALSE) InvalidateRect(hWnd, &rect, 1);
			bMoved = TRUE;
		}
		else {
			if (bMoved == TRUE) InvalidateRect(hWnd, &rect, 1);
			bMoved = FALSE;
		}

	}

	return rValue;
}

void CHyperLink::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages

	//creating font;
	CBrush brush;
	brush.CreateSysColorBrush(COLOR_3DFACE);
	dc.SelectObject(&brush);
	dc.SetBkMode(TRANSPARENT);

	//creating font
	LOGFONT logfont;
	CFont font;
	this->GetFont()->GetLogFont(&logfont);

	if (bMoved == TRUE)
	{
		logfont.lfUnderline = TRUE;
	}
	else 
		logfont.lfUnderline = FALSE;

	font.CreateFontIndirectW(&logfont);
	dc.SelectObject(&font);

	//setting color
	if (bClicked == FALSE)
		dc.SetTextColor(RGB(0,0,255));
	else 
	{
		dc.SetTextColor(RGB(128,0,128));
	}
	
	//displaying text
	CString str;
	this->GetWindowTextW(str);
	dc.TextOutW(0, 0, str);

	brush.DeleteObject();



	if (OldWndProc == 0)
	{
		hHyperLinkWnd = m_hWnd;
		OldWndProc = (WNDPROC)SetWindowLongPtrW(this->GetParent()->m_hWnd, GWL_WNDPROC, (LONG)NewWndProcedure);
	}
//	this->GetParent()->SetCursor(m_hCursor);
}

void CHyperLink::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
}

BOOL CHyperLink::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default

	this->GetParent()->SetWindowTextW(L"Cursor changed?");
	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

int CHyperLink::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD error = GetLastError();
	int x = 5;
	x++;

	return 0;
}

BOOL CHyperLink::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: Add your specialized code here and/or call the base class

	this->GetParent()->MessageBox(L"Create!");

	return CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID);
}

BOOL CHyperLink::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam)
{
	// TODO: Add your specialized code here and/or call the base class

	MessageBox(L"creating!");
	return CStatic::CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, lpParam);
}

void CHyperLink::OnShowWindow(BOOL bShow, UINT nStatus)
{
	MessageBox(L"creating!");
	CStatic::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
}

void CHyperLink::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	MessageBox(L"LButtonDown!");
//	CStatic::OnLButtonDown(nFlags, point);
}

BOOL CHyperLink::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class

	if (message == WM_LBUTTONDOWN) MessageBox(L"L down");
	return CStatic::OnWndMsg(message, wParam, lParam, pResult);
}
