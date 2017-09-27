/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	ok
	brief:		UDP
*********************************************************************/

#pragma once

#include "lib_include.h"
#include "lib_net.h"
#include "lib_memory.h"

namespace el{
	class lib_udp_t : public lib_net_t
	{
	protected:
		sockaddr_in addr;
	public:
		lib_udp_t(){
			::memset(&(this->addr), 0, sizeof(this->addr));
		}
		virtual ~lib_udp_t(){}
		virtual int send(const void* data, int len);
		virtual int recv(void* data, int len);
		int connect(const std::string& ip, uint16_t port);
	protected:
		
	private:
		lib_udp_t(const lib_udp_t& cr);
		lib_udp_t& operator=(const lib_udp_t& cr);
	};

}//end namespace el
