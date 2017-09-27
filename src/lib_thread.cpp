#include <lib_include.h>
#include "lib_thread.h"

#include "lib_util.h"
#include "lib_log.h"

#ifndef WIN32
namespace {
	void* thread_fun(void* pdata){
		el::lib_thread_t *pthread = (el::lib_thread_t *)pdata;
		if (SUCC != pthread->do_work_begin(pdata)){
			exit(0);
		}
		
		while(pthread->is_run){
			if (0 < pthread->msg_event.wait_event()){
				pthread->do_work_fn(pthread->p_fun_data);
			}
		}

		while (SUCC != pthread->do_work_end(pdata)){
		}
		return NULL;
	}
}

int el::lib_thread_t::start( void* pdata)
{
	this->p_fun_data = pdata;

	int r = ::pthread_create(&this->thread_id, NULL, thread_fun, (void*)this);

	if (0 != r){
		ALERT_LOG("[err_code:%d, err:%s]", errno, strerror(errno));
		return r;
	}
	return SUCC;
}

el::lib_thread_t::lib_thread_t()
{
	this->thread_id = 0;
	this->is_run = true;
	this->p_fun_data = NULL;

	int r = this->msg_event.open();
	if (0 != r){
		ALERT_LOG("[ret:%d]", r);
	}
}

el::lib_thread_t::~lib_thread_t()
{
	this->is_run = false;
	::pthread_join(this->thread_id, NULL);
	::pthread_detach(this->thread_id);

	this->msg_event.close();
}

int el::lib_thread_t::stop()
{
	this->is_run = false;

	return SUCC;
}

int el::lib_thread_t::notify_event()
{
	return msg_event.notify_event();
}

int el::lib_thread_t::do_work_begin( void* data )
{
	return SUCC;
}

int el::lib_thread_t::do_work_end( void* data )
{
	return SUCC;
}

int el::lib_event_pipe_t::wait_event()
{
	//!!!这里的日志不能使用线程打印,否则是死循环,继续调用日志事件!!!	

	timeval time_out_val;
	time_out_val.tv_sec = 0;
	time_out_val.tv_usec = 100000;//100毫秒

	fd_set fd_read_set;
	FD_ZERO(&fd_read_set);
	FD_SET(this->select_fd_max, &fd_read_set);

	int r = HANDLE_EINTR(::select(this->select_fd_max + 1, &fd_read_set, NULL, NULL, &time_out_val));
	if (0 == r){//time out
		return 0;
	}

	if (ERR == r){
		if (EBADF == errno){
			//ALERT_LOG("[err_code:%d, err:%s]", errno, strerror(errno));
		}
		return ERR;
	}

	if(FD_ISSET(this->select_fd_max, &fd_read_set)){
		char sz[100];
		int ret = (int)HANDLE_EINTR(::read(this->select_fd_max, sz, sizeof(sz)));
		return ret;
	}

	return 0;
}

int el::lib_event_pipe_t::open()
{
	if (SUCC != this->event_pipe.create()){
		return ERR;
	}
	this->select_fd_max = this->event_pipe.read_fd();
	return SUCC;
}

void el::lib_event_pipe_t::close()
{
	this->event_pipe.close(E_PIPE_INDEX_RDONLY);
	this->event_pipe.close(E_PIPE_INDEX_WRONLY);
}

int el::lib_event_pipe_t::notify_event()
{
	char f;
	int r = (int)HANDLE_EINTR(::write(this->event_pipe.write_fd(), &f, 1));
	if (-1 == r){
		ALERT_LOG("[err_code:%d, err:%s]", errno, strerror(errno));
	}
	return r;
}

el::lib_event_pipe_t::lib_event_pipe_t()
{
	this->select_fd_max = INVALID_FD;
}
#endif