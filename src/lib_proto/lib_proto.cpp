#include "../../include/lib_proto/lib_proto.h"

uint32_t el::lib_example_recv_data_cli_t::get_len(){
	char* c = (char*)this->recv_data;
	uint32_t len_pos =  lib_example_proto_head_t::get_pkg_len_pos();
	c += len_pos;
	uint32_t proto_head_len = EL_BYTE_SWAP((uint32_t)(*(uint32_t*)c));
	TRACE_LOG("tt [len_pos:%u, proto_head_len:%u]", len_pos, proto_head_len);
	return (uint32_t)(proto_head_len + lib_example_proto_head_t::PROTO_HEAD_LEN);
}