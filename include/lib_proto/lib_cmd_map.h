/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	ok
	brief:		命令消息存放
*********************************************************************/

#include "lib_include.h"

#pragma once

namespace el{

	typedef uint32_t CMD_ID;

	//命令数据
	template <typename T_FUNC> 
	struct lib_cmd_t{
		CMD_ID cmd_id;
		class lib_msg_t* msg_in;
		class lib_msg_t* msg_out;
		T_FUNC func;
		lib_cmd_t(){
			this->cmd_id = 0;
			this->msg_in = NULL;
			this->msg_out = NULL;
		}
	};

	//存放命令消息,与映射的命令数据
	template <typename T_FUNC> 
	class lib_cmd_map_t{
	public:
		lib_cmd_map_t(){}
		virtual ~lib_cmd_map_t(){
			FOREACH(this->cmd_map, it){
				lib_cmd_t<T_FUNC>& r = it->second;
				SAFE_DELETE(r.msg_in);
				SAFE_DELETE(r.msg_out);
			}
		}

		//msg_in&msg_out must be use new create;
		void insert(CMD_ID cmd_id, T_FUNC func, class lib_msg_t* msg_in, class lib_msg_t* msg_out){
			assert(this->cmd_map.end() == this->cmd_map.find(cmd_id));
			lib_cmd_t<T_FUNC> item;
			item.cmd_id = cmd_id;
			item.msg_in = msg_in;
			item.msg_out = msg_out;
			item.func = func;
			this->cmd_map[cmd_id] = item;
		}

		inline lib_cmd_t<T_FUNC>* find(CMD_ID cmd_id){
			auto it = this->cmd_map.find(cmd_id);
			if (this->cmd_map.end() == it){
				return NULL;
			}
			return &it->second;
		}
	private:
		std::map<CMD_ID, lib_cmd_t<T_FUNC> > cmd_map;//key:cmd_id, val:lib_cmd_t<T_FUNC>
		lib_cmd_map_t(const lib_cmd_map_t& cr);
		lib_cmd_map_t& operator=(const lib_cmd_map_t& cr);
	};
}//end namespace el
