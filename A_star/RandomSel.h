#pragma once


// CRandomSel dialog

class CRandomSel : public CDialog
{
	DECLARE_DYNAMIC(CRandomSel)

public:
	CRandomSel(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRandomSel();

// Dialog Data
	enum { IDD = IDD_RANDOMSEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	int m_nrRandom;
public:
	int m_nrRounds;
public:
	virtual BOOL OnInitDialog();
public:
	BOOL m_rgen;
public:
	afx_msg void OnBnClickedRandgen();
public:
	int m_nrColumns;
};
