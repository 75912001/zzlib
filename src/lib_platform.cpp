#include "lib_platform.h"
#include "lib_log.h"

#ifdef WIN32

int gettimeofday( struct timeval* tv, struct timezone* tp )
{
	time_t rawtime; 
	time(&rawtime); 

	tv->tv_sec = (long)rawtime;

	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	tv->tv_usec = wtm.wMilliseconds * 1000;
	return 0;
}

struct tm* localtime_r( const time_t* timep, struct tm* result )
{
	struct tm* ptm;
 	ptm = ::localtime(timep);

	result->tm_hour = ptm->tm_hour;
	result->tm_isdst = ptm->tm_isdst;
	result->tm_mday = ptm->tm_mday;
	result->tm_min = ptm->tm_min;
	result->tm_mon = ptm->tm_mon;
	result->tm_sec = ptm->tm_sec;
	result->tm_wday = ptm->tm_wday;
	result->tm_yday = ptm->tm_yday;
	result->tm_year = ptm->tm_year;
	return result;
}

#define zend_isinf(a)   ((_fpclass(a) == _FPCLASS_PINF) || (_fpclass(a) == _FPCLASS_NINF))
#define zend_isnan(x)   _isnan(x)

double round( double val, int places /*= 1*/ )
{
	double t;
	double f = pow(10.0, (double) places);
	double x = val * f;

	if (zend_isinf(x) || zend_isnan(x)) {
		return val;
	}

	if (x >= 0.0) {
		t = ceil(x);
		if ((t - x) > 0.50000000001) {
			t -= 1.0;
		}
	} else {
		t = ceil(-x);
		if ((t + x) > 0.50000000001) {
			t -= 1.0;
		}
		t = -t; 
	}
	x = t / f;

	return !zend_isnan(x) ? x : t;
}

// int strncasecmp( const char *s1, const char *s2, size_t n )
// {
// 	register const unsigned char *p1 = (const unsigned char *) s1;
// 	register const unsigned char *p2 = (const unsigned char *) s2;
// 	unsigned char c1, c2;
// 	if (p1 == p2 || n == 0)
// 		return 0;
// 	do
// 	{
// 		c1 = tolower (*p1++);
// 		c2 = tolower (*p2++);
// 		n--;
// 		if (c1 == '' || n == 0)
// 			break;
// 	}
// 	while (c1 == c2);
// 	return c1 - c2;
// }

#endif//WIN32

int WSAStartup()
{
#ifndef WIN32
	return 0;
#else
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		BOOT_LOG(-1,"WSAStartup failed with error:%d", err);
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		BOOT_LOG(-1,"Could not find a usable version of Winsock.dll");
		WSACleanup();
		return -1;
	}
	return 0;
#endif//WIN32
}




