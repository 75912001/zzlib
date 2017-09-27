/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	ok
	brief:		¹ÜµÀ	
*********************************************************************/

#pragma once

#ifndef WIN32
namespace el{
	enum {
		E_PIPE_INDEX_RDONLY = 0,
		E_PIPE_INDEX_WRONLY = 1,
		E_PIPE_INDEX_MAX,
	};

	#pragma pack(1)

	struct lib_pipe_t {
		lib_pipe_t();
		int create();
		int close(int mode);
		int read_fd();
		int write_fd();
	private:
		int handles[E_PIPE_INDEX_MAX];
	};

	#pragma pack()
}
#endif