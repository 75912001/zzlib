/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	ok
	brief:		
*********************************************************************/

#pragma once

#include "lib_include.h"
#include "lib_util.h"
#include "lib_net.h"

namespace el{
	class lib_tcp_t : public lib_net_t
	{
	public:
		uint16_t port;
		uint32_t ip;
		lib_tcp_t(){
			this->port = 0;
			this->ip = 0;
		}
		virtual ~lib_tcp_t(){}

		virtual int send(const void* data, int len);

		virtual int recv(void* data, int len);

		std::string get_ip_str();

		/**
		* @brief Create a TCP connection
		*
		* @param const std::string& ip,  the ip address to connect to.
		* @param uint16_t port,  the port to connect to.
		* @param int timeout,  abort the connecting attempt after timeout secs. If timeout is less than or equal to 0, 
		*                                then the connecting attempt will not be interrupted until error occurs.
		* @param int block,  true and the connected fd will be set blocking, false and the fd will be set nonblocking.
		* @param uint32_t send_buf_len, 发送缓冲大小
		* @param uint32_t rev_buf_len, 接收缓冲大小
		* @return int, the connected fd on success, -1 on error.
		*/
		static int connect( const std::string& ip, uint16_t port, int timeout,
			bool block, uint32_t send_buf_len = 0, uint32_t rev_buf_len = 0 );
		//************************************
		// Brief:	  Set the given fd SO_REUSEADDR
		// Returns:   int (0 on success, -1 on error)
		// Parameter: int fd (fd)
		//************************************
		static int set_reuse_addr(int sfd);

	protected:
		
	private:
		lib_tcp_t(const lib_tcp_t& cr);
		lib_tcp_t& operator=(const lib_tcp_t& cr);
	};

}//end namespace el
