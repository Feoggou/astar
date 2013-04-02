// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "A_star.h"
#include "OptionsDlg.h"
#include "A_starDoc.h"
#include "A_starView.h"


// COptionsDlg dialog

IMPLEMENT_DYNAMIC(COptionsDlg, CDialog)

COptionsDlg::COptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDlg::IDD, pParent)
	, m_dSquareSize(20)
	, m_dUserObjSize(1)
	, m_dNeutObjSize(0)
{
	m_pParent = pParent;
}

COptionsDlg::~COptionsDlg()
{
}

void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SQUARESIZE, m_dSquareSize);
	DDV_MinMaxInt(pDX, m_dSquareSize, 10, 200);
	DDX_Radio(pDX, IDC_USEROBJSIZE, m_dUserObjSize);
	DDX_Radio(pDX, IDC_NEUTOBJSIZE, m_dNeutObjSize);
}


BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
END_MESSAGE_MAP()


// COptionsDlg message handlers

BOOL COptionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	this->m_dNeutObjSize = ((CA_starView*)m_pParent)->m_dNeutObjSizeOption;
	this->m_dSquareSize = ((CA_starView*)m_pParent)->m_dSquareSize;
	this->m_dUserObjSize = ((CA_starView*)m_pParent)->m_dUserObjSizeOption;

	m_ToolTip.Create(this);
	m_ToolTip.AddTool(GetDlgItem(IDC_SQUARESIZE), L"The size of each square. This affects the availability of values for disk sizes of below.");
	m_ToolTip.AddTool(GetDlgItem(IDC_USEROBJSIZE), 
L"Click to set the size of the main disk (the blue one) to 50% size of a square");
	m_ToolTip.AddTool(GetDlgItem(IDC_RADIO2), 
L"Click to set the size of the main disk (the blue one) to 75% size of a square");
	m_ToolTip.AddTool(GetDlgItem(IDC_RADIO3), 
L"Click to set the size of the main disk (the blue one) to the size of a square");
	m_ToolTip.AddTool(GetDlgItem(IDC_NEUTOBJSIZE), 
L"Click to set the size of the computer disks (the cyan ones) to 50% size of a square");
	m_ToolTip.AddTool(GetDlgItem(IDC_RADIO5), 
L"Click to set the size of the computer disks (the cyan ones) to 75% size of a square");
	m_ToolTip.AddTool(GetDlgItem(IDC_RADIO6), 
L"Click to set the size of the computer disks (the cyan ones) to the size of a square");

	m_ToolTip.SetDelayTime(10000, TTDT_AUTOPOP);
	m_ToolTip.SetDelayTime(600, TTDT_RESHOW);
	m_ToolTip.SetDelayTime(600, TTDT_INITIAL);
	

	m_ToolTip.Activate(true);

//	this->EnableToolTips();

	this->UpdateData(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL COptionsDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	m_ToolTip.RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}
