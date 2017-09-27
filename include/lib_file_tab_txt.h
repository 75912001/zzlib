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
	//��tabΪ�ָ�����txt�ļ���
	/*
	id	name	desc	icon	
	��Ʒid	����	����	iconid��Դid	
	1	��Ʒ1	����1	7,3,1	
	2	��Ʒ2	����2	7,3,2	
	3	��Ʒ3	����3	7,3,3	
	4	��Ʒ4	����4	7,3,4	
	5	��Ʒ5	����5	7,3,5	
	6	��Ʒ6	����6	7,3,6	
	*/
	class lib_file_tab_txt_t
	{
	public:
		lib_file_tab_txt_t(){}
		virtual ~lib_file_tab_txt_t(){}
		int load(const char* file_path);
		std::map<uint32_t, std::string> title_map;//key:col��[1-n], val:�ַ���
		std::vector<std::vector<std::string>> content_vector;//���� �ַ�����Ϣ 


		template <typename T>
		inline T get_val_def(const char* col_name, const std::vector<std::string>& content, T def){
			T ret;
			return SUCC == this->get_val(ret, col_name, content) ? ret : def;
		}
		//************************************
		// Brief:     ��ȡָ����ֵ
		// Returns:   int(0:�ɹ�.����:ʧ��)
		// Parameter: std::string & data(����ֵ)
		//************************************
		template <typename T>
		inline int get_val(T& data, const char* col_name, const std::vector<std::string>& content){
			uint32_t _col_ = 0;//Ҫȡ��ֵ������
			{
				uint32_t col_cnt = 0;

				//�����Ӧ������
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
		// @brief:	�����ļ�
		//************************************
		int parser(std::vector<std::string>& str_vector);

		lib_file_tab_txt_t(const lib_file_tab_txt_t& cr);
		lib_file_tab_txt_t& operator=(const lib_file_tab_txt_t& cr);
	};
}//end namespace el
