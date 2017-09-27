/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	�����ڴ�����
	brief:		OK
*********************************************************************/

#pragma once

#include "lib_include.h"
#include "lib_util.h"
//#include "lib_lock.h"

namespace el{
	//////////////////////////////////////////////////////////////////////////
	//�Զ����޷Ŵ�,���ǲ����Զ�����.
	class lib_active_buf_t{

	private:

//		lib_lock_mutex_t lock;
	public:
		char* data;//����ͷָ��
		uint32_t total_len;//�ѷ�����ܳ���
		uint32_t write_pos;//��ʹ�õ���λ��
		lib_active_buf_t(){
			this->init_data();
		}
		virtual ~lib_active_buf_t(){
			this->clean();
		}
		//************************************
		// Brief:     ����������
		// Returns:   void
		// Parameter: const char * const pdata ����
		// Parameter: uint32_t len ���ݳ���
		//************************************
		inline void push_back(const char* const pdata, uint32_t len){
			uint32_t remain_len = this->total_len - this->write_pos;
			if(remain_len >= len){
				::memcpy(this->data + this->write_pos, pdata, len);
				this->write_pos += len;
			}else{
				this->data = (char*)::realloc(this->data, len + this->write_pos);
				::memcpy(this->data + this->write_pos, pdata, len);
				this->total_len = len + this->write_pos;
				this->write_pos = this->total_len;
			}
		}
		//************************************
		// Brief:	  ������ͷ�е�������Ϊlen������
		// Returns:   u32 (m_pos,ʣ������ݳ���(0:û��ʣ��))
		// Parameter: u32 len (��������)
		//************************************
		inline uint32_t pop_front(uint32_t len){
			if(this->write_pos <= len){
				//ȫ������
				this->clean();
			}else{
				::memmove(this->data, this->data + len, this->write_pos - len);
				this->write_pos -= len;
			}
			return this->write_pos;
		}
		//************************************
		// Brief:	������������/�ͷ��ڴ�ռ�  
		// Returns:   void ()
		//************************************
		inline void clean(){
			SAFE_FREE(this->data);
			this->init_data();
		}
	private:
		inline void init_data(){
			this->data = NULL;
			this->total_len = 0;
			this->write_pos = 0;
		}
	private:
		lib_active_buf_t(const lib_active_buf_t& cr);
		lib_active_buf_t& operator=(const lib_active_buf_t& cr);
	};

}//end namespace el
