#include "lib_pipe.h"

#include <lib_log.h>
#include <lib_file.h>

#ifndef WIN32
el::lib_pipe_t::lib_pipe_t()
{
	::memset(this->handles, 0, sizeof(this->handles));
}

int el::lib_pipe_t::create()
{
	if (ERR == ::pipe (this->handles)){
		g_log->boot(ERR, 0, "PIPE CREATE FAILED [err:%s]", strerror(errno));
		return ERR;
	}

	lib_file_t::fcntl_add_flag(this->handles[E_PIPE_INDEX_RDONLY], O_NONBLOCK | O_RDONLY);
	lib_file_t::fcntl_add_flag(this->handles[E_PIPE_INDEX_WRONLY], O_NONBLOCK | O_WRONLY);

	// 这里设置为FD_CLOEXEC表示当程序执行exec函数时本fd将被系统自动关闭,表示不传递给exec创建的新进程
	lib_file_t::fcntl_add_flag(this->handles[E_PIPE_INDEX_RDONLY], FD_CLOEXEC);
	lib_file_t::fcntl_add_flag(this->handles[E_PIPE_INDEX_WRONLY], FD_CLOEXEC);

	return SUCC;
}

int el::lib_pipe_t::close( int mode )
{
	if (mode < E_PIPE_INDEX_RDONLY || mode >= E_PIPE_INDEX_MAX){
		return ERR;
	}
	return lib_file_t::close_fd(this->handles[mode]);
}

int el::lib_pipe_t::read_fd()
{
	return this->handles[E_PIPE_INDEX_RDONLY];
}

int el::lib_pipe_t::write_fd()
{
	return this->handles[E_PIPE_INDEX_WRONLY];
}

#endif