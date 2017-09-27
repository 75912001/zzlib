/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	协议消息
	brief:		ok
*********************************************************************/

#pragma once

#include "lib_util.h"
#include "lib_packer.h"
#include "lib_memory.h"

namespace el{

#define GEN_READ_SINGLE_VALUE_FUNC(func, value_type)\
	inline bool func(value_type& value){\
		if(!this->is_read_only || (this->postion + sizeof(value_type) > this->size)) return false;\
		value = *(value_type*)(this->buf + this->postion);\
		value = EL_BYTE_SWAP(value);\
		this->postion += (uint32_t)sizeof(value_type);\
		return true;\
	}

#define GEN_READ_SINGLE_VALUE_FUNC_STRING16(func)\
	inline bool func(std::string& value, uint32_t max_len__){\
		if(!this->is_read_only || (this->postion + sizeof(uint16_t) > this->size)) return false;\
		uint16_t len = *(uint16_t*)(this->buf + this->postion);\
		len = EL_BYTE_SWAP(len);\
		if (len > max_len__) return false;\
		this->postion += (uint32_t)sizeof(uint16_t);\
		if(this->postion + len > this->size){\
			return false;\
		}\
		value.append(this->buf + this->postion, len);\
		this->postion += len;\
		return true;\
	}

#define GEN_WRITE_SINGLE_VALUE_FUNC(func, value_type)\
	inline bool func(const value_type value){\
		if(this->is_read_only) return false;\
		value_type new_value = EL_BYTE_SWAP(value);\
		this->w_buf.push_back((char*)&new_value, sizeof(value_type));\
		return true;\
	}
	
#define GEN_WRITE_SINGLE_VALUE_FUNC_STRING16(func)\
	inline bool func(const std::string& value, uint32_t max_len__){\
		if(this->is_read_only || value.size() > max_len__) return false;\
		uint16_t len = (uint16_t)value.size();\
		len = EL_BYTE_SWAP(len);\
		this->w_buf.push_back((char*)&len, sizeof(uint16_t));\
		this->w_buf.push_back(value.c_str(), (uint32_t)value.size());\
		return true;\
	}

	class lib_msg_byte_t
	{

	public:
		char* buf;
		uint32_t size;
		uint32_t postion;
		bool is_read_only;
		lib_active_buf_t w_buf;
		lib_msg_byte_t(char* data, uint32_t len){
			this->is_read_only = true;
			this->size = len;
			this->postion = 0;
			this->buf = data;
		}
		lib_msg_byte_t(){
			this->is_read_only = false;
		}
		virtual ~lib_msg_byte_t(){}
		inline bool is_end(){return this->postion == this->size;}
		//read
		GEN_READ_SINGLE_VALUE_FUNC(read_int8, int8_t);
		GEN_READ_SINGLE_VALUE_FUNC(read_int16, int16_t);
		GEN_READ_SINGLE_VALUE_FUNC(read_int32, int32_t);
		GEN_READ_SINGLE_VALUE_FUNC(read_int64, int64_t);
		GEN_READ_SINGLE_VALUE_FUNC(read_uint8, uint8_t);
		GEN_READ_SINGLE_VALUE_FUNC(read_uint16, uint16_t);
		GEN_READ_SINGLE_VALUE_FUNC(read_uint32, uint32_t);
		GEN_READ_SINGLE_VALUE_FUNC(read_uint64, uint64_t);
		GEN_READ_SINGLE_VALUE_FUNC(read_char, char);
		GEN_READ_SINGLE_VALUE_FUNC(read_float, float);
		GEN_READ_SINGLE_VALUE_FUNC_STRING16(read_string16);
		//write
		GEN_WRITE_SINGLE_VALUE_FUNC(write_int8, int8_t);
		GEN_WRITE_SINGLE_VALUE_FUNC(write_int16, int16_t);
		GEN_WRITE_SINGLE_VALUE_FUNC(write_int32, int32_t);
		GEN_WRITE_SINGLE_VALUE_FUNC(write_int64, int64_t);
		GEN_WRITE_SINGLE_VALUE_FUNC(write_uint8, uint8_t);
		GEN_WRITE_SINGLE_VALUE_FUNC(write_uint16, uint16_t);
		GEN_WRITE_SINGLE_VALUE_FUNC(write_uint32, uint32_t);
		GEN_WRITE_SINGLE_VALUE_FUNC(write_uint64, uint64_t);
		GEN_WRITE_SINGLE_VALUE_FUNC(write_char, char);
		GEN_WRITE_SINGLE_VALUE_FUNC(write_float, float);
		GEN_WRITE_SINGLE_VALUE_FUNC_STRING16(write_string16);

		inline bool read_buf(char* data, uint32_t len){
			if(this->postion + len <= this->size) {
				::memcpy(data, this->buf + this->postion, len);
				this->postion += len;
				return true;			
			}
			return false;
		}
		inline bool write_buf(char* data, uint32_t len){
			this->w_buf.push_back(data, len);
			return true;
		}

	protected:
		
	private:
		lib_msg_byte_t(const lib_msg_byte_t& cr);
		lib_msg_byte_t& operator=(const lib_msg_byte_t& cr);
	};



	class lib_msg_t
	{
	public:
		lib_msg_t(){}
		virtual ~lib_msg_t(){}
		virtual void init(){}
		virtual bool read(lib_msg_byte_t& msg_byte){return true;}
		virtual bool write(lib_msg_byte_t& msg_byte){return true;}
		virtual void show(uint32_t idx = 0){}
		
	protected:
		
	private:
	};
	
}//end namespace el  



