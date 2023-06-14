#pragma once


// Cmokdong_client_chatDlg 대화 상자

class CmokdongclientchatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CmokdongclientchatDlg)

public:
	CmokdongclientchatDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CmokdongclientchatDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOKDONG_CLIENT_CHAT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit txChat_;
	CEdit txMsg_;
	CListBox lstUsers_;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedRefresh();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnCancel();
	void OnExit();
	void RefreshList();
	void OnServiceRoomSayTo(service_data* sd);
	void OnServiceRoomUserList(service_data* sd);
	void OnServiceRoomUserJoin(service_data* sd);
	void OnServiceRoomUserLeave(service_data* sd);
};
