// CmokdongclientNameDlg.cpp: 구현 파일
//

#include "pch.h"
#include "mokdong_client_mfc.h"
#include "mokdong_client_NameDlg.h"
#include "afxdialogex.h"


// CmokdongclientNameDlg 대화 상자

IMPLEMENT_DYNAMIC(CmokdongclientNameDlg, CDialogEx)

CmokdongclientNameDlg::CmokdongclientNameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NAME, pParent)
{

}

CmokdongclientNameDlg::~CmokdongclientNameDlg()
{
}

void CmokdongclientNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NAME, txName_);
}


BEGIN_MESSAGE_MAP(CmokdongclientNameDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CmokdongclientNameDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CmokdongclientNameDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CmokdongclientNameDlg 메시지 처리기


void CmokdongclientNameDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	txName_.GetWindowText(name_);
	name_.Trim();

	CDialogEx::OnOK();
}


void CmokdongclientNameDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


CString CmokdongclientNameDlg::GetName()
{
	return name_;
}






