/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	任务框架
	brief:		ok
*********************************************************************/

#pragma once

namespace el_game{

	enum E_TASK_TYPE{
		E_TASK_TYPE_MAJOR		= 1,//主线任务
		E_TASK_TYPE_MINOR		= 2,//支线任务
		E_TASK_TYPE_DAY			= 3,//每日任务
	};

class lib_task_t{
public:
	lib_task_t(){}
	virtual ~lib_task_t(){}
protected:
	
private:
	lib_task_t(const lib_task_t& cr);
	lib_task_t& operator=(const lib_task_t& cr);
};

//任务管理器
class lib_task_mgr_t{
public:
	lib_task_mgr_t(){}
	virtual ~lib_task_mgr_t(){}
	virtual uint32_t open(){return 0;}
	virtual uint32_t drop(){return 0;}
	virtual uint32_t update(){return 0;}
protected:
	
private:
	lib_task_mgr_t(const lib_task_mgr_t& cr);
	lib_task_mgr_t& operator=(const lib_task_mgr_t& cr);
};

}

