#include "lib_util.h"
#include "lib_net/lib_net_util.h"
#include "lib_file.h"

int el::lib_net_util_t::get_ip_addr( const char* nif, int af, std::string& ipaddr )
{
#ifndef WIN32
	char ip[INET6_ADDRSTRLEN] = {0};
	assert((af == AF_INET) || (af == AF_INET6));

	// get a list of network interfaces
	struct ifaddrs* ifaddr;
	if (::getifaddrs(&ifaddr) < 0) {
		return ERR;
	}

	// walk through linked list
	int err = EADDRNOTAVAIL;
	int ret_code = -1;
	struct ifaddrs* ifa; // maintaining head pointer so we can free list later
	for (ifa = ifaddr; ifa != 0; ifa = ifa->ifa_next) {
		if ((ifa->ifa_addr == 0) || (ifa->ifa_addr->sa_family != af)
			|| ::strcmp(ifa->ifa_name, nif)) {
			continue;
		}
		// convert binary form ip address to numeric string form
		ret_code = getnameinfo(ifa->ifa_addr,
			(af == AF_INET) ? (socklen_t)sizeof(struct sockaddr_in) : (socklen_t)sizeof(struct sockaddr_in6),
			(char*)ip, INET6_ADDRSTRLEN, 0, 0, NI_NUMERICHOST);
		if (ret_code != 0) {
			if (ret_code == EAI_SYSTEM) {
				err = errno;
			}
			ret_code = -1;
		}
		break;
	}

	::freeifaddrs(ifaddr);
	errno = err;
	ipaddr = ip;
	return ret_code;
#else
	return 0;
#endif

}

int el::lib_net_util_t::family_to_level( int family )
{
	int level = -1;

	switch (family) {
	case AF_INET:
		level = IPPROTO_IP;
		break;
	case AF_INET6:
		level = IPPROTO_IPV6;
		break;
	default:
		break;
	}

	return level;
}

bool el::lib_net_util_t::get_local_ip( std::string& ip )
{
	//////////////////////////////////////////////////////////////////////////
	// Don't use 'gethostbyname(NULL)'. The winsock DLL may be replaced by a DLL from a third party
	// which is not fully compatible to the original winsock DLL. ppl reported crash with SCORSOCK.DLL
	// when using 'gethostbyname(NULL)'.
	char szHost[256];
	if (gethostname(szHost, sizeof szHost) == 0){
		hostent* pHostEnt = gethostbyname(szHost);
		if (pHostEnt != NULL && pHostEnt->h_length == 4 && pHostEnt->h_addr_list[0] != NULL){
			char* pTemp = inet_ntoa(*((in_addr*)pHostEnt->h_addr_list[0]));
			ip = pTemp;
			return true;
		}
	}
	return false;
}

bool el::lib_net_util_t::get_local_ip( long& ip )
{
	char szHost[256];
	if (gethostname(szHost, sizeof szHost) == 0){
		hostent* pHostEnt = gethostbyname(szHost);
		if (pHostEnt != NULL && pHostEnt->h_length == 4 && pHostEnt->h_addr_list[0] != NULL){
			ip = *((long*)pHostEnt->h_addr_list[0]);
			return true;
		}
	}
	return false;
}






