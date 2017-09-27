/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	ok
	brief:		定时器函数，有秒级和微妙级两种精度的接口。	
				用于设定某一时刻调用某个函数	
*********************************************************************/

#pragma once

#include "lib_include.h"
#include "lib_list.h"
#include "lib_platform.h"

// 如果回调函数返回0，则表示要删除该定时器，反之，则不删除	
typedef int (*ON_TIMER_FUN)(void* owner, void* data);

namespace el{
	struct lib_timer_t{
	public:
		struct timeval now;
		struct tm now_tm;
		//************************************
		// @brief:	获取当前秒数(每次el_async循环时生成)	
		//************************************
		uint32_t now_sec(){
			return (uint32_t)this->now.tv_sec;
		}
		//************************************
		// @brief:	获取当前微秒数(每次el_async循环时生成)	
		// return:	uint32_t	
		//************************************
		uint32_t now_usec(){
			return (uint32_t)this->now.tv_usec;
		}

		//************************************
		// @brief:	扫描定时器列表，调用到期了的定时器的回调函数，ON_TIMER_FUN	
		// 必须定期调用该函数才能调用到期了的定时器的回调函数	
		// 注意:调用前一定要调用过renew_now()	
		//************************************
		void handle_timer();
		//秒级定时器	
		struct lib_timer_sec_t{
			//联系定时器转轮	
			list_head	entry;
			//联系定时器的拥有者	
			list_head	sprite_list;
			//定时器的触发时刻，亦即调用回调函数的时间	
			time_t				expire;
			void*				owner;
			void*				data;
			ON_TIMER_FUN		function;
			void init();
		};
		//微秒级定时器	
		struct lib_timer_micro_t{
			list_head	entry;
			//定时器的触发时刻，亦即调用回调函数的时间	
			timeval		tv;
			void*				owner;
			void*				data;
			ON_TIMER_FUN		function;
			void init();
		};
		//定时器添加模式	
		enum E_TIMER_CHG_MODE {
			//添加一个新的定时器	
			E_TIMER_ADD_NEW_TIMER = 1,
			//替换一个定时器。如果找不到符合条件的定时器，则创建一个新的定时器	
			E_TIMER_REPLACE_TIMER = 2,
		};
	public:
		lib_timer_t();
		virtual ~lib_timer_t();

		//************************************
		// @brief:	添加/替换一个秒级定时器，该定时器的到期时间是expire，到期时回调的函数是func	
		// return:	lib_timer_sec_t* 指向新添加/替换的秒级定时器的指针	
		// parameter:	list_head_t * head 链头，新创建的定时器会被插入到该链表中	
		// parameter:	ON_TIMER_FUN func 定时器到期时调用的回调函数	
		// parameter:	void * owner 传递给回调函数的第一个参数	
		// parameter:	void * data 传递给回调函数的第二个参数	
		// parameter:	time_t expire 定时器到期时间（从Epoch开始的秒数）	
		// parameter:	E_TIMER_CHG_MODE flag 指示add_sec_event添加/替换定时器	
		// 如果flag==E_TIMER_REPLACE_TIMER，那么add_sec_event将在head链表中	
		// 搜索出第一个回调函数==func的定时器，然后把这个定时器的到期时间	
		// 修改成expire。如果找不到符合条件的定时器，则新建一个定时器	
		// 建议:只有当head链表中所有定时器的回调函数都各不相同的情况下，	
		// 才使用E_TIMER_REPLACE_TIMER	
		// 注意：绝对不能在定时器的回调函数中修改该定时器的到期时间！	
		//************************************
		lib_timer_sec_t* add_sec_event(list_head_t* head, ON_TIMER_FUN func,
			void* owner, void* data, time_t expire, E_TIMER_CHG_MODE flag);
		//************************************
		// @brief:	删除指定的秒级定时器	
		// parameter:	lib_timer_sec_t * t	
		//************************************
		void remove_sec_timer(lib_timer_sec_t* t);
		//************************************
		// @brief:	删除链表head中所有的秒级定时器	
		// parameter:	list_head_t * head 定时器链表的链头	
		//************************************
		void remove_sec_timers(list_head_t* head);
		//************************************
		// @brief:	添加一个微秒级定时器	
		// return:	lib_timer_micro_t* 指向新添加的微秒级定时器的指针	
		// parameter:	ON_TIMER_FUN func 回调的函数	
		// parameter:	const timeval * tv 定时器到期时间	
		// parameter:	void * owner 传递给回调函数的第一个参数	
		// parameter:	void * data 传递给回调函数的第二个参数	
		//************************************
		lib_timer_micro_t* add_micro_event(ON_TIMER_FUN func, 
			const timeval* tv, void* owner, void* data);
		void remove_micro_timer(lib_timer_micro_t *t, int freed = 1);
		//************************************
		// @brief:	删除传递给回调函数的第一个参数==owner的所有微秒级定时器	
		//************************************
		void remove_micro_timers(void* owner);
		//************************************
		// @brief:	更新当前时间	
		// return:	void	
		//************************************
		inline void renew_now(){
			gettimeofday(&this->now, 0);
 			time_t tt = this->now.tv_sec;
 			localtime_r(&tt, &this->now_tm);
		}

	protected:
	private:
		//************************************
		// @brief:	初始化定时器功能。必须调用了这个函数，才能使用定时器功能	
		//************************************
		void setup_timer();
		//************************************
		// @brief:	 销毁所有定时器（包括秒级和微秒级的定时器），并释放内存	
		//************************************
		void destroy_timer();
		//************************************
		// @brief:	修改秒级定时器tmr的到期时间	
		// parameter:	lib_timer_sec_t * tmr 需要修改到期时间的定时器	
		// parameter:	time_t exptm 将tmr的到期时间修改成exptm（从Epoch开始的秒数）	
		// 注意：绝对不能在定时器的回调函数中修改该定时器的到期时间！	
		//************************************
		void mod_expire_time(lib_timer_sec_t* tmr, time_t exptm);
		enum {
			E_TIMER_VEC_SIZE = 5,
		};
		struct tvec_root_s {
			list_head head;
			int     expire;
			//最小到期时间	
			time_t  min_expiring_time;
			tvec_root_s();
		};
		time_t last_renew_sec;
		tvec_root_s  vec[E_TIMER_VEC_SIZE];
		list_head    micro_timer;//毫秒是只有一个链表.每次全部检索	


		inline int find_min_idx(time_t diff, int max_idx){
			while (max_idx && (this->vec[max_idx - 1].expire >= diff)) {
				--max_idx;
			}
			return max_idx;
		}
		//************************************
		// @brief:	更新秒级定时器	
		//************************************
		void renew_sec_timer_list(const int idx);
		//************************************
		// @brief:	设置定时轮中一个转轮中到期最小时间	
		// return:	void	
		// parameter:	time_t exptm 到期时间	
		// parameter:	int idx 定时轮中的序号	
		//************************************
		inline void set_min_exptm(time_t exptm, int idx){
			if ((exptm < this->vec[idx].min_expiring_time) 
				|| (0 == this->vec[idx].min_expiring_time)) {
				this->vec[idx].min_expiring_time = exptm;
			}
		}
		//************************************
		// @brief:	根据到期时间找时间轮的序号	
		// return:	int 时间轮序号	
		//************************************
		inline int find_root_idx(time_t expire_time){
			int idx = 0;
			time_t diff = expire_time - this->now.tv_sec;
			for (; idx != (E_TIMER_VEC_SIZE - 1); ++idx) {
				if (diff <= this->vec[idx].expire){
					break;
				}
			}
			return idx;
		}
		//************************************
		// @brief:	增加一个定时	
		//************************************
		inline void add_timer(lib_timer_sec_t *t){
			int idx = this->find_root_idx(t->expire);
			list_add_tail(&t->entry, &this->vec[idx].head);
			this->set_min_exptm(t->expire, idx);
		}
		//************************************
		// @brief:	扫描毫秒定时器链表(逐个)	
		//************************************
		void scan_micro_timer();
		//************************************
		// @brief:	找回调函数相同的定时事件	
		// 搜索出第一个回调函数==func的定时器	
		//************************************
		inline lib_timer_sec_t* find_event(list_head_t* head, ON_TIMER_FUN function){
			lib_timer_sec_t* t;
			list_for_each_entry_type(t, lib_timer_sec_t, head, sprite_list) {
				if (t->function == function){
					return t;
				}
			}
			return NULL;
		}
		//************************************
		// @brief:	扫描秒级定时器离链表(转轮序号0)	
		//************************************
		void scan_sec_timer();

		lib_timer_t(const lib_timer_t& cr);
		lib_timer_t& operator=(const lib_timer_t& cr);
	};
}

extern el::lib_timer_t* g_timer;

namespace el{
	//使用定时器的基类	
	class lib_timer_base_t
	{
	public:
		list_head_t timer_list;
		lib_timer_base_t(el::lib_timer_t* timer);
		virtual ~lib_timer_base_t();
		//************************************
		// @brief:	创建一个新的秒级定时器，传递给回调函数的第一个参数是this，第二个参数是data	
		// return:	lib_timer_sec_t* 指向新创建的秒级定时器的指针	
		// parameter:	ON_TIMER_FUN func 回调函数	
		// parameter:	void * data 第二个参数	
		// parameter:	time_t expire 该定时器的到期时间是expire（从Epoch开始的秒数）	
		//************************************
		lib_timer_t::lib_timer_sec_t* add_sec_event(ON_TIMER_FUN func, void* data, time_t expire,
													el::lib_timer_t::E_TIMER_CHG_MODE timer_mode = el::lib_timer_t::E_TIMER_ADD_NEW_TIMER);
		//************************************
		// @brief:	删除秒级定时器timer_sec	
		//************************************
		void remove_sec_timer(lib_timer_t::lib_timer_sec_t* timer_sec);
		//************************************
		// @brief:	创建一个新的微妙级定时器，传递给回调函数的第一个参数是this，第二个参数是data	
		// return:	lib_timer_t::lib_timer_micro_t* 指向新创建的微妙级定时器的指针	
		// parameter:	ON_TIMER_FUN func 回调函数	
		// parameter:	void * data 第二个参数	
		// parameter:	timeval * expire 该定时器的到期时间是expire	
		//************************************
		lib_timer_t::lib_timer_micro_t* add_micro_event(ON_TIMER_FUN func, void* data, timeval* expire);
		//************************************
		// @brief:	删除微秒级定时器timer_micro	
		//************************************
		void remove_micro_timer(lib_timer_t::lib_timer_micro_t* timer_micro);
		//************************************
		// @brief:	 删除所有秒级定时器 && 删除所有微秒级定时器	
		//************************************
		void remove_timers();
	private:
		el::lib_timer_t* timer;
	};
}

