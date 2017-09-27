/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	协议
	brief:		ok
*********************************************************************/
// 检查 [11/5/2014 kevin]
#pragma once

#include "lib_include.h"
#include "lib_msgbuf.h"
#include "lib_log.h"
namespace el{
	#pragma pack(1)
	class lib_proto_head_base_t
	{
	public:
	public:
		lib_proto_head_base_t(){}
		virtual ~lib_proto_head_base_t(){}
		//************************************
		// @brief:	获取包头长度
		//************************************
		virtual uint32_t get_proto_head_len() = 0;
		//************************************
		// @brief:	len 字段是否是整个包的长度
		//************************************
		virtual bool is_all_len() = 0;
		/**
		 * @brief	获取包的总长
		 */
		virtual uint32_t get_all_len() = 0;
		/**
		 * @brief	获取包体长度
		 */
		virtual uint32_t get_body_len() = 0;
		/**
		 * @brief	设置包中len字段
		 */
		virtual void set_len(uint32_t len) = 0;
		/**
		 * @brief	获取命令号
		 */
		virtual uint32_t get_cmd() = 0;
		/**
		 * @brief	获取包中len字段指针
		 */
		virtual void* get_len_pointer() = 0;
		//************************************
		// @brief:	包头中len字段的偏移长度
		//************************************
		virtual uint32_t len_offset() = 0;
		//************************************
		// @brief:	获取包头数据的指针
		//************************************
		virtual void* get_data_pointer() = 0;
		virtual void trace_log(){}
		virtual void error_log(){}
		//************************************
		// @brief:	把data 数据解到包头中
		//************************************
		virtual void unpack(const void* data) = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	//example:使用接收数据的例子
	//接收客户端的数据

	class lib_example_recv_data_cli_t : public lib_recv_data_t{
	public:
		lib_example_recv_data_cli_t(const void* recvdata, int readpos = 0)
			: lib_recv_data_t(recvdata, readpos){
		}
	public:
		//第3个字节开始,4字节长度是包体长度
		//返回消息总长度(包头+包体长度)
		virtual uint32_t get_len();
		inline uint32_t remain_len() {
			return this->get_len() - this->read_pos;
		}
	protected:
	private:
		lib_example_recv_data_cli_t(const lib_example_recv_data_cli_t& cr); // 拷贝构造函数
		lib_example_recv_data_cli_t& operator=( const lib_example_recv_data_cli_t& cr); // 赋值函数
	};
	//////////////////////////////////////////////////////////////////////////
	/*协议包头格式(例子) */
	#define PACKET_MAGIC_D ('S' << 8 | 'D') //'DS' 未压缩
	#define PACKET_MAGIC_C ('S' << 8 | 'C') //'CS' 压缩
	struct lib_example_proto_head_t : public lib_proto_head_base_t{
		uint16_t	magic;//是否压缩
		uint32_t	len;//len:包体长度
		uint32_t 	cmd;//cmd
		uint32_t 	param;//id
		uint32_t	seq;/* 序列号 */
		uint32_t	ret; /* S->C, 错误码 */
		static const uint32_t PROTO_HEAD_LEN = 22;
		virtual uint32_t get_proto_head_len(){
			return lib_example_proto_head_t::PROTO_HEAD_LEN;
		}
		virtual void unpack(const void* data){
			el::lib_example_recv_data_cli_t in(data);
			in>>this->magic
				>>this->len
				>>this->cmd
				>>this->param
				>>this->seq
				>>this->ret;
		}
		virtual uint32_t len_offset(){
			return sizeof(this->magic);
		}
		virtual void trace_log(){
			TRACE_MSG_LOG("[magic:%u, len:%u, cmd:%#x, id:%u, seq:%u, ret:%#x]", 
				this->magic, this->len, this->cmd, this->param, this->seq, this->ret);
		}
		virtual void error_log(){
			ERROR_LOG("[magic:%u, len:%u, cmd:%#x, id:%u, seq:%u, ret:%#x]", 
				this->magic, this->len, this->cmd, this->param, this->seq, this->ret);
		}
		virtual void* get_len_pointer(){
			return &this->len;
		}
		virtual uint32_t get_all_len(){
			return this->len + lib_example_proto_head_t::PROTO_HEAD_LEN;
		}
		virtual uint32_t get_body_len(){
			return this->len;
		}
		virtual uint32_t get_cmd(){
			return this->cmd;
		}
		virtual void set_len(uint32_t len){
			this->len = len;
		}
		virtual bool is_all_len(){
			return false;
		}
		virtual void* get_data_pointer(){
			return &this->magic;
		}
		//////////////////////////////////////////////////////////////////////////
		void set_param(uint32_t param){
			this->param = param;
		}
		uint32_t get_param(){
			return this->param;
		}
		void set_seq(uint32_t seq){
			this->seq = seq;
		}
		uint32_t get_seq(){
			return this->seq;
		}
		uint32_t get_ret(){
			return this->ret;
		}
		lib_example_proto_head_t(){
			this->clear();
		}
		lib_example_proto_head_t(uint32_t cmd, uint32_t param, uint32_t seq = 0, uint32_t ret = 0){
			this->clear();
			this->cmd = cmd;
			this->param = param;
			this->seq = seq;
			this->ret = ret;
		}
		void clear(){
			this->magic = PACKET_MAGIC_D;
			this->len = 0;
			this->cmd = 0;
			this->param = 0;
			this->seq = 0;
			this->ret = 0;
		}
		static inline uint32_t get_seq_pos(){
			return (uint32_t)(sizeof(uint16_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t));
		}
		static inline uint32_t get_cmd_pos(){
			return (uint32_t)(sizeof(uint16_t) + sizeof(uint32_t));
		}
		//获取包头长度的最小包长
		static inline uint32_t get_pkg_len_min_len(){
			return (uint32_t)(sizeof(uint16_t) + sizeof(uint32_t));//magic +　len
		}
		//获取包头长度的开始位置
		static inline uint32_t get_pkg_len_pos(){
			return sizeof(uint16_t);//magic
		}
	};

	#pragma pack()
}

