/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	OK
	brief:		��ѯ	
*********************************************************************/

#pragma once

#include "lib_include.h"
#include "lib_proto/lib_msg.h"
#include "lib_log.h"
#include "lib_util.h"
#include "lib_proto/lib_proto.h"

namespace el{

class lib_query_t;
/**
 * @brief ��װ���ڲ�ѯ�����е���ɾ�Ͳ��Ҳ�������Ϊ��Ƶ��ʹ�õ�	
 */
class lib_querys_t
{
public:
	typedef std::map<uint32_t, lib_query_t*> QUERY_MAP;//key:seq, val: lib_query_t*
public:
	//����0:ʧ��
    uint32_t add_query(lib_query_t* qr);
    void del_query(uint32_t seq);
    lib_query_t* find_query(uint32_t seq);
	//����g_querys�е�����/������ϵ�����	
	void del_all_querys();// todo [8/7/2015 a]
	lib_querys_t();
    virtual ~lib_querys_t();
protected:
	QUERY_MAP querys;
	lib_seq_gen_t seq_gen;
private:
	lib_querys_t(const lib_querys_t& cr);
	lib_querys_t& operator=(const lib_querys_t& cr);
};

//ʹ�ò�ѯ�ĸ���	
//��ѯӵ����(�����Ĳ�ѯ���ڴ�������洢/����)	
//�����lib_query_t ʹ��deltet ������	
class lib_query_owner_t
{
public:
	int add_query(uint32_t seq, lib_query_t* qr);
	bool del_query(uint32_t seq);
	void del_all_query(el::lib_querys_t* query_mgr);
	lib_query_t* find_query(uint32_t seq);
	bool is_empty();
	lib_query_owner_t(lib_querys_t* qmgr);
	virtual ~lib_query_owner_t();
protected:
	lib_querys_t::QUERY_MAP querys;
	lib_querys_t* query_mgr;
private:
	lib_query_owner_t(const lib_query_owner_t& cr);
	lib_query_owner_t& operator=(const lib_query_owner_t& cr);
};

class lib_query_cb_handle_base_t
{
public:
	lib_query_cb_handle_base_t();
	virtual ~lib_query_cb_handle_base_t();
	virtual int exec(lib_query_t* q, el::lib_proto_head_base_t* head, lib_msg_t* c_out) = 0;
private:
	lib_query_cb_handle_base_t(const lib_query_cb_handle_base_t& cr);
	lib_query_cb_handle_base_t& operator=(const lib_query_cb_handle_base_t& cr);
};

//�ص�	
template<typename T_CB_CLASS>
class lib_query_cb_handle_t : public lib_query_cb_handle_base_t
{
public:
    typedef int (T_CB_CLASS::*Method)(el::lib_proto_head_base_t* head, lib_msg_t* c_out);
public:
	lib_query_cb_handle_t( Method m )
	{
		this->cb_fn = m;
	}

	int exec( lib_query_t* q, el::lib_proto_head_base_t* head, lib_msg_t* c_out )
	{
		T_CB_CLASS* owner = (T_CB_CLASS*)q;
		return (owner->*(this->cb_fn))(head, c_out);
	}
	virtual ~lib_query_cb_handle_t()
	{

	}
private:
    Method cb_fn;
	lib_query_cb_handle_t(const lib_query_cb_handle_t& cr);
	lib_query_cb_handle_t& operator=(const lib_query_cb_handle_t& cr);
};


//��ѯ�жϰ�(����0 != ret ��ʱ����ж�)	
/**
 * @brief һ�������������������Ĳ�ѯ�������Զ���װ��Query�С�	
 *        ����Ĳ�ѯ������Ҫ�����������ķ������ݵġ�	
 *        �����Query������Ǵ������߼��ϵ�һ����ѯ������	
 *        ���磬���ߵ�ʱ�������ݿ��ȡ�û���Ϣ���ò�ѯ�����˶�����ݿ��ѯЭ�顣	
 *        ʹ��delete ɾ�� new �����Ļص�����	
 */
class lib_query_t
{
public: 
	/**
     * @brief ���캯��	
     * @param querys �����ѯ����	
     * @param query_id ����ѯ�˵�id	
     */
	lib_query_t(lib_querys_t* query_mgr, lib_query_owner_t* query_owner, uint32_t peer_id);
    virtual ~lib_query_t();

	 /**
     * @brief ִ�����еĲ�ѯЭ��	
     */
    virtual void excute() = 0; 
	   
	/**
     * @brief �����ж��Ƿ����еĲ�ѯ���Ѿ�������,��Query�������	
     * @return true,���;false,δ���		
     */
    virtual bool is_finish();

	/**
     * @brief ִ�й�������������᲻����ֹ	
     * @return 
     */
    virtual bool is_block();

	//ÿ�η�����Ϣ�������	
	virtual void on_update(uint32_t seq, uint32_t ret);
    /**
     * @brief Query��Ҫ��ֹ����ֹ��ʱ�򱻵��ã�����ҵ����Ҫ����ʵ��	
     * @param ret 0��ʾ�������������еĲ�ѯЭ�鶼��ȷ�Ļ�ȡ�������ݣ���0����ʾ��;����Ĵ�����	
     */
    virtual void on_finish(uint32_t ret) = 0;

    /**
     * @brief ���������������Ͳ�ѯЭ��ʱ�����ã�	
     *        ������һ��ȫ��Ψһ�����кź�����Э���	
     * @return ȫ��Ψһ�����к�,���������ݷ���ʱ�ܹ���ȷ��	
     *         �ҵ���Ӧ��Query����,����0:ʧ��	
     */
    uint32_t do_send(el::lib_querys_t& query_mgr);

    lib_query_owner_t* get_owner();
    uint32_t get_peer_id();

    /**
     * @brief ÿ�εõ���ѯЭ�鷵��ʱ������,��Ӧ�����кŴ�ϵͳ��ɾ��	
     * @param seq �ò�ѯЭ���Ӧ�����к�	
     * @param ret ��ѯЭ�鷵�صĴ����롣0��ʾ����;	
     *            ��0��ʾ��ѯ������Query����ֹ��	
     * @return -1��ʾ��ǰQuery������û���ҵ���Ӧ��seq;0��ɾ���ɹ���	
     */
	int update(el::lib_querys_t& query_mgr, uint32_t seq, uint32_t ret);

    template<typename T_CB_CLASS>
    void registe_cb(uint32_t seq, int (T_CB_CLASS::*Method)(el::lib_proto_head_base_t* head, lib_msg_t* c_out))
    {
        lib_query_cb_handle_base_t* b = new lib_query_cb_handle_t<T_CB_CLASS>(Method);
        if (!this->call_back_fn.insert(std::make_pair(seq, b)).second) {
			SAFE_DELETE(b);
			ERROR_LOG("[seq:%u]", seq);
        }
    }
	//ɾ��ָ����ŵĻص�����,���ͷ��ڴ�	
	void del_cb(uint32_t seq);

	//�����ж�head->ret �Ƿ�Ϸ�.(0:����, ��0:c_out==NULL)	
    int call_cb_handle(uint32_t seq, el::lib_proto_head_base_t* head, el::lib_msg_t* c_out);
private:
    typedef std::map<uint32_t, lib_query_cb_handle_base_t*> CbMap;
    CbMap call_back_fn;
    /**
     * @brief ����ѯ��ID	
     */
    uint32_t peer_id; 
	/**
     * @brief �����������	
     */
    lib_query_owner_t* query_owner;
	lib_querys_t* query_mgr;
	lib_query_t(const lib_query_t& cr);
	lib_query_t& operator=(const lib_query_t& cr);
};

//��ѯ���жϰ�(����0 != ret ��ʱ��Ҳ�����ж�)	
class lib_query_noblock_t : public lib_query_t
{
public:
    lib_query_noblock_t(lib_querys_t* query_mrg, lib_query_owner_t* owner, uint32_t respondent) : lib_query_t(query_mrg, owner, respondent) {
    }
    /**
     * @brief ִ�й�������������᲻����ֹ	
     * @return 
     */
    virtual bool is_block();
    virtual ~lib_query_noblock_t();
};

}


