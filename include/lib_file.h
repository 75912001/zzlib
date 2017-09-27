/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	�ļ���ز���
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
		// �����ж� -1 != fd  ʱ�ſɹر�
		//************************************
		static int close_fd(int& fd);

		//************************************
		// @brief:	Disables further receive operations.
		// ֻ���ڹر�socket�ɶ�
		// return:	int (0 on success, -1 on error)
		//************************************
		static int shutdown_rd(int fd);
#ifndef WIN32
		//************************************
		// Brief:     �õ�Ŀ¼�µ��ļ�
		// Returns:   int	0:OK ����:ʧ��
		// Parameter: const char * path Ŀ¼��·��
		// Parameter: std::vector<std::string> & file_names  ���ص��ļ���
		//************************************
		static int get_dir_file(const char* path, std::vector<std::string>& file_names);
#endif
	protected:
	private:
		lib_file_t(const lib_file_t& cr);
		lib_file_t& operator=(const lib_file_t& cr);
	};

}//end namespace el
