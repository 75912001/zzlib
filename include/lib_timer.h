/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	ok
	brief:		��ʱ�����������뼶��΢����־��ȵĽӿڡ�	
				�����趨ĳһʱ�̵���ĳ������	
*********************************************************************/

#pragma once

#include "lib_include.h"
#include "lib_list.h"
#include "lib_platform.h"

// ����ص���������0�����ʾҪɾ���ö�ʱ������֮����ɾ��	
typedef int (*ON_TIMER_FUN)(void* owner, void* data);

namespace el{
	struct lib_timer_t{
	public:
		struct timeval now;
		struct tm now_tm;
		//************************************
		// @brief:	��ȡ��ǰ����(ÿ��el_asyncѭ��ʱ����)	
		//************************************
		uint32_t now_sec(){
			return (uint32_t)this->now.tv_sec;
		}
		//************************************
		// @brief:	��ȡ��ǰ΢����(ÿ��el_asyncѭ��ʱ����)	
		// return:	uint32_t	
		//************************************
		uint32_t now_usec(){
			return (uint32_t)this->now.tv_usec;
		}

		//************************************
		// @brief:	ɨ�趨ʱ���б����õ����˵Ķ�ʱ���Ļص�������ON_TIMER_FUN	
		// ���붨�ڵ��øú������ܵ��õ����˵Ķ�ʱ���Ļص�����	
		// ע��:����ǰһ��Ҫ���ù�renew_now()	
		//************************************
		void handle_timer();
		//�뼶��ʱ��	
		struct lib_timer_sec_t{
			//��ϵ��ʱ��ת��	
			list_head	entry;
			//��ϵ��ʱ����ӵ����	
			list_head	sprite_list;
			//��ʱ���Ĵ���ʱ�̣��༴���ûص�������ʱ��	
			time_t				expire;
			void*				owner;
			void*				data;
			ON_TIMER_FUN		function;
			void init();
		};
		//΢�뼶��ʱ��	
		struct lib_timer_micro_t{
			list_head	entry;
			//��ʱ���Ĵ���ʱ�̣��༴���ûص�������ʱ��	
			timeval		tv;
			void*				owner;
			void*				data;
			ON_TIMER_FUN		function;
			void init();
		};
		//��ʱ�����ģʽ	
		enum E_TIMER_CHG_MODE {
			//���һ���µĶ�ʱ��	
			E_TIMER_ADD_NEW_TIMER = 1,
			//�滻һ����ʱ��������Ҳ������������Ķ�ʱ�����򴴽�һ���µĶ�ʱ��	
			E_TIMER_REPLACE_TIMER = 2,
		};
	public:
		lib_timer_t();
		virtual ~lib_timer_t();

		//************************************
		// @brief:	���/�滻һ���뼶��ʱ�����ö�ʱ���ĵ���ʱ����expire������ʱ�ص��ĺ�����func	
		// return:	lib_timer_sec_t* ָ�������/�滻���뼶��ʱ����ָ��	
		// parameter:	list_head_t * head ��ͷ���´����Ķ�ʱ���ᱻ���뵽��������	
		// parameter:	ON_TIMER_FUN func ��ʱ������ʱ���õĻص�����	
		// parameter:	void * owner ���ݸ��ص������ĵ�һ������	
		// parameter:	void * data ���ݸ��ص������ĵڶ�������	
		// parameter:	time_t expire ��ʱ������ʱ�䣨��Epoch��ʼ��������	
		// parameter:	E_TIMER_CHG_MODE flag ָʾadd_sec_event���/�滻��ʱ��	
		// ���flag==E_TIMER_REPLACE_TIMER����ôadd_sec_event����head������	
		// ��������һ���ص�����==func�Ķ�ʱ����Ȼ��������ʱ���ĵ���ʱ��	
		// �޸ĳ�expire������Ҳ������������Ķ�ʱ�������½�һ����ʱ��	
		// ����:ֻ�е�head���������ж�ʱ���Ļص�������������ͬ������£�	
		// ��ʹ��E_TIMER_REPLACE_TIMER	
		// ע�⣺���Բ����ڶ�ʱ���Ļص��������޸ĸö�ʱ���ĵ���ʱ�䣡	
		//************************************
		lib_timer_sec_t* add_sec_event(list_head_t* head, ON_TIMER_FUN func,
			void* owner, void* data, time_t expire, E_TIMER_CHG_MODE flag);
		//************************************
		// @brief:	ɾ��ָ�����뼶��ʱ��	
		// parameter:	lib_timer_sec_t * t	
		//************************************
		void remove_sec_timer(lib_timer_sec_t* t);
		//************************************
		// @brief:	ɾ������head�����е��뼶��ʱ��	
		// parameter:	list_head_t * head ��ʱ���������ͷ	
		//************************************
		void remove_sec_timers(list_head_t* head);
		//************************************
		// @brief:	���һ��΢�뼶��ʱ��	
		// return:	lib_timer_micro_t* ָ������ӵ�΢�뼶��ʱ����ָ��	
		// parameter:	ON_TIMER_FUN func �ص��ĺ���	
		// parameter:	const timeval * tv ��ʱ������ʱ��	
		// parameter:	void * owner ���ݸ��ص������ĵ�һ������	
		// parameter:	void * data ���ݸ��ص������ĵڶ�������	
		//************************************
		lib_timer_micro_t* add_micro_event(ON_TIMER_FUN func, 
			const timeval* tv, void* owner, void* data);
		void remove_micro_timer(lib_timer_micro_t *t, int freed = 1);
		//************************************
		// @brief:	ɾ�����ݸ��ص������ĵ�һ������==owner������΢�뼶��ʱ��	
		//************************************
		void remove_micro_timers(void* owner);
		//************************************
		// @brief:	���µ�ǰʱ��	
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
		// @brief:	��ʼ����ʱ�����ܡ�����������������������ʹ�ö�ʱ������	
		//************************************
		void setup_timer();
		//************************************
		// @brief:	 �������ж�ʱ���������뼶��΢�뼶�Ķ�ʱ���������ͷ��ڴ�	
		//************************************
		void destroy_timer();
		//************************************
		// @brief:	�޸��뼶��ʱ��tmr�ĵ���ʱ��	
		// parameter:	lib_timer_sec_t * tmr ��Ҫ�޸ĵ���ʱ��Ķ�ʱ��	
		// parameter:	time_t exptm ��tmr�ĵ���ʱ���޸ĳ�exptm����Epoch��ʼ��������	
		// ע�⣺���Բ����ڶ�ʱ���Ļص��������޸ĸö�ʱ���ĵ���ʱ�䣡	
		//************************************
		void mod_expire_time(lib_timer_sec_t* tmr, time_t exptm);
		enum {
			E_TIMER_VEC_SIZE = 5,
		};
		struct tvec_root_s {
			list_head head;
			int     expire;
			//��С����ʱ��	
			time_t  min_expiring_time;
			tvec_root_s();
		};
		time_t last_renew_sec;
		tvec_root_s  vec[E_TIMER_VEC_SIZE];
		list_head    micro_timer;//������ֻ��һ������.ÿ��ȫ������	


		inline int find_min_idx(time_t diff, int max_idx){
			while (max_idx && (this->vec[max_idx - 1].expire >= diff)) {
				--max_idx;
			}
			return max_idx;
		}
		//************************************
		// @brief:	�����뼶��ʱ��	
		//************************************
		void renew_sec_timer_list(const int idx);
		//************************************
		// @brief:	���ö�ʱ����һ��ת���е�����Сʱ��	
		// return:	void	
		// parameter:	time_t exptm ����ʱ��	
		// parameter:	int idx ��ʱ���е����	
		//************************************
		inline void set_min_exptm(time_t exptm, int idx){
			if ((exptm < this->vec[idx].min_expiring_time) 
				|| (0 == this->vec[idx].min_expiring_time)) {
				this->vec[idx].min_expiring_time = exptm;
			}
		}
		//************************************
		// @brief:	���ݵ���ʱ����ʱ���ֵ����	
		// return:	int ʱ�������	
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
		// @brief:	����һ����ʱ	
		//************************************
		inline void add_timer(lib_timer_sec_t *t){
			int idx = this->find_root_idx(t->expire);
			list_add_tail(&t->entry, &this->vec[idx].head);
			this->set_min_exptm(t->expire, idx);
		}
		//************************************
		// @brief:	ɨ����붨ʱ������(���)	
		//************************************
		void scan_micro_timer();
		//************************************
		// @brief:	�һص�������ͬ�Ķ�ʱ�¼�	
		// ��������һ���ص�����==func�Ķ�ʱ��	
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
		// @brief:	ɨ���뼶��ʱ��������(ת�����0)	
		//************************************
		void scan_sec_timer();

		lib_timer_t(const lib_timer_t& cr);
		lib_timer_t& operator=(const lib_timer_t& cr);
	};
}

extern el::lib_timer_t* g_timer;

namespace el{
	//ʹ�ö�ʱ���Ļ���	
	class lib_timer_base_t
	{
	public:
		list_head_t timer_list;
		lib_timer_base_t(el::lib_timer_t* timer);
		virtual ~lib_timer_base_t();
		//************************************
		// @brief:	����һ���µ��뼶��ʱ�������ݸ��ص������ĵ�һ��������this���ڶ���������data	
		// return:	lib_timer_sec_t* ָ���´������뼶��ʱ����ָ��	
		// parameter:	ON_TIMER_FUN func �ص�����	
		// parameter:	void * data �ڶ�������	
		// parameter:	time_t expire �ö�ʱ���ĵ���ʱ����expire����Epoch��ʼ��������	
		//************************************
		lib_timer_t::lib_timer_sec_t* add_sec_event(ON_TIMER_FUN func, void* data, time_t expire,
													el::lib_timer_t::E_TIMER_CHG_MODE timer_mode = el::lib_timer_t::E_TIMER_ADD_NEW_TIMER);
		//************************************
		// @brief:	ɾ���뼶��ʱ��timer_sec	
		//************************************
		void remove_sec_timer(lib_timer_t::lib_timer_sec_t* timer_sec);
		//************************************
		// @brief:	����һ���µ�΢���ʱ�������ݸ��ص������ĵ�һ��������this���ڶ���������data	
		// return:	lib_timer_t::lib_timer_micro_t* ָ���´�����΢���ʱ����ָ��	
		// parameter:	ON_TIMER_FUN func �ص�����	
		// parameter:	void * data �ڶ�������	
		// parameter:	timeval * expire �ö�ʱ���ĵ���ʱ����expire	
		//************************************
		lib_timer_t::lib_timer_micro_t* add_micro_event(ON_TIMER_FUN func, void* data, timeval* expire);
		//************************************
		// @brief:	ɾ��΢�뼶��ʱ��timer_micro	
		//************************************
		void remove_micro_timer(lib_timer_t::lib_timer_micro_t* timer_micro);
		//************************************
		// @brief:	 ɾ�������뼶��ʱ�� && ɾ������΢�뼶��ʱ��	
		//************************************
		void remove_timers();
	private:
		el::lib_timer_t* timer;
	};
}

