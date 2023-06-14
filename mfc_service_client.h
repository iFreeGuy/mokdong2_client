#pragma once

#include <mokdong/service_client.h>
#include <mokdong/service_core.h>
#include <queue>


using namespace std;
using namespace boost::asio;
using namespace mokdong::network;

class mfc_service_client : public service_client
{
private:
	map<uint16_t, service_data_builder*> builders_;
	queue<service_data*> sd_que_;

public:
	mfc_service_client() = delete;
	mfc_service_client(boost::asio::io_service& io_service);

	virtual ~mfc_service_client();

	virtual void do_service(tcp_buffer& buffer);
	queue<service_data*>& sd_que() { return sd_que_; }
};

