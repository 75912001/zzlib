/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	ok
	brief:		tcp客户端
*********************************************************************/
#pragma once

#include "lib_util.h"
#include "lib_net_util.h"
#include "lib_tcp.h"
#include "lib_memory.h"
#include "lib_file.h"
#include "lib_lock.h"

namespace el{
	class lib_tcp_peer_info_t : public lib_tcp_t
	{
	public:
		el::E_FD_TYPE fd_type;
		lib_active_buf_t recv_buf;
		lib_active_buf_t send_buf;
		lib_lock_mutex_t lock_mutex;
		void* data;//用户数据
	public:
		lib_tcp_peer_info_t();
		virtual ~lib_tcp_peer_info_t();
	public:
		void open(int fd, el::E_FD_TYPE fd_type, const char* ip, uint16_t port);
		void close();
		int shutdown_rd();
		void init();
	protected:
	private:
		lib_tcp_peer_info_t(const lib_tcp_peer_info_t& cr);
		lib_tcp_peer_info_t& operator=(const lib_tcp_peer_info_t& cr);
	};

}//end namespace el

