#include "pch.h"
#include "mfc_service_client.h"
#include <mokdong/service.h>


mfc_service_client::~mfc_service_client()
{
	for (auto entry : builders_)
	{
		if (entry.second != nullptr)
		{
			delete entry.second;
		}
	}
}

mfc_service_client::mfc_service_client(boost::asio::io_service& io_service)
	: service_client(io_service)
{
	builders_[to_underlying(common_service_code::login_result)] =
		new service_data_builder_maker<to_underlying(common_service_code::login_result),
		sd_simple_result<to_underlying(common_service_code::login_result)>
		>();

	builders_[to_underlying(chat_service_code::create_room_result)] =
		new service_data_builder_maker<to_underlying(chat_service_code::create_room_result),
		sd_simple_result<to_underlying(chat_service_code::create_room_result)>
		>();

	builders_[to_underlying(chat_service_code::room_say_to)] =
		new service_data_builder_maker<to_underlying(chat_service_code::room_say_to),
		sd_room_say_to>();

	builders_[to_underlying(chat_service_code::room_list)] =
		new service_data_builder_maker<to_underlying(chat_service_code::room_list),
		sd_room_list>();

	builders_[to_underlying(chat_service_code::join_room_result)] =
		new service_data_builder_maker<to_underlying(chat_service_code::join_room_result),
		sd_simple_result<to_underlying(chat_service_code::join_room_result)>
		>();

	builders_[to_underlying(chat_service_code::room_user_list)] =
		new service_data_builder_maker<to_underlying(chat_service_code::room_user_list),
		sd_room_user_list>();

	builders_[to_underlying(chat_service_code::room_user_join)] =
		new service_data_builder_maker<to_underlying(chat_service_code::room_user_join),
		sd_room_user_join>();

	builders_[to_underlying(chat_service_code::room_user_leave)] =
		new service_data_builder_maker<to_underlying(chat_service_code::room_user_leave),
		sd_room_user_leave>();
}

void mfc_service_client::do_service(tcp_buffer& buffer)
{
	auto service_code = buffer.packet_service_code();
	auto builder = builders_[service_code];
	
	if (builder != nullptr)
	{
		service_data* sd = builder->create();
		sd->demarshal(buffer);
		sd_que_.push(sd);
	}
}
