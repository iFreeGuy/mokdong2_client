#pragma once


// CmokdongclientNameDlg 대화 상자

class CmokdongclientNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CmokdongclientNameDlg)

public:
	CmokdongclientNameDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CmokdongclientNameDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	CString GetName();
	CEdit txName_;
	CString name_;
};
