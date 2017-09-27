/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	线程		
	brief:		
*********************************************************************/

#pragma  once 

#include "lib_include.h"
#include "lib_pipe.h"


//<errno.h> 会把errno扩展成一个函数,访问errno变量的某个局限于线程的副本	
//见unix网络编程第二版卷2,9页	
//#define _POSIX_C_SOURCE	199506L	
#define _REENTRANT

namespace el{
#ifndef WIN32
	//单向事件
	class lib_event_pipe_t{
	public:
		lib_event_pipe_t();
		virtual ~lib_event_pipe_t(){}
		int open();
		//************************************
		// Returns:   int > 0: 有事件发生,可以调用应用层处理	
		// 由触发事件者调用.用来通知工作线程有事件到来	
		//************************************
		int wait_event();
		int notify_event();
		void close();
	private:
		lib_pipe_t event_pipe;
		int select_fd_max;
		lib_event_pipe_t(const lib_event_pipe_t& cr);
		lib_event_pipe_t& operator=(const lib_event_pipe_t& cr);
	};

	class lib_thread_t{
	public:
		lib_thread_t();
		virtual ~lib_thread_t();
		//************************************
		// Parameter: void * pdata应用层调用的函数参数		
		//************************************
		int start(void* pdata = NULL);
		int stop();
		virtual int do_work_begin(void* data);
		virtual void do_work_fn(void* data) = 0;
		virtual int do_work_end(void* data);
		int notify_event();
		
		void* p_fun_data;
		bool is_run;
		lib_event_pipe_t msg_event;
	protected:
	private:
		pthread_t thread_id;
	private:
		lib_thread_t(const lib_thread_t& cr);
		lib_thread_t& operator=(const lib_thread_t& cr);
	};
#else
	class lib_thread_t{};
#endif
}


