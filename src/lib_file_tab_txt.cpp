#include "lib_file_tab_txt.h"
#include "lib_log.h"

int el::lib_file_tab_txt_t::load( const char* file_path )
{
	std::ifstream fin(file_path);
	if (!fin.good()){
		ERROR_LOG("");
		return ERR;
	}

	std::vector<std::string> str_vector;
	std::string s;

	while(std::getline(fin, s)){
		if (s.empty()){//去除空行
			continue;
		}
		if ('#' == s[0]){//去除注释
			continue;
		}
		str_vector.push_back(s);
	}
	this->parser(str_vector);
	fin.close();
	return SUCC;
}

int el::lib_file_tab_txt_t::parser( std::vector<std::string>& str_vector )
{
	std::string cur_section_name;//当前章节名称
	uint32_t row = 0;//行
	FOREACH(str_vector, it){
		std::string& str = *it;

		row++;
 		if (2 == row){
 			continue;
 		}
		std::vector<std::string> string_vector;
		el::g_cat_string(string_vector, str, '	');
		if (1 == row){
			uint32_t col_cnt = 0;//列
			FOREACH(string_vector, it_title){
				col_cnt++;
				std::string& str_title = *it_title;
				this->title_map[col_cnt] = str_title;
			}
			continue;
		}

		this->content_vector.push_back(string_vector);
	}
	return SUCC;
}
