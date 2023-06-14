
// mokdong_client_mfc.h: PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// 주 기호입니다.
#include "mfc_service_client.h"

// CmokdongclientmfcApp:
// 이 클래스의 구현에 대해서는 mokdong_client_mfc.cpp을(를) 참조하세요.
//

class CmokdongclientmfcApp : public CWinApp
{
public:
	CmokdongclientmfcApp();

private:
	boost::asio::io_service mfc_io_service_;
	mfc_service_client service_client_;

// 재정의입니다.
public:
	virtual BOOL InitInstance();
	
	mfc_service_client& service_client() {return service_client_;}

// 구현입니다.

	DECLARE_MESSAGE_MAP()
};

extern CmokdongclientmfcApp theApp;
