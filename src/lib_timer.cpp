#include <lib_include.h>
#include "lib_timer.h"

el::lib_timer_t* g_timer = NULL;

//4,8,16,32,64
#define GEN_TIME_INTERVAL(__idx__) (1 << ((__idx__) + 2))

void el::lib_timer_t::setup_timer()
{
	this->renew_now();
	for (int i = 0; i < E_TIMER_VEC_SIZE; i++) {
		this->vec[i].expire = GEN_TIME_INTERVAL(i);
	}
	INIT_LIST_HEAD(&this->micro_timer);
}

void el::lib_timer_t::destroy_timer()
{
	list_head_t* l;
	list_head_t* p;

	for (int i = 0; i < E_TIMER_VEC_SIZE; i++) {
		list_for_each_safe(l, p, &this->vec[i].head) {
			lib_timer_sec_t* t = list_entry(l, lib_timer_sec_t, entry);
			this->remove_sec_timer(t);
		}	
	}

	list_for_each_safe(l, p, &this->micro_timer) {
		lib_timer_micro_t* t = list_entry(l, lib_timer_micro_t, entry);
		this->remove_micro_timer(t);
	}
}

void el::lib_timer_t::handle_timer()
{
	//second timer
	if (this->last_renew_sec != (int32_t)this->now_sec()) {
		this->last_renew_sec = this->now_sec();
		this->scan_sec_timer();
	}
	//microseconds timer
	this->scan_micro_timer();
}

el::lib_timer_t::lib_timer_sec_t* el::lib_timer_t::add_sec_event( list_head_t* head, ON_TIMER_FUN func, void* owner, void* data, time_t expire, E_TIMER_CHG_MODE flag )
{
	lib_timer_sec_t* timer;
	if (E_TIMER_REPLACE_TIMER == flag) {
		timer = this->find_event(head, func);
		if (NULL == timer){
			goto new_timer;
		}
		this->mod_expire_time(timer, expire);
		return timer;
	}
new_timer:
	timer = (lib_timer_sec_t*)::malloc(sizeof(lib_timer_sec_t));

	timer->init();
	timer->function  = func;
	timer->expire    = expire;
	timer->owner     = owner;
	timer->data      = data;

	list_add_tail(&timer->sprite_list, head);
	this->add_timer(timer);
	return timer;
}

void el::lib_timer_t::mod_expire_time( lib_timer_sec_t* t, time_t expiretime )
{
	t->expire = expiretime;

	int idx = this->find_root_idx(expiretime);

	list_del_init(&t->entry);
	list_add_tail(&t->entry, &this->vec[idx].head);
	this->set_min_exptm(t->expire, idx);
}

void el::lib_timer_t::remove_sec_timer( lib_timer_sec_t* t)
{
	if (0 != t->sprite_list.next) {
		list_del(&t->sprite_list);
	}
	list_del(&t->entry);
	free(t);
}

void el::lib_timer_t::remove_sec_timers( list_head_t* head )
{
	lib_timer_sec_t *t;
	list_head_t *l, *m;

	list_for_each_safe (l, m, head) {
		t = list_entry (l, lib_timer_sec_t, sprite_list);
		this->remove_sec_timer(t);
	}
}

el::lib_timer_t::lib_timer_micro_t* el::lib_timer_t::add_micro_event( ON_TIMER_FUN func, const struct timeval* tv, void* owner, void* data )
{
	lib_timer_micro_t* timer = (lib_timer_micro_t*)::malloc(sizeof(lib_timer_micro_t));

	timer->init();
	timer->function  = func;
	timer->tv        = *tv;
	timer->owner     = owner;
	timer->data      = data;

	list_add_tail(&timer->entry, &this->micro_timer);
	return timer;
}

void el::lib_timer_t::remove_micro_timer( lib_timer_micro_t *t, int freed /*= 1*/ )
{
	list_del_init(&t->entry);
	free(t);
}

void el::lib_timer_t::remove_micro_timers( void* owner )
{
	list_head_t *l, *p;
	lib_timer_micro_t* t;

	list_for_each_safe(l, p, &this->micro_timer) {
		t = list_entry(l, lib_timer_micro_t, entry);
		if (t->owner == owner) {
			this->remove_micro_timer(t, 0);
		}
	}
}

void el::lib_timer_t::renew_sec_timer_list( const int idx )
{
	time_t min_exptm = 0;
	list_head_t* cur;
	list_head_t* next;

	list_for_each_safe(cur, next, &this->vec[idx].head) {
		lib_timer_sec_t* t = list_entry(cur, lib_timer_sec_t, entry);
		int i = this->find_min_idx(t->expire - this->now_sec(), idx);
		if (i != idx) {
			list_del(&t->entry);
			list_add_tail(&t->entry, &this->vec[i].head);
			this->set_min_exptm(t->expire, i);
		} else if ((t->expire < min_exptm) || 0 == min_exptm) {
			min_exptm = t->expire;
		}
	}

	this->vec[idx].min_expiring_time = min_exptm;
}

void el::lib_timer_t::scan_micro_timer()
{
	list_head_t *l, *p;
	lib_timer_micro_t* t;

	list_for_each_safe(l, p, &this->micro_timer) {
		t = list_entry(l, lib_timer_micro_t, entry);
		if (NULL == (t->function)) {
			this->remove_micro_timer(t);
		} else if ((long)this->now_sec() > t->tv.tv_sec 
			|| ((long)this->now_sec() == t->tv.tv_sec && (long)this->now_usec() > t->tv.tv_usec)) {
			if (0 == t->function(t->owner, t->data)) {
				this->remove_micro_timer(t);
			}
		}
	}
}

void el::lib_timer_t::scan_sec_timer()
{
	list_head_t *l, *p;
	lib_timer_sec_t* t;

	list_for_each_safe(l, p, &this->vec[0].head) {
		t = list_entry(l, lib_timer_sec_t, entry);
		if (t->expire <= (int32_t)this->now_sec()) {
			if (0 == t->function(t->owner, t->data)) {
				this->remove_sec_timer(t);
			}
		}
	}

	for (int i = 1; i != E_TIMER_VEC_SIZE; ++i) {
		if ((this->vec[i].min_expiring_time - this->now_sec()) < (uint32_t)GEN_TIME_INTERVAL(i)) {
			this->renew_sec_timer_list(i);
		}
	}
}

el::lib_timer_t::lib_timer_t()
{
	this->last_renew_sec = 0;
	this->setup_timer();
}

el::lib_timer_t::~lib_timer_t()
{
	this->destroy_timer();
}

el::lib_timer_base_t::~lib_timer_base_t()
{
	this->remove_timers();
}

el::lib_timer_base_t::lib_timer_base_t( el::lib_timer_t* timer )
{
	this->timer = timer;
	INIT_LIST_HEAD(&this->timer_list);
}

el::lib_timer_t::lib_timer_sec_t* el::lib_timer_base_t::add_sec_event( ON_TIMER_FUN func, void* data, time_t expire ,el::lib_timer_t::E_TIMER_CHG_MODE timer_mode)
{
	return this->timer->add_sec_event(&this->timer_list, func, this, data, expire, timer_mode);
}

void el::lib_timer_base_t::remove_timers()
{
	this->timer->remove_sec_timers(&(this->timer_list));
	this->timer->remove_micro_timers(this);
}

void el::lib_timer_base_t::remove_micro_timer( lib_timer_t::lib_timer_micro_t* timer_micro )
{
	this->timer->remove_micro_timer(timer_micro, 0);
}

void el::lib_timer_base_t::remove_sec_timer( lib_timer_t::lib_timer_sec_t* timer_sec )
{
	this->timer->remove_sec_timer(timer_sec);
}

el::lib_timer_t::lib_timer_micro_t* el::lib_timer_base_t::add_micro_event( ON_TIMER_FUN func, void* data, timeval* expire )
{
	return this->timer->add_micro_event(func, expire, this, data);
}

void el::lib_timer_t::lib_timer_sec_t::init()
{
	INIT_LIST_HEAD(&this->sprite_list);
	INIT_LIST_HEAD(&this->entry);
	this->expire = 0;
	this->owner = NULL;
	this->data = NULL;
	this->function = NULL;
}

void el::lib_timer_t::lib_timer_micro_t::init()
{
	INIT_LIST_HEAD(&this->entry);
	this->tv.tv_sec = 0;
	this->tv.tv_usec = 0;
	this->owner = NULL;
	this->data = NULL;
	this->function = NULL;
}

el::lib_timer_t::tvec_root_s::tvec_root_s()
{
	INIT_LIST_HEAD(&head);
	expire = 0;
	min_expiring_time = 0;
}
