/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	锁
	brief:		OK
*********************************************************************/

#pragma  once 

#include "lib_include.h"
#include "lib_util.h"


namespace el{
#ifndef WIN32
	//互斥锁
	class lib_lock_mutex_t{
	public:
		pthread_mutex_t lock_mutex;
		lib_lock_mutex_t(){
			::pthread_mutex_init(&this->lock_mutex, NULL);
		}
		virtual ~lib_lock_mutex_t(){
			::pthread_mutex_destroy(&this->lock_mutex);
		}
	public:
		//************************************
		// Brief:     加锁
		// Returns:   int 0:上锁成功
		//************************************
		inline int lock(){
			return ::pthread_mutex_lock(&this->lock_mutex);
		}
		//************************************
		// Brief:     解锁
		// Returns:   int 0:解锁成功
		//************************************
		inline int ulock(){
			return ::pthread_mutex_unlock(&this->lock_mutex);
		}
		//************************************
		// Brief:     尝试上锁
		// Returns:   int: 上锁成功:0		上锁失败(已被锁:EBUSY) 
		//************************************
		inline int try_lock(){
			return ::pthread_mutex_trylock(&this->lock_mutex);
		}
	private:
		lib_lock_mutex_t(const lib_lock_mutex_t& cr);
		lib_lock_mutex_t& operator=(const lib_lock_mutex_t& cr);
	};
#else
	//互斥锁
	class lib_lock_mutex_t{
	public:
		lib_lock_mutex_t(){}
		virtual ~lib_lock_mutex_t(){}
	public:
		//************************************
		// Brief:     加锁
		// Returns:   int 0:上锁成功
		//************************************
		inline int lock(){
			return 0;
		}
		//************************************
		// Brief:     解锁
		// Returns:   int 0:解锁成功
		//************************************
		inline int ulock(){
			return 0;
		}
		//************************************
		// Brief:     尝试上锁
		// Returns:   int: 上锁成功:0		上锁失败(已被锁:EBUSY) 
		//************************************
		inline int try_lock(){
			return 0;
		}
	private:
		lib_lock_mutex_t(const lib_lock_mutex_t& cr);
		lib_lock_mutex_t& operator=(const lib_lock_mutex_t& cr);
	};
#endif
}//end namespace el

