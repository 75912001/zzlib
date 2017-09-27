#include <lib_include.h>
#include "lib_query.h"

#include "lib_err_code.h"

bool el::lib_query_t::is_finish()
{
	return this->query_owner->is_empty();
}

bool el::lib_query_t::is_block()
{
	return true;
}

void el::lib_query_t::on_update( uint32_t seq, uint32_t ret )
{

}

el::lib_query_t::lib_query_t(lib_querys_t* query_mgr, lib_query_owner_t* query_owner, uint32_t peer_id )
{
	this->query_owner = query_owner;
	this->peer_id = peer_id;
	this->query_mgr = query_mgr;
}

el::lib_query_t::~lib_query_t()
{
	this->query_owner->del_all_query(this->query_mgr);
	FOREACH(call_back_fn, it){
		SAFE_DELETE(it->second);
	}
}

uint32_t el::lib_query_t::do_send(el::lib_querys_t& query_mgr)
{
	uint32_t query_seq = query_mgr.add_query(this);
	if (0 == query_seq){
		return 0;
	}

	this->query_owner->add_query(query_seq, this);
	return query_seq;
}

el::lib_query_owner_t* el::lib_query_t::get_owner()
{
	return this->query_owner;
}

uint32_t el::lib_query_t::get_peer_id()
{
	return this->peer_id;
}

int el::lib_query_t::update( el::lib_querys_t& query_mgr, uint32_t seq, uint32_t ret )
{
	if (this->query_owner->del_query(seq)){
		query_mgr.del_query(seq);
		this->on_update(seq, ret);
	} else {
		ERROR_LOG("can not find this query [seq:%u]", seq);
		return ERR;
	}

	if (0 != ret) {
		if (this->is_block()) {
			this->on_finish(ret);
			delete this;
			return SUCC;
		}
	}

	if (this->is_finish()) {
		this->on_finish(ret);
		delete this;
	}
	return SUCC;
}

int el::lib_query_t::call_cb_handle( uint32_t seq, el::lib_proto_head_base_t* head, el::lib_msg_t* c_out )
{
	CbMap::iterator it = this->call_back_fn.find(seq);
	if (it != this->call_back_fn.end()) {
		return it->second->exec(this, head, c_out);
	}
	return SUCC;
}

void el::lib_query_t::del_cb( uint32_t seq )
{
	auto it = this->call_back_fn.find(seq);
	if (this->call_back_fn.end() != it){
		SAFE_DELETE(it->second);
		this->call_back_fn.erase(seq);
	}
}

bool el::lib_query_noblock_t::is_block()
{
	return false;
}

el::lib_query_noblock_t::~lib_query_noblock_t()
{

}

uint32_t el::lib_querys_t::add_query( lib_query_t* qr )
{
	uint32_t seq = this->seq_gen.gen();
	TRACE_LOG("[seq:%u]", seq);
	if (!this->querys.insert(std::make_pair(seq, qr)).second) {
		ERROR_LOG("add query err [seq:%u]", seq);
		return 0;
	}
	return seq;
}

void el::lib_querys_t::del_query( uint32_t seq )
{
	this->seq_gen.del(seq);
	if (0 == this->querys.erase(seq)) {
		ERROR_LOG("del query fail [seq:%u]", seq);
	}
}

el::lib_query_t* el::lib_querys_t::find_query( uint32_t seq )
{
	auto it = this->querys.find(seq);
	return it == this->querys.end() ? NULL : it->second;
}

el::lib_querys_t::~lib_querys_t()
{

}

el::lib_querys_t::lib_querys_t() :seq_gen(UINT_MAX)
{

}

void el::lib_querys_t::del_all_querys()
{
	while (!this->querys.empty()){
		auto it = this->querys.begin();
		lib_query_t* p = it->second;
		p->update(*this, it->first, ERR_SYS::DISCONNECT);
	}
}

int el::lib_query_owner_t::add_query( uint32_t seq, lib_query_t* qr )
{
	if (!this->querys.insert(std::make_pair(seq, qr)).second) {
		ERROR_LOG("add query err [seq:%u]", seq);
		return ERR;
	}
	return SUCC;
}

bool el::lib_query_owner_t::del_query( uint32_t seq )
{
	if (0 == this->querys.erase(seq)) {
		ERROR_LOG("del query fail [seq:%u]", seq);
		return false;
	}
	return true;
}

void el::lib_query_owner_t::del_all_query(el::lib_querys_t* query_mgr)
{
	FOREACH(this->querys, it){
		query_mgr->del_query(it->first);
	}
	this->querys.clear();
}

el::lib_query_t* el::lib_query_owner_t::find_query( uint32_t seq )
{
	auto it = this->querys.find(seq);
	return it == this->querys.end() ? NULL : it->second;
}

bool el::lib_query_owner_t::is_empty()
{
	return this->querys.empty();
}

el::lib_query_owner_t::lib_query_owner_t(lib_querys_t* qmgr)
{
	this->query_mgr = qmgr;
}

el::lib_query_owner_t::~lib_query_owner_t()
{
	this->del_all_query(this->query_mgr);
}

el::lib_query_cb_handle_base_t::lib_query_cb_handle_base_t()
{

}

el::lib_query_cb_handle_base_t::~lib_query_cb_handle_base_t()
{

}


