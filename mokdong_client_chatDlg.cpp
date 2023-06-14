// Cmokdong_client_chatDlg.cpp: 구현 파일
//

#include "pch.h"
#include "mokdong_client_mfc.h"
#include "mokdong_client_chatDlg.h"
#include "afxdialogex.h"


// Cmokdong_client_chatDlg 대화 상자

IMPLEMENT_DYNAMIC(CmokdongclientchatDlg, CDialogEx)

CmokdongclientchatDlg::CmokdongclientchatDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MOKDONG_CLIENT_CHAT, pParent)
{

}

CmokdongclientchatDlg::~CmokdongclientchatDlg()
{
}

void CmokdongclientchatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHAT, txChat_);
	DDX_Control(pDX, IDC_MSG, txMsg_);
	DDX_Control(pDX, IDC_LIST_USERS, lstUsers_);
}


BEGIN_MESSAGE_MAP(CmokdongclientchatDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CmokdongclientchatDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCLOSE, &CmokdongclientchatDlg::OnBnClickedClose)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDREFRESH, &CmokdongclientchatDlg::OnBnClickedRefresh)
END_MESSAGE_MAP()


// Cmokdong_client_chatDlg 메시지 처리기


void CmokdongclientchatDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	CString msg;
	txMsg_.GetWindowText(msg);

	CT2CA ct2ca(msg);
	string _msg(ct2ca);

	sd_room_say sd;
	sd.set_say(_msg);
	theApp.service_client().write_async(&sd);

	msg = _T("");
	txMsg_.SetWindowText(msg);
}


void CmokdongclientchatDlg::OnBnClickedClose()
{
	OnExit();
	CDialogEx::OnOK();
}


void CmokdongclientchatDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	
	RefreshList();
	SetTimer(3, 100, NULL);
}


void CmokdongclientchatDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 3)
	{
		auto& sc = theApp.service_client();

		if (!sc.socket().is_open())
		{
			OnCancel();
			return;
		}

		sc.poll();

		if (!sc.sd_que().empty())
		{
			auto sd = sc.sd_que().front();
			sc.sd_que().pop();
			auto service_code = sd->service_code();
			if (service_code == to_underlying(chat_service_code::room_say_to))
			{
				OnServiceRoomSayTo(sd);
				delete sd;
			}
			else if (service_code == to_underlying(chat_service_code::room_user_list))
			{
				OnServiceRoomUserList(sd);
				delete sd;
			}
			else if (service_code == to_underlying(chat_service_code::room_user_join))
			{
				OnServiceRoomUserJoin(sd);
				delete sd;
			}
			else if (service_code == to_underlying(chat_service_code::room_user_leave))
			{
				OnServiceRoomUserLeave(sd);
				delete sd;
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CmokdongclientchatDlg::OnServiceRoomUserJoin(service_data* sd)
{
	sd_room_user_join* sdr = (sd_room_user_join*)sd;

	CStringA say_a;
	say_a.Format("%s이 방에 참여하였습니다.\r\n", sdr->name().c_str());

	CString say(say_a);

	CString str;
	txChat_.GetWindowText(str);
	str += say;
	txChat_.SetWindowText(str);

	RefreshList();
}

void CmokdongclientchatDlg::OnServiceRoomUserLeave(service_data* sd)
{
	sd_room_user_leave* sdr = (sd_room_user_leave*)sd;

	CStringA say_a;
	say_a.Format("%s이 방에서 떠났습니다.\r\n", sdr->name().c_str());

	CString say(say_a);

	CString str;
	txChat_.GetWindowText(str);
	str += say;
	txChat_.SetWindowText(str);

	RefreshList();
}


void CmokdongclientchatDlg::OnServiceRoomSayTo(service_data* sd)
{
	sd_room_say_to* sdr = (sd_room_say_to*)sd;

	CStringA say_a;
	say_a.Format("%s : %s\r\n", sdr->from().c_str(), sdr->say().c_str());

	CString say(say_a);

	CString str;
	txChat_.GetWindowText(str);
	str += say;
	txChat_.SetWindowText(str);
}


void CmokdongclientchatDlg::OnServiceRoomUserList(service_data* sd)
{
	sd_room_user_list* sdr = (sd_room_user_list*)sd;

	lstUsers_.ResetContent();
	for (auto& name : sdr->names())
	{
		CStringA str = name.c_str();
		CString item(str);
		lstUsers_.AddString(item);
	}
}


void CmokdongclientchatDlg::OnCancel()
{
	OnExit();
	CDialog::OnCancel();
}


void CmokdongclientchatDlg::OnExit()
{
	KillTimer(3);

	sd_simple<to_underlying(chat_service_code::leave_room)> sd;
	theApp.service_client().write_async(&sd);
}


void CmokdongclientchatDlg::RefreshList()
{
	sd_simple<to_underlying(chat_service_code::get_room_user_list)> sd;
	theApp.service_client().write_async(&sd);
}


void CmokdongclientchatDlg::OnBnClickedRefresh()
{
	RefreshList();
}




