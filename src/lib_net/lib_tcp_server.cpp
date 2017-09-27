#include <lib_include.h>
#include "lib_file.h"
#include "lib_net/lib_net_util.h"
#include "lib_net/lib_tcp_server.h"

int el::lib_tcp_srv_t::accept( struct sockaddr_in& peer, uint32_t sendbuf, uint32_t recvbuf, bool block  )
{
	socklen_t peer_size = sizeof(peer);

	int newfd = 0;
	do {
		newfd = ::accept(this->listen_fd, (struct sockaddr*)&peer, &peer_size);
	} while(unlikely(newfd < 0 
		&& (EINTR == errno || EWOULDBLOCK == errno || ECONNABORTED == errno || EPROTO == errno)));
	
	if (unlikely(newfd < 0)){
		return ERR;
	}

	//设置发送与接收缓存大小
	if (0 != sendbuf){
		lib_net_util_t::set_sendbuf(newfd, sendbuf);
	}
	if (0 != recvbuf){
		lib_net_util_t::set_recvbuf(newfd, recvbuf);
	}

	lib_file_t::set_io_block(newfd, block);

	return newfd;
}

int el::lib_tcp_srv_t::bind( const char* ip, uint16_t port )
{
	sockaddr_in sa_in;
	::memset(&sa_in, 0, sizeof(sa_in));

	sa_in.sin_family = PF_INET;
	sa_in.sin_port = htons(port);
	if (NULL != ip){
		::inet_pton(PF_INET, ip, &sa_in.sin_addr);
	}else{
		sa_in.sin_addr.s_addr = htonl(INADDR_ANY);
	}

	return ::bind(this->listen_fd, (sockaddr*)&sa_in,sizeof(sa_in));
}

el::lib_tcp_srv_t::lib_tcp_srv_t()
{
	this->cli_time_out_sec = 0;
	this->peer_fd_infos = NULL;
	this->listen_fd = INVALID_FD;
	this->cli_fd_value_max = 0;
}

el::lib_tcp_srv_t::~lib_tcp_srv_t()
{
	if (NULL != this->peer_fd_infos){
		for (int i = 0; i < this->cli_fd_value_max; i++) {
			lib_tcp_peer_info_t& cfi = this->peer_fd_infos[i];
			if (FD_TYPE_UNUSED == cfi.fd_type){
				continue;
			}
			cfi.close();
		}
		SAFE_DELETE_ARR(this->peer_fd_infos);
	}
	lib_file_t::close_fd(this->listen_fd);
}

el::on_functions_tcp_srv::on_functions_tcp_srv()
{
	this->on_events = 0;
	this->on_cli_pkg = 0;
	this->on_srv_pkg = 0;
	this->on_cli_conn_closed = 0;
	this->on_svr_conn_closed = 0;
	this->on_init = 0;
	this->on_fini = 0;
	this->on_get_pkg_len = 0;
	this->on_mcast_pkg = 0;
	this->on_addr_mcast_pkg = 0;
	this->on_svr_conn = 0;
	this->on_cli_conn = 0;
}
