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
	//以tab为分隔符的txt文件行
	/*
	id	name	desc	icon	
	物品id	名字	描述	iconid资源id	
	1	物品1	描述1	7,3,1	
	2	物品2	描述2	7,3,2	
	3	物品3	描述3	7,3,3	
	4	物品4	描述4	7,3,4	
	5	物品5	描述5	7,3,5	
	6	物品6	描述6	7,3,6	
	*/
	class lib_file_tab_txt_t
	{
	public:
		lib_file_tab_txt_t(){}
		virtual ~lib_file_tab_txt_t(){}
		int load(const char* file_path);
		std::map<uint32_t, std::string> title_map;//key:col列[1-n], val:字符串
		std::vector<std::vector<std::string>> content_vector;//内容 字符串信息 


		template <typename T>
		inline T get_val_def(const char* col_name, const std::vector<std::string>& content, T def){
			T ret;
			return SUCC == this->get_val(ret, col_name, content) ? ret : def;
		}
		//************************************
		// Brief:     获取指定的值
		// Returns:   int(0:成功.其它:失败)
		// Parameter: std::string & data(传出值)
		//************************************
		template <typename T>
		inline int get_val(T& data, const char* col_name, const std::vector<std::string>& content){
			uint32_t _col_ = 0;//要取得值的列数
			{
				uint32_t col_cnt = 0;

				//计算对应的列数
				FOREACH(this->title_map, it_title){
					col_cnt++;
					std::string& r = it_title->second;
					if (0 == r.compare(col_name)){
						_col_ = col_cnt;
						break;
					}
				}
			}
			if (0 == _col_){
				return ERR;
			}
			{
				uint32_t col_cnt = 0;
				FOREACH(content, it_item){
					col_cnt++;
					const std::string& r_item = *it_item;

					if (_col_ == col_cnt){
						if (r_item.empty()){
							return ERR;
						}
							
						el::convert_from_string(data, r_item);
						break;
					}
				}
			}
			

			return SUCC;
		}
	protected:

	private:
		
		//************************************
		// @brief:	解析文件
		//************************************
		int parser(std::vector<std::string>& str_vector);

		lib_file_tab_txt_t(const lib_file_tab_txt_t& cr);
		lib_file_tab_txt_t& operator=(const lib_file_tab_txt_t& cr);
	};
}//end namespace el
