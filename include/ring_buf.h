/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	���λ���
	brief:		OK
*********************************************************************/

#pragma once

#include "lib_include.h"
#include "lib_util.h"

namespace el{
	class ring_buf_t{
	public:
		ring_buf_t(){
			this->init();
		}
		virtual ~ring_buf_t(){
			this->clean();
		}
		//len:2����
		//return:-1:ʧ��
		int create(uint32_t len){
			if (!this->is_power_of_2(len)){
				return -1;
			}
			this->buffer = (char*)::realloc(this->buffer, len);
			if (NULL == this->buffer){
				return -1;
			}
			this->size = len;
			return 0;
		}
		//��
		//return: ���ط���ĳ���
		uint32_t in(char* data, uint32_t len){
			//���ÿռ�
			uint32_t free_len = this->get_free_len();

			if (0 == free_len){
				//-todo ��������
				return 0;
			}

			if (this->get_mod(this->in) < this->get_mod(this->out)){
				char* p = this->buffer[this->get_mod(this->in)];
				//-todo
			} else {
				uint32_t r_free_len = this->size - this->get_mod(this->in);
				char* rp = this->buffer[this->get_mod(this->in)];
				uint32_t l_free_len = free_len - r_free_len;
				char* lp = this->buffer[0];
				//-todo
			}
			uint32_t put_len = free_len < len ? free_len : len;
			this->in += put_len;
			return put_len;
		}
		//ȡ
		//return:����ȡ���ĳ���
		uint32_t out(char* data, uint32_t len){
			uint32_t free_len = this->get_free_len();

			if (this->size == free_len){
				//����ȫ��
				return 0;
			}
			if (this->get_mod(this->in) < this->get_mod(this->out)){
				char* p = this->buffer[this->get_mod(this->in)];
				//-todo
			} else {
				uint32_t r_free_len = this->size - this->get_mod(this->in);
				char* rp = this->buffer[this->get_mod(this->in)];

				uint32_t l_free_len = free_len - r_free_len;
				char* lp = this->buffer[0];
				//-todo
			}
			
			uint32_t get_len = (this->size - free_len) < len ? (this->size - free_len) : len;
			this->out += get_len;
			return get_len;
		}

	private:
		//���ڴ�����ݵĻ���
		void* buffer;
		//�������ռ�Ĵ�С���ڳ���ʱ����������Բ����2����
		uint32_t size;
		//ָ��buffer�ж�ͷ
		uint32_t in;
		//ָ��buffer�еĶ�β
		uint32_t out;
		inline void clean(){
			SAFE_FREE(this->buffer);
			this->init();
		}
		inline void init(){
			this->buffer = NULL;
			this->in = 0;
			this->out = 0;
		}
		//�ж�һ�����Ƿ�Ϊ2�Ĵ���
		inline bool is_power_of_2(uint32_t n){
			return (n != 0 && ((n & (n - 1)) == 0));
 		}
		//ȡģ����
		inline uint32_t get_mod(uint32_t n){
			return n & (this->size �C 1);
		}
		//��ȡ���ó���
		uint32_t get_free_len(){
			return this->size - this->in + this->out;
		}
	}
}//end namespace el
