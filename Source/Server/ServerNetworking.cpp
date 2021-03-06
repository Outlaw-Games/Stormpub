#include "stdafx.h"
#include "ServerNetworking.h"
#include <Common\CustomPacket.h>

bool ServerComponent::initialize()
{
	auto init_client_func = [&](server_client& client, const char* ip)
	{
		client._uid = next_uid;
		next_uid++;
	};

	server.start_listening(enetpp::server_listen_params<server_client>()
		.set_max_client_count(MAX_CLIENT_COUNT)
		.set_channel_count(2)
		.set_listen_port(8888)
		.set_initialize_client_function(init_client_func));

	on_connected = [&](server_client& client)
	{
		trace("Wrong connected func called");
	};

	 on_disconnected = [&](unsigned int client_uid)
	{
		trace("Wrong disconnected func called" + std::to_string(client_uid));
	};

	auto on_client_data_received = 
		[&](server_client& client, const enet_uint8* data, size_t data_size)
	{
		trace(
			"received packet from client :" +
			std::to_string(client.get_id()));
	};

	return true;
}

bool ServerComponent::deinitialize()
{
	server.stop_listening();
	return true;
}

void ServerComponent::consumeEvents()
{
	server.consume_events(
		on_connected,
		on_disconnected,
		on_data_received);
}

enetpp::server<server_client>* ServerComponent::getServer()
{
	return &server;
}
