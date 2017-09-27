#include "lib_log.h"
#include "lib_net/lib_tcp.h"
#include "lib_file.h"
#include "lib_net/lib_net_util.h"

#include "lib_net/lib_multicast.h"
#include "lib_platform.h"

int el::lib_mcast_t::create(const std::string& mcast_ip, uint16_t mcast_port,
								 const std::string& mcast_incoming_if, const std::string& mcast_outgoing_if)
{
	this->mcast_ip = mcast_ip;
	this->mcast_port = mcast_port;
	this->mcast_incoming_if = mcast_incoming_if;
	this->mcast_outgoing_if = mcast_outgoing_if;

	this->fd = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (INVALID_FD == this->fd) {
		ALERT_LOG("failed to create mcast_fd [err_code:%d, err:%s]", errno, strerror(errno));
		return ERR;
	}

	lib_file_t::set_io_block(this->fd, false);

	::memset(&this->addr, 0, sizeof(this->addr));
	/*
	 *ai_family����ָ���������ڴ����ص��׽ӿڵ�ַ�ṹ������
	 *����ֵ�������֣�AF_INET��AF_INET6��AF_UNSPEC�����ָ��AF_INET��
	 *��ô�����Ͳ��ܷ����κ�IPV6��صĵ�ַ��Ϣ�������ָ����AF_INET6��
	 *��Ͳ��ܷ����κ�IPV4��ַ��Ϣ��AF_UNSPEC����ζ�ź������ص���������ָ��
	 *�������ͷ��������ʺ��κ�Э����ĵ�ַ�����ĳ����������AAAA��¼(IPV6)��ַ��
	 *ͬʱ����A��¼(IPV4)��ַ����ôAAAA��¼����Ϊsockaddr_in6�ṹ���أ�
	 ��A��¼����Ϊsockaddr_in�ṹ����
	*/
	this->addr.sin_family = AF_INET;//AF_UNSPEC ;
#ifndef WIN32
	::inet_pton(AF_INET, this->mcast_ip.c_str(), &(this->addr.sin_addr));
#else
	this->addr.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr(this->mcast_ip.c_str());//
#endif
	this->addr.sin_port = htons(this->mcast_port);

	lib_tcp_t::set_reuse_addr(this->fd);

	int loop = 1;
	if (-1 == ::setsockopt(this->fd, IPPROTO_IP, IP_MULTICAST_LOOP, (char*)&loop, sizeof(loop))){
		ALERT_LOG("failed to set ip_multicast_loop [err_code:%d, err:%s]", errno, strerror(errno));
		return ERR;
	}

	// set default interface for outgoing multicasts
#ifndef WIN32
	in_addr_t ipaddr;
#else
	in_addr ipaddr;
#endif

	::inet_pton(AF_INET, this->mcast_outgoing_if.c_str(), &ipaddr);
	if (::setsockopt(this->fd, IPPROTO_IP, IP_MULTICAST_IF, (char*)&ipaddr, sizeof ipaddr) == -1) {
		ALERT_LOG("failed to set outgoing interface [err_code:%d, err:%s, ip:%s]",
			errno, strerror(errno), this->mcast_outgoing_if.c_str());
		return ERR;
	}

	if (::bind(this->fd, (struct sockaddr*)&this->addr, sizeof(this->addr)) == -1) {
		ALERT_LOG("failed to bind mcast_fd [err_code:%d, err:%s]", errno, strerror(errno));
		return ERR;
	}
#ifdef WIN32
	::inet_pton(AF_INET, this->mcast_ip.c_str(), &(this->addr.sin_addr));
#endif

	return this->mcast_join();
}

int el::lib_mcast_t::mcast_join()
{
#ifndef WIN32
	struct group_req req;

	req.gr_interface = if_nametoindex(this->mcast_incoming_if.c_str());
	if (req.gr_interface == 0) {
		errno = ENXIO; /* i/f name not found */
		ALERT_LOG("failed [err_code:%d, err:%s, mcast_incoming_if:%s]", 
			errno, strerror(errno), this->mcast_incoming_if.c_str());
		return ERR;
	}

	memcpy(&req.gr_group, &this->addr, sizeof(this->addr));
	if (-1 == ::setsockopt(this->fd, lib_net_util_t::family_to_level(this->addr.sin_family), MCAST_JOIN_GROUP, (char*)&req, sizeof(req))) {
		ERROR_LOG("failed to join mcast grp [err_code:%d, err:%s]", errno, strerror(errno));
		return ERR;
	}
#else
	struct ip_mreq mreq;
	mreq.imr_interface.s_addr = inet_addr(this->mcast_outgoing_if.c_str());
	mreq.imr_multiaddr.s_addr = inet_addr(this->mcast_ip.c_str());
	if(setsockopt(this->fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq)) == -1){
		ERROR_LOG("[UdpListenSocketBase][Listen][IP_ADD_MEMBERSHIP error]");
		return -1;
	}
#endif
	return SUCC;
}

el::lib_mcast_t::lib_mcast_t()
{
	this->mcast_port = 0;
}





