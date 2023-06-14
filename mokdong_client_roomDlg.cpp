// Cmokdong_client_roomDlg.cpp: 구현 파일
//

#include "pch.h"
#include "mokdong_client_mfc.h"
#include "mokdong_client_roomDlg.h"
#include "mokdong_client_NameDlg.h"
#include "afxdialogex.h"
#include <mokdong/service_data.h>
#include "mokdong_client_chatDlg.h"
#include <mokdong/room.h>


// Cmokdong_client_roomDlg 대화 상자

IMPLEMENT_DYNAMIC(CmokdongclientroomDlg, CDialogEx)

CmokdongclientroomDlg::CmokdongclientroomDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MOKDONG_CLIENT_ROOM_LIST, pParent)
{

}

CmokdongclientroomDlg::~CmokdongclientroomDlg()
{
}

void CmokdongclientroomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, lstRoom_);
}


BEGIN_MESSAGE_MAP(CmokdongclientroomDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CmokdongclientroomDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDJOIN, &CmokdongclientroomDlg::OnBnClickedJoin)
	ON_BN_CLICKED(IDCREATE, &CmokdongclientroomDlg::OnBnClickedCreate)
//	ON_WM_ACTIVATE()
//	ON_WM_CLOSE()
ON_WM_SHOWWINDOW()
ON_WM_TIMER()
ON_BN_CLICKED(IDREFRESH, &CmokdongclientroomDlg::OnBnClickedRefresh)
ON_LBN_DBLCLK(IDC_LIST, &CmokdongclientroomDlg::OnLbnDblclkList)
END_MESSAGE_MAP()


// Cmokdong_client_roomDlg 메시지 처리기


void CmokdongclientroomDlg::OnBnClickedOk()
{
	KillTimer(2);

	CDialogEx::OnOK();
}


void CmokdongclientroomDlg::OnBnClickedJoin()
{
	JoinRoom();
}


void CmokdongclientroomDlg::OnBnClickedCreate()
{
	CmokdongclientNameDlg dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		CString name = dlg.GetName();
		if (name == "") return;

		CT2CA ct2ca(name);
		string _name(ct2ca);

		sd_create_room sd;
		sd.set_name(_name);
		theApp.service_client().write_async(&sd);
	}
}

void CmokdongclientroomDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	RefreshList();
	SetTimer(2, 100, NULL);
}

void CmokdongclientroomDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 2)
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
			if (service_code == to_underlying(chat_service_code::create_room_result))
			{
				OnServiceCreateRoomResult(sd);
				delete sd;
			}
			else if (service_code == to_underlying(chat_service_code::room_list))
			{
				OnServiceRoomList(sd);
				delete sd;
			}
			else if (service_code == to_underlying(chat_service_code::join_room_result))
			{
				OnServiceJoinRoom(sd);
				delete sd;
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CmokdongclientroomDlg::OnServiceCreateRoomResult(service_data* sd)
{
	sd_simple_result<to_underlying(chat_service_code::create_room_result)>*
		sdr = (sd_simple_result<to_underlying(chat_service_code::create_room_result)>*) sd;

	if (sdr->result() != (int)room_manager_error_code::success)
	{
		CString msg;
		msg.Format(_T("%d"), sdr->result());
		AfxMessageBox(msg);
		return;
	}

	KillTimer(2);

	CmokdongclientchatDlg  dlg;
	dlg.DoModal();

	RefreshList();
	SetTimer(2, 100, NULL);
}

void CmokdongclientroomDlg::OnServiceRoomList(service_data* sd)
{
	UpdateData(TRUE);

	sd_room_list* sdr = (sd_room_list*)sd;

	lstRoom_.ResetContent();
	for (auto& name : sdr->names())
	{
		CStringA str = name.c_str();
		CString item(str);
		lstRoom_.AddString(item);
	}
}

void CmokdongclientroomDlg::OnServiceJoinRoom(service_data* sd)
{
	sd_simple_result<to_underlying(chat_service_code::join_room_result)>*
		sdr = (sd_simple_result<to_underlying(chat_service_code::join_room_result)>*) sd;

	if (sdr->result() != (int)room_manager_error_code::success)
	{
		CString msg;
		msg.Format(_T("%d"), sdr->result());
		AfxMessageBox(msg);
		return;
	}

	KillTimer(2);

	CmokdongclientchatDlg  dlg;
	dlg.DoModal();

	RefreshList();
	SetTimer(2, 100, NULL);
}


void CmokdongclientroomDlg::OnCancel()
{
	KillTimer(2);

	CDialog::OnCancel();
}


void CmokdongclientroomDlg::OnBnClickedRefresh()
{
	RefreshList();
}


void CmokdongclientroomDlg::RefreshList()
{
	sd_simple<to_underlying(chat_service_code::get_room_list)> sd;
	theApp.service_client().write_async(&sd);
}


void CmokdongclientroomDlg::OnLbnDblclkList()
{
	JoinRoom();
}


void CmokdongclientroomDlg::JoinRoom()
{
	int nSel = lstRoom_.GetCurSel();
	if (nSel != LB_ERR)
	{
		CString itemSelected;
		lstRoom_.GetText(nSel, itemSelected);

		CT2CA item(itemSelected);
		string name = item;

		sd_join_room sd;
		sd.set_name(name);
		theApp.service_client().write_async(&sd);
	}
}










