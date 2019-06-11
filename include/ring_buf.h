/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	环形缓冲
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
		//len:2的幂
		//return:-1:失败
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
		//放
		//return: 返回放入的长度
		uint32_t in(char* data, uint32_t len){
			//可用空间
			uint32_t free_len = this->get_free_len();

			if (0 == free_len){
				//-todo 缓存已满
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
		//取
		//return:返回取出的长度
		uint32_t out(char* data, uint32_t len){
			uint32_t free_len = this->get_free_len();

			if (this->size == free_len){
				//缓存全空
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
		//用于存放数据的缓存
		void* buffer;
		//缓冲区空间的大小，在初化时，将它向上圆整成2的幂
		uint32_t size;
		//指向buffer中队头
		uint32_t in;
		//指向buffer中的队尾
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
		//判断一个数是否为2的次幂
		inline bool is_power_of_2(uint32_t n){
			return (n != 0 && ((n & (n - 1)) == 0));
 		}
		//取模运算
		inline uint32_t get_mod(uint32_t n){
			return n & (this->size – 1);
		}
		//获取可用长度
		uint32_t get_free_len(){
			return this->size - this->in + this->out;
		}
	}
}//end namespace el
