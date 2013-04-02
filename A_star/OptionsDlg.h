#pragma once
#include "afxcmn.h"


// COptionsDlg dialog

class COptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionsDlg)

public:
	COptionsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COptionsDlg();

// Dialog Data
	enum { IDD = IDD_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_dSquareSize;
private:
	CWnd* m_pParent;
public:
	virtual BOOL OnInitDialog();
	int m_dUserObjSize;
	int m_dNeutObjSize;
private:
	CToolTipCtrl m_ToolTip;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
