// RandomSel.cpp : implementation file
//

#include "stdafx.h"
#include "A_star.h"
#include "RandomSel.h"

// CRandomSel dialog

BOOL randgen_value = TRUE;

IMPLEMENT_DYNAMIC(CRandomSel, CDialog)

CRandomSel::CRandomSel(CWnd* pParent /*=NULL*/)
	: CDialog(CRandomSel::IDD, pParent)
	, m_nrRandom(0)
	, m_nrRounds(0)
	, m_nrColumns(0)
{
	m_rgen = randgen_value;
}

CRandomSel::~CRandomSel()
{
}

void CRandomSel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_nrRandom);
	DDV_MinMaxInt(pDX, m_nrRandom, 1, 65536);
	DDX_Text(pDX, IDC_EDIT1, m_nrRounds);
	DDV_MinMaxInt(pDX, m_nrRounds, 5, 256);
	DDX_Check(pDX, IDC_RANDGEN, m_rgen);
	DDX_Text(pDX, IDC_EDIT3, m_nrColumns);
	DDV_MinMaxInt(pDX, m_nrColumns, 5, 256);
}


BEGIN_MESSAGE_MAP(CRandomSel, CDialog)
	ON_BN_CLICKED(IDOK, &CRandomSel::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RANDGEN, &CRandomSel::OnBnClickedRandgen)
END_MESSAGE_MAP()


// CRandomSel message handlers

void CRandomSel::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	BOOL check_err;
	UINT Rounds = GetDlgItemInt(IDC_EDIT1, &check_err, 0);
	if (check_err==0) { MessageBox(L"Invalid value!", L"Error!"); return;}
	UINT Random = GetDlgItemInt(IDC_EDIT2, &check_err, 0);
	if (check_err==0) { MessageBox(L"Invalid value!", L"Error!"); return;}
	if (Rounds * Rounds < Random) { MessageBox(L"The Random value must be less than the square of Rounds!", L"Error!"); return;}
	OnOK();
	randgen_value = m_rgen;
}


BOOL CRandomSel::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	((CButton*)this->GetDlgItem(IDC_RANDGEN))->SetCheck(m_rgen);
	this->GetDlgItem(IDC_EDIT2)->EnableWindow(m_rgen);
	if (m_rgen==FALSE)
		GetDlgItem(IDC_EDIT2)->SetWindowTextW(L"5");

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CRandomSel::OnBnClickedRandgen()
{
	// TODO: Add your control notification handler code here
	int check = ((CButton*)this->GetDlgItem(IDC_RANDGEN))->GetCheck();
	if (check==FALSE)
	{
		GetDlgItem(IDC_EDIT2)->SetWindowText(L"5");
		GetDlgItem(IDC_EDIT2)->EnableWindow(0);
	}
	else
	{
		GetDlgItem(IDC_EDIT2)->SetWindowText(L"0");
		GetDlgItem(IDC_EDIT2)->EnableWindow();
	}
}
