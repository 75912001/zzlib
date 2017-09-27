/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	基本头文件
	brief:		ok
*********************************************************************/

#pragma once 

#include <stddef.h>
#include <signal.h>
#include <assert.h>
#include <algorithm>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <float.h>
#include <iostream>
#include <limits.h>
#include <list>
#include <map>
#include <math.h>
#include <set>
#include <string>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdexcept>
#include <stdint.h>
#include <sstream>
#include <time.h>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>

#ifdef WIN32
	#undef FD_SETSIZE
	#define FD_SETSIZE 1024
		#include <WinSock2.h>
		#include <winsock.h>
		#include <Ws2tcpip.h>
	#define _WINSOCKAPI_    // stops windows.h including winsock.h
		#include <windows.h>
		#include <io.h>
		#include <process.h>
		#include <netioapi.h>
#else//linux
	#include <byteswap.h>
	#include <dirent.h>
	#include <dlfcn.h>
	#include <ifaddrs.h>
	#include <inttypes.h>  
	#include <netdb.h>
	#include <pthread.h>
	#include <syslog.h>
	#include <unistd.h>
	#include <linux/version.h>
	#include <arpa/inet.h>
	#include <sys/mman.h>
	#include <sys/time.h>
	#include <sys/socket.h>
	#include <sys/epoll.h>
	#include <sys/wait.h>
	#include <sys/resource.h>
	#include <netdb.h>
	#include <net/if.h>
	#include <netinet/in.h>
#endif//end WIN32
