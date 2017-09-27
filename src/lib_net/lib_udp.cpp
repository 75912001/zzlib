#include <lib_include.h>
#include "lib_net/lib_udp.h"

int el::lib_udp_t::send( const void* buf, int total )
{
#ifdef WIN32
	return HANDLE_EINTR(::sendto(this->fd, (char*)buf, total, 0, (sockaddr*)&(this->addr), sizeof(this->addr)));
#else 
	return (int)HANDLE_EINTR(::sendto(this->fd, buf, total, MSG_NOSIGNAL, (sockaddr*)&(this->addr), sizeof(this->addr)));
#endif
}

int el::lib_udp_t::recv( void* data, int len )
{
	return 0;
}

int el::lib_udp_t::connect( const std::string& ip, uint16_t port )
{
	::memset(&(this->addr), 0, sizeof(this->addr));
	this->addr.sin_family = AF_INET;
	this->addr.sin_port = htons(port);
	::inet_pton(AF_INET, ip.c_str(), &(this->addr.sin_addr));
	this->fd = ::socket(AF_INET, SOCK_DGRAM, 0);
	return this->fd;
}
