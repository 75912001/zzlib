/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	��
	brief:		OK
*********************************************************************/

#pragma  once 

#include "lib_include.h"
#include "lib_util.h"


namespace el{
#ifndef WIN32
	//������
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
		// Brief:     ����
		// Returns:   int 0:�����ɹ�
		//************************************
		inline int lock(){
			return ::pthread_mutex_lock(&this->lock_mutex);
		}
		//************************************
		// Brief:     ����
		// Returns:   int 0:�����ɹ�
		//************************************
		inline int ulock(){
			return ::pthread_mutex_unlock(&this->lock_mutex);
		}
		//************************************
		// Brief:     ��������
		// Returns:   int: �����ɹ�:0		����ʧ��(�ѱ���:EBUSY) 
		//************************************
		inline int try_lock(){
			return ::pthread_mutex_trylock(&this->lock_mutex);
		}
	private:
		lib_lock_mutex_t(const lib_lock_mutex_t& cr);
		lib_lock_mutex_t& operator=(const lib_lock_mutex_t& cr);
	};
#else
	//������
	class lib_lock_mutex_t{
	public:
		lib_lock_mutex_t(){}
		virtual ~lib_lock_mutex_t(){}
	public:
		//************************************
		// Brief:     ����
		// Returns:   int 0:�����ɹ�
		//************************************
		inline int lock(){
			return 0;
		}
		//************************************
		// Brief:     ����
		// Returns:   int 0:�����ɹ�
		//************************************
		inline int ulock(){
			return 0;
		}
		//************************************
		// Brief:     ��������
		// Returns:   int: �����ɹ�:0		����ʧ��(�ѱ���:EBUSY) 
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

