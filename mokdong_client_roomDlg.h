#pragma once


// Cmokdong_client_roomDlg 대화 상자

class CmokdongclientroomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CmokdongclientroomDlg)

public:
	CmokdongclientroomDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CmokdongclientroomDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOKDONG_CLIENT_ROOM_LIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox lstRoom_;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedJoin();
	afx_msg void OnBnClickedCreate();

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnCancel();
	afx_msg void OnBnClickedRefresh();
	void RefreshList();

	void OnServiceCreateRoomResult(service_data* sd);
	void OnServiceRoomList(service_data* sd);
	void OnServiceJoinRoom(service_data* sd);
	afx_msg void OnLbnDblclkList();
	void JoinRoom();
};
