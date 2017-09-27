#include "lib_file_ini.h"

int el::lib_file_ini_t::load( const char* file_path )
{
	std::ifstream fin(file_path);
	std::vector<std::string> str_vector;
	std::string s;
	while(getline(fin, s)){
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

int el::lib_file_ini_t::parser( std::vector<std::string>& str_vector )
{
	std::string cur_section_name;//当前章节名称
	FOREACH(str_vector, it){
		std::string& str = *it;
		std::string section_name = this->get_section_name(str);
		if (!section_name.empty()){//章节
			cur_section_name = section_name;
		} else {//名字和值, ip=xxxxxx;
			std::string name;
			std::string val;
			if (this->get_name_and_val(str, name, val)){
				this->section_map[cur_section_name][name] = val;
			}
		}
	}
	return SUCC;
}

std::string el::lib_file_ini_t::get_section_name( const std::string& str )
{
	if ('[' == str[0] && ']' == str[str.size()-1]){
		std::string section_name(str, 1, str.size()-2);
		return section_name;
	}
	std::string section_name;
	return section_name;
}

bool el::lib_file_ini_t::get_name_and_val( const std::string& str, std::string& name, std::string& val )
{
	std::string::size_type pos = str.find('=');
	if (std::string::npos == pos){
		return false;
	}
	std::string t_name(str, 0, pos);
	name = t_name;
	std::string t_val(str, pos + 1, str.size() - 1);
	val = t_val;
	return true;
}

std::string el::lib_file_ini_t::get_strval( const char* section, const char* name )
{
	std::string str;
	auto it = this->section_map.find(section);
	if (this->section_map.end() != it){
		NAME_MAP& r = it->second;
		NAME_MAP::iterator it_name = r.find(name);
		if (r.end() != it_name){
			return it_name->second;
		}
	}
	return str;
}
