#pragma once

#include "lib_include.h"

#ifdef WIN32
	#pragma warning(disable:4819)
	#pragma warning(disable:4996)
	#pragma comment(lib, "ws2_32.lib")

	//////////////////////////////////////////////////////////////////////////
	//
	#ifndef PRIu64
	#define PRIu64 "llu"
	#endif

	//////////////////////////////////////////////////////////////////////////
	//字符交换
	#define bswap_16(x) \
		 ((((x) >> 8) & 0xff) \
		| (((x) & 0xff) << 8))

	#define bswap_32(x) \
		 ((((x) & 0xff000000) >> 24) \
		| (((x) & 0x00ff0000) >>  8) \
		| (((x) & 0x0000ff00) <<  8) \
		| (((x) & 0x000000ff) << 24))

	#define bswap_64(x) \
		((((x)  & 0xff00000000000000ull) >> 56)				      \
		| (((x) & 0x00ff000000000000ull) >> 40)				      \
		| (((x) & 0x0000ff0000000000ull) >> 24)				      \
		| (((x) & 0x000000ff00000000ull) >> 8)				      \
		| (((x) & 0x00000000ff000000ull) << 8)				      \
		| (((x) & 0x0000000000ff0000ull) << 24)				      \
		| (((x) & 0x000000000000ff00ull) << 40)				      \
		| (((x) & 0x00000000000000ffull) << 56))

	/* Values for the second argument to access.
	   These may be OR'd together.  */
	// #define	R_OK	4		/* Test for read permission.  */
	#define	W_OK	2		/* Test for write permission.  */
	// #define	X_OK	1		/* Test for execute permission.  */
	#define	F_OK	0		/* Test for existence.  */
	
	/* The following constants should be used for the second parameter of
	   `shutdown'.  */
	enum
	{
	  SHUT_RD = 0,		/* No more receptions.  */
	  SHUT_WR,		/* No more transmissions.  */
	  SHUT_RDWR		/* No more receptions or transmissions.  */
	};

	#define snprintf _snprintf
	#define strcasecmp stricmp
	/* Compare S1 and S2, ignoring case, returning less than, equal to or
	   greater than zero if S1 is lexiographically less than,
	   equal to or greater than S2.  */
	int strncasecmp (const char *s1, const char *s2, size_t n);

	int gettimeofday(struct timeval* tv, struct timezone* tp);

	struct tm* localtime_r(const time_t* timep, struct tm* result);

	double round(double val, int places = 1);


#ifndef atoll
#define atoll(__val__) atol(__val__)
#endif



	#ifndef va_copy 
	# ifdef __va_copy 
	# define va_copy(DEST,SRC) __va_copy((DEST),(SRC)) 
	# else 
	# define va_copy(DEST, SRC) memcpy((&DEST), (&SRC), sizeof(va_list)) 
	# endif 
	#endif  
#endif

int WSAStartup();
