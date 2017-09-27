/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	文件相关操作
	brief:		ok
*********************************************************************/

#pragma once

#include "lib_include.h"

namespace el{
	class lib_file_t{
	public:	
		//************************************
		// Brief:     Set the given fd to be blocking or noblocking
		// Returns:   int	0 on success, -1 on error.
		// Parameter: int fd	file descriptor to be set.
		// Parameter: bool is_block	true:blocking, false:nonblocking
		//************************************
		static int set_io_block(int fd, bool is_block);
		static int fcntl_add_flag(int fd, int add_flag);

		//************************************
		// Brief:	  close the given fd(if fd is VALID),and set to be -1 
		// Returns:   int (0 on success, -1 on error)
		// Parameter: int & s (fd)
		// 其中判定 -1 != fd  时才可关闭
		//************************************
		static int close_fd(int& fd);

		//************************************
		// @brief:	Disables further receive operations.
		// 只用于关闭socket可读
		// return:	int (0 on success, -1 on error)
		//************************************
		static int shutdown_rd(int fd);
#ifndef WIN32
		//************************************
		// Brief:     得到目录下的文件
		// Returns:   int	0:OK 其它:失败
		// Parameter: const char * path 目录的路径
		// Parameter: std::vector<std::string> & file_names  返回的文件名
		//************************************
		static int get_dir_file(const char* path, std::vector<std::string>& file_names);
#endif
	protected:
	private:
		lib_file_t(const lib_file_t& cr);
		lib_file_t& operator=(const lib_file_t& cr);
	};

}//end namespace el
