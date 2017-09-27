
/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	OK
*********************************************************************/

#pragma once

#include "lib_udp.h"

namespace el{
	class lib_mcast_t : public lib_udp_t{
	protected:
		std::string mcast_incoming_if;//recv
		std::string mcast_outgoing_if;//send
		std::string mcast_ip;//239.X.X.X
		uint16_t mcast_port;//239.X.X.X
	public:
		lib_mcast_t();
		virtual ~lib_mcast_t(){}
		//return 0:succ
		int create(const std::string& mcast_ip, uint16_t mcast_port,
			const std::string& mcast_incoming_if, const std::string& mcast_outgoing_if);
	protected:
	private:
		lib_mcast_t(const lib_mcast_t& cr);
		lib_mcast_t& operator=(const lib_mcast_t& cr);
		int mcast_join();
	private:
	};
}//end namespace el
