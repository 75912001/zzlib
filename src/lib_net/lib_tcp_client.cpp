#include <lib_include.h>
#include "lib_log.h"
#include "lib_net/lib_tcp_client.h"

el::lib_tcp_peer_info_t::lib_tcp_peer_info_t()
{
	this->init();
}

void el::lib_tcp_peer_info_t::close()
{
	if (SUCC == lib_file_t::close_fd(this->fd)){
	}
	this->init();
}

void el::lib_tcp_peer_info_t::init()
{
	this->fd = INVALID_FD;
	this->fd_type = FD_TYPE_UNUSED;
	this->port = 0;
	this->ip = 0;
	this->recv_buf.clean();
	this->send_buf.clean();
	this->data = NULL;
}

el::lib_tcp_peer_info_t::~lib_tcp_peer_info_t()
{
	this->close();
}

int el::lib_tcp_peer_info_t::shutdown_rd()
{
	return lib_file_t::shutdown_rd(this->fd);
}

void el::lib_tcp_peer_info_t::open( int fd, el::E_FD_TYPE fd_type, const char* ip, uint16_t port )
{
	this->init();
	this->fd = fd;
	this->fd_type = fd_type;
	if (NULL != ip) {
		this->ip = el::lib_net_util_t::ip2int(ip);
		this->port = port;
	}
}
