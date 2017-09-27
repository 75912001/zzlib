/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	
	brief:		ok
*********************************************************************/

#pragma once

#include "lib_include.h"

#include "lib_util.h"

namespace el{
	class lib_file_ini_t
	{
	public:
		lib_file_ini_t(){}
		virtual ~lib_file_ini_t(){}
		int load(const char* file_path);

		template <typename T>
		inline T get_val_def(const char* section, const char* name, T def){
			T ret;
			return SUCC == this->get_val(ret, section, name) ? ret : def;
		}
		//************************************
		// Brief:     获取指定组中key的值
		// Returns:   int(0:成功.其它:失败)
		// Parameter: std::string & data(传出值)
		//************************************
		template <typename T>
		inline int get_val(T& data, const char* section, const char* name){
			std::string val = this->get_strval(section, name);
			if (val.empty()){
				return ERR;
			} else {
				el::convert_from_string(data, val);
			}
			return SUCC;
		}
	protected:

	private:
		typedef std::map<std::string, std::string> NAME_MAP;//key:名字, val:值
		std::map<std::string, NAME_MAP> section_map;//key:节, val:名字

		//************************************
		// @brief:	解析文件
		//************************************
		int parser(std::vector<std::string>& str_vector);
		//************************************
		// @brief:	获取章节名
		//************************************
		std::string get_section_name(const std::string& str);
		/**
		 * @brief	从字符串中找出name和val
		 * @return	bool:true 成功 false:失败
		 */
		bool get_name_and_val(const std::string& str, std::string& name, std::string& val);
		std::string get_strval(const char* section, const char* name);
		lib_file_ini_t(const lib_file_ini_t& cr);
		lib_file_ini_t& operator=(const lib_file_ini_t& cr);
	};
}//end namespace el
