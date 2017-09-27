/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	OK
	brief:		查询	
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
 * @brief 封装了在查询集合中的增删和查找操作，因为会频繁使用到	
 */
class lib_querys_t
{
public:
	typedef std::map<uint32_t, lib_query_t*> QUERY_MAP;//key:seq, val: lib_query_t*
public:
	//返回0:失败
    uint32_t add_query(lib_query_t* qr);
    void del_query(uint32_t seq);
    lib_query_t* find_query(uint32_t seq);
	//清理g_querys中的数据/清理堆上的数据	
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

//使用查询的父类	
//查询拥有者(创建的查询堆内存在这里存储/销毁)	
//加入的lib_query_t 使用deltet 来回收	
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

//回调	
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


//查询中断版(返回0 != ret 的时候会中断)	
/**
 * @brief 一个或多个对其他服务器的查询操作可以都封装在Query中。	
 *        这里的查询都是需要其他服务器的返回数据的。	
 *        这里的Query更多的是代表了逻辑上的一个查询操作。	
 *        例如，上线的时候向数据库获取用户信息，该查询包含了多个数据库查询协议。	
 *        使用delete 删除 new 出来的回调函数	
 */
class lib_query_t
{
public: 
	/**
     * @brief 构造函数	
     * @param querys 发起查询的人	
     * @param query_id 被查询人的id	
     */
	lib_query_t(lib_querys_t* query_mgr, lib_query_owner_t* query_owner, uint32_t peer_id);
    virtual ~lib_query_t();

	 /**
     * @brief 执行所有的查询协议	
     */
    virtual void excute() = 0; 
	   
	/**
     * @brief 用于判断是否所有的查询都已经返回了,即Query完成了吗	
     * @return true,完成;false,未完成		
     */
    virtual bool is_finish();

	/**
     * @brief 执行过程中遇到错误会不会中止	
     * @return 
     */
    virtual bool is_block();

	//每次返回消息都会调用	
	virtual void on_update(uint32_t seq, uint32_t ret);
    /**
     * @brief Query将要终止或中止的时候被调用，根据业务需要进行实现	
     * @param ret 0表示正常结束，所有的查询协议都正确的获取到了数据；非0，表示中途出错的错误码	
     */
    virtual void on_finish(uint32_t ret) = 0;

    /**
     * @brief 向其他服务器发送查询协议时被调用，	
     *        将产生一个全局唯一的序列号和这条协议绑定	
     * @return 全局唯一的序列号,用于在数据返回时能够正确的	
     *         找到对应的Query对象,返回0:失败	
     */
    uint32_t do_send(el::lib_querys_t& query_mgr);

    lib_query_owner_t* get_owner();
    uint32_t get_peer_id();

    /**
     * @brief 每次得到查询协议返回时被调用,对应的序列号从系统中删除	
     * @param seq 该查询协议对应的序列号	
     * @param ret 查询协议返回的错误码。0表示正常;	
     *            非0表示查询出错，则Query被中止；	
     * @return -1表示当前Query对象中没有找到对应的seq;0，删除成功。	
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
	//删除指定序号的回调函数,并释放内存	
	void del_cb(uint32_t seq);

	//首先判断head->ret 是否合法.(0:正常, 非0:c_out==NULL)	
    int call_cb_handle(uint32_t seq, el::lib_proto_head_base_t* head, el::lib_msg_t* c_out);
private:
    typedef std::map<uint32_t, lib_query_cb_handle_base_t*> CbMap;
    CbMap call_back_fn;
    /**
     * @brief 被查询的ID	
     */
    uint32_t peer_id; 
	/**
     * @brief 发起请求的人	
     */
    lib_query_owner_t* query_owner;
	lib_querys_t* query_mgr;
	lib_query_t(const lib_query_t& cr);
	lib_query_t& operator=(const lib_query_t& cr);
};

//查询不中断版(返回0 != ret 的时候也不会中断)	
class lib_query_noblock_t : public lib_query_t
{
public:
    lib_query_noblock_t(lib_querys_t* query_mrg, lib_query_owner_t* owner, uint32_t respondent) : lib_query_t(query_mrg, owner, respondent) {
    }
    /**
     * @brief 执行过程中遇到错误会不会中止	
     * @return 
     */
    virtual bool is_block();
    virtual ~lib_query_noblock_t();
};

}


