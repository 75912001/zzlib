#include "lib_file.h"
#include "lib_util.h"
#include "lib_platform.h"

#ifndef WIN32
int el::lib_file_t::get_dir_file( const char *path, std::vector<std::string>& file_names )
{
	DIR* dirp;
	dirp = ::opendir(path);
	if (NULL != dirp) {
		while(1) {
			struct dirent* direntp;
			direntp = ::readdir(dirp);
			if ( NULL == direntp ) {
				break;
			}
			if (DT_REG == direntp->d_type) {
				std::string name = direntp->d_name;
				file_names.push_back(name);
			}
		}
		::closedir(dirp);
	} else {
		return ERR;
	}
	return SUCC;
}
#endif

#ifndef WIN32
int el::lib_file_t::set_io_block( int fd, bool is_block )
{
	int val;
	if (is_block) {
		val = (~O_NONBLOCK & ::fcntl(fd, F_GETFL));
	} else {
		val = (O_NONBLOCK | ::fcntl(fd, F_GETFL));
	}
	return ::fcntl(fd, F_SETFL, val);
}
#else
int el::lib_file_t::set_io_block( int fd, bool is_block )
{
	u_long mode = 0;
	if (!is_block){
		mode = 1;
	}
	int ret = ioctlsocket(fd, FIONBIO, &mode);
	if (NO_ERROR != ret){
		return SOCKET_ERROR; 
	}
	return SUCC;
}
#endif

int el::lib_file_t::fcntl_add_flag( int fd, int add_flag )
{
#ifndef WIN32
	int flag  = ::fcntl(fd, F_GETFD, 0);
	flag |= add_flag;
	return ::fcntl(fd, F_SETFD, flag);
#else
	return SUCC;
#endif
}

int el::lib_file_t::close_fd( int& fd )
{
	int ret = SUCC;
	if (INVALID_FD != fd){
#ifdef WIN32
		ret = ::closesocket(fd);
#else
		ret = HANDLE_EINTR(::close(fd));
#endif
		if(ERR != ret){
			fd = INVALID_FD;
		}
	}
	return ret;
}

int el::lib_file_t::shutdown_rd( int fd )
{
	int ret = SUCC;
	if (INVALID_FD != fd){
		ret = ::shutdown(fd, SHUT_RD);
	}
	return ret;
}
