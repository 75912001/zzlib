/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	��Ϣ����������(�Զ�ת���ֽ���,����lib_packer.h �ļ��к궨��)
	brief:		ok
*********************************************************************/

#pragma once

#include "lib_byte_swap.h"
#include "lib_packer.h"
#include "lib_util.h"

namespace el{
	//////////////////////////////////////////////////////////////////////////
	//��������
	template <typename T_PROTO_HEAD_TYPE>
	class lib_send_data_t{
	public:
		uint32_t write_pos;//����д����λ��
		void* write_data;
		lib_send_data_t(void* senddata){
			this->init();
			this->write_data = senddata;
		}
		virtual ~lib_send_data_t(){}
		inline void init(){
			this->write_pos = T_PROTO_HEAD_TYPE::PROTO_HEAD_LEN;
		}
		//************************************
		// Brief:     �������ݵ�ͷ�ṹ(��������ǰҪ���ð�ͷ)
		// Warning:		���ú�Ͳ����������д���κ�����,�����ͷ�е��ܳ�����Ҫ�����趨
		//************************************
		virtual void set_head(const T_PROTO_HEAD_TYPE* head) = 0;

		inline void* data(){
			return this->write_data;
		}
		inline uint32_t len(){
			return this->write_pos;
		}
		//�������
		template <typename T>
		inline lib_send_data_t& operator<<(T value){
			lib_packer_t::pack(this->write_data, value, this->write_pos);
			return *this;
		} 

		//�����ڴ��,����+�ڴ�
		//������T_SIZE_TYPE��ȷ������
		template <typename T_SIZE_TYPE>
		inline void pack_str(const char* data, T_SIZE_TYPE len){
			lib_packer_t::pack(this->write_data, len, this->write_pos);
			lib_packer_t::pack(this->write_data, data, len, this->write_pos);
		}
	private:
		lib_send_data_t(const lib_send_data_t& cr); // �������캯��
		lib_send_data_t& operator=( const lib_send_data_t& cr); //  ��ֵ����
	};

	//////////////////////////////////////////////////////////////////////////
	//exmaple:ʹ�÷������ݶ˵�����
	//���͸��ͻ��˵�����
// 	class lib_send_data_cli_t : public lib_send_data_t<lib_proto_head_t>{
// 	public:
// 		lib_send_data_cli_t()
// 			: lib_send_data_t<lib_proto_head_t>(send_data){
// 		}
// 		virtual void set_head(lib_proto_head_t* head){
// 			const uint32_t all_len = this->write_pos;
// 			this->write_pos = 0;
// 
// 			*this<<head->magic
// 				<<all_len
// 				<<head->op
// 				<<head->param
// 				<<head->seq
// 				<<head->ret;
// 
// 			this->write_pos = all_len;
// 		}
// 	protected:
// 	private:
// 		static const uint32_t PACK_DEFAULT_SIZE = 819200;//ȥ����ͷ,������800K
// 		char send_data[PACK_DEFAULT_SIZE];
// 		lib_send_data_cli_t(const lib_send_data_cli_t& cr); // �������캯��
// 		lib_send_data_cli_t& operator=( const lib_send_data_cli_t& cr); // ��ֵ����
// 	};
	//////////////////////////////////////////////////////////////////////////
	//��������
	class lib_recv_data_t{
	public:
		const void* recv_data;
		uint32_t read_pos;
		lib_recv_data_t(const void* recvdata, int readpos){
			this->recv_data = recvdata;
			this->read_pos = readpos;
		}
		inline const void* data(){
			return this->recv_data;
		}

		//////////////////////////////////////////////////////////////////////////
		//�������
		template <typename T>
		inline lib_recv_data_t& operator>>(T& value){
			lib_packer_t::unpack(this->recv_data, value, this->read_pos);
			return *this;
		}

		inline void unpack_str(void* mstring, size_t length){
			lib_packer_t::unpack(this->recv_data, mstring, length, this->read_pos);
		}

	private:
		lib_recv_data_t(const lib_recv_data_t& cr);
		lib_recv_data_t& operator=( const lib_recv_data_t& cr);
	};
}//end namespace el  



