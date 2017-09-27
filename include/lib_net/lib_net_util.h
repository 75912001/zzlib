/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	OK
*********************************************************************/

#pragma once

#include "lib_include.h"

namespace el{
	enum E_FD_TYPE{
		FD_TYPE_UNUSED = 0,
		FD_TYPE_LISTEN = 1,
		FD_TYPE_PIPE = 2,
		FD_TYPE_CLI = 3,
		FD_TYPE_MCAST = 4,
		FD_TYPE_ADDR_MCAST = 5,
		FD_TYPE_UDP = 6,
		FD_TYPE_ASYN_CONNECT = 7,
		FD_TYPE_SVR = 8,
	};

	class lib_net_util_t{
	public:
		/**
		* @brief ����ͨ�������ӿڣ�eth0/eth1/lo...����ȡ��Ӧ��IP��ַ��֧��IPv4��IPv6��
		* @param nif �����ӿڡ�eth0/eth1/lo...
		* @param af �����ַ���͡�AF_INET����AF_INET6��
		* @param ipaddr ���ڷ���nif��af��Ӧ��IP��ַ��
		* @return succ: 0, ����ipaddr�б�����nif��af��Ӧ��IP��ַ��fail: -1
		*/
		static int get_ip_addr(const char* nif, int af, std::string& ipaddr);

		static bool get_local_ip( std::string& ip);

		static bool get_local_ip( long& ip);

		/**
		* @brief	translate the given address family to its corresponding level
		*/
		static int family_to_level(int family);
		/**
		* @brief Set a timeout on sending data. If you want to disable timeout, just simply
		*           call this function again with millisec set to be 0.
		*
		* @param sockfd socket descriptor to be set.
		* @param millisec timeout in milliseconds.
		*
		* @return 0 on success, -1 on error and errno is set appropriately.
		*/
		static inline int set_sock_send_timeo(int sockfd, int millisec){
			return lib_net_util_t::set_sock_timeo(sockfd, SO_SNDTIMEO, millisec);
		}

		/**
		* @brief Set a timeout on receiving data. If you want to disable timeout, just simply
		*           call this function again with millisec set to be 0.
		*
		* @param sockfd socket descriptor to be set.
		* @param millisec timeout in milliseconds.
		*
		* @return 0 on success, -1 on error and errno is set appropriately.
		*/
		static inline int set_sock_rcv_timeo(int sockfd, int millisec){
			return lib_net_util_t::set_sock_timeo(sockfd, SO_RCVTIMEO, millisec);
		}
		//************************************
		// Brief:	  Set the given fd recv buffer
		// Returns:   int (0 on success, -1 on error)
		// Parameter: int s (fd)
		// Parameter: uint32_t len (recv buffer len)
		//************************************
		static inline int set_recvbuf(int s, uint32_t len){
			return ::setsockopt(s, SOL_SOCKET, SO_RCVBUF, (char*)&len, sizeof(len));
		}
		static inline int set_sendbuf(int s, uint32_t len){
			return ::setsockopt(s, SOL_SOCKET, SO_SNDBUF, (char*)&len, sizeof(len));
		}
		static inline uint32_t ip2int(const char* ip){
			return inet_addr(ip);
		}
		static inline std::string ip2str(uint32_t ip){
			struct in_addr a;
			a.s_addr = ip;
			std::string s = inet_ntoa(a);
			return s;
		}
	protected:
		
	private:
		static inline int set_sock_timeo(int sockfd, int optname, int millisec){
			struct timeval tv;

			tv.tv_sec  = millisec / 1000;
			tv.tv_usec = (millisec % 1000) * 1000;

			return ::setsockopt(sockfd, SOL_SOCKET, optname, (char*)&tv, sizeof(tv));
		}
		lib_net_util_t(const lib_net_util_t& cr);
		lib_net_util_t& operator=(const lib_net_util_t& cr);
	};
}//end namespace el
