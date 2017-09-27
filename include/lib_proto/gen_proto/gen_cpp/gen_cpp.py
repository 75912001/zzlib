#!/usr/bin/python
# -*- coding:utf-8 -*-

import sys,os,md5
reload(sys)
sys.setdefaultencoding('utf-8')

import parse
import pro_type

sys.path.append("./lib/")
import lib_file

g_file = lib_file.lib_file_t();

def gen_cpp_header_detailed(file, xml_name):
	file.write_line(r"#include <lib_include.h>", 0);
	file.write_line(r"#include <lib_log.h>", 0);
	file.write_line(r"#include <lib_proto/lib_msg.h>", 0, 2);
	if "share" != xml_name:
		file.write_line(r'#include "share.h"', 0, 2);
	file.write_line(r"#pragma once", 0, 1);
	file.write_line(r"#pragma pack(1)", 0, 1);
	
#写cpp文件
def gen_cpp_detailed(file, xml_data, base_class_name):
	for struct_data in xml_data:
		file.new_line(2);
		file.write_line(r"//" + struct_data.desc);
		file.write_line(r"struct " + struct_data.name + base_class_name + r"{");
		
		init_string = [];
		for field_data in struct_data.fields:
			file.write_line(r"//" + field_data.desc, 1);
			if "single" == field_data.mode:
				if "std::string" == pro_type.get_type(field_data.type):
					file.write_line(r"static const uint32_t " + field_data.name + r"_max_len__ = " + field_data.max_len + r";", 1);
				file.write_line(pro_type.get_type(field_data.type) + r" " + field_data.name + r";", 1);
				
				if pro_type.is_sys_type(field_data.type):
					if "std::string" == pro_type.get_type(field_data.type):
						init_string.append(r"this->" + field_data.name + r".clear();");
					else:
						init_string.append(r"this->" + field_data.name + r" = 0;");
				else:
					init_string.append(r"this->" + field_data.name + r".init();");
			elif "list" == field_data.mode:
				file.write_line(r"std::vector<" + pro_type.get_type(field_data.type) + r"> " + field_data.name + r";", 1);
				init_string.append(r"this->" + field_data.name + r".clear();");
			elif "array" == field_data.mode:
				file.write_line(pro_type.get_type(field_data.type) + r" " + field_data.name + r"[" + field_data.size + r"];", 1);
				if pro_type.is_sys_type(field_data.type):
					init_string.append(r"::memset(this->" + field_data.name + r", 0, sizeof(this->" + field_data.name + r"));");
		file.new_line();
		#初始化函数
		file.write_line(struct_data.name + r"(){this->init();}", 1);
		
		#函数init()
		file.write_line(r"virtual void init(){", 1);
		for i in init_string:
			file.write_line(i, 2);
		file.write_line(r"}", 1);

		#read();
		file.write_line(r"virtual bool read(el::lib_msg_byte_t& msg_byte){", 1);
		for field_data in struct_data.fields:
			if "single" == field_data.mode:
				if pro_type.is_sys_type(field_data.type):
					if "std::string" == pro_type.get_type(field_data.type):
						file.write_line(r"if(!msg_byte.read_" + field_data.type + "(this->" + field_data.name + ", " + struct_data.name + r"::" + field_data.name + r"_max_len__)) return false;", 2);
					else:
						file.write_line(r"if(!msg_byte.read_" + field_data.type + "(this->" + field_data.name + ")) return false;", 2);
				else:
					file.write_line(r"if(!this->" + field_data.name + ".read(msg_byte)) return false;", 2);
			elif "list" == field_data.mode:
				field_data_name_list_cnt = field_data.name + "_list_cnt__";
				field_data_name_list_item = field_data.name + "_list_item__";
				file.write_line(r"uint32_t " + field_data_name_list_cnt + ";", 2);
				file.write_line(r"if(!msg_byte.read_uint32(" + field_data_name_list_cnt + ")) return false;", 2);
#				if pro_type.is_sys_type(field_data.type):
#					file.write_line(pro_type.get_type(field_data.type) + " " + field_data_name_list_item + ";", 2);
#				else:
#					file.write_line(field_data.type + " " + field_data_name_list_item + ";", 2);
				file.write_line(r"this->" + field_data.name + ".clear();", 2);
				file.write_line(r"for(uint32_t i = 0; i < " + field_data_name_list_cnt + "; i++){", 2);
				if pro_type.is_sys_type(field_data.type):
					file.write_line(pro_type.get_type(field_data.type) + " " + field_data_name_list_item + ";", 3);
				else:
					file.write_line(field_data.type + " " + field_data_name_list_item + ";", 3);
				if pro_type.is_sys_type(field_data.type):
					file.write_line(r"if(!msg_byte.read_" + field_data.type + "(" + field_data_name_list_item + ")) return false;", 3);
				else:
					file.write_line(r"if(!" + field_data_name_list_item + ".read(msg_byte)) return false;", 3);
				
				file.write_line(r"this->" + field_data.name + ".push_back(" + field_data_name_list_item + ");", 3);
				file.write_line(r"}", 2);
			elif "array" == field_data.mode:
				if pro_type.is_sys_type(field_data.type):
					if pro_type.is_sys_type_integer(field_data.type):
						file.write_line(r"for(uint32_t i = 0; i < " + field_data.size + "; i++){", 2);
						file.write_line(r"if(!msg_byte.read_" + field_data.type + "(this->" + field_data.name + "[i])) return false;", 3);
						file.write_line(r"}", 2);
					else:
						file.write_line(r"if(!msg_byte.read_buf((char*)this->" + field_data.name + ", sizeof(this->" + field_data.name + "))) return false;", 2);
				else:
					file.write_line(r"error!", 2);
				
		file.write_line(r"return true;", 2);
		file.write_line(r"}", 1);
		
		#write();
		file.write_line(r"virtual bool write(el::lib_msg_byte_t& msg_byte){", 1);
		for field_data in struct_data.fields:
			if "single" == field_data.mode:
				if pro_type.is_sys_type(field_data.type):
					if "std::string" == pro_type.get_type(field_data.type):
						file.write_line(r"if(!msg_byte.write_" + field_data.type + "(this->" + field_data.name + ", " + struct_data.name + r"::" + field_data.name + r"_max_len__)) return false;", 2);
					else:
						file.write_line(r"if(!msg_byte.write_" + field_data.type + "(this->" + field_data.name + ")) return false;", 2);
				else:
					file.write_line(r"if(!this->" + field_data.name + ".write(msg_byte)) return false;", 2);
			elif "list" == field_data.mode:
				file.write_line(r"if(!msg_byte.write_uint32(this->" + field_data.name + ".size())) return false;", 2);
				file.write_line(r"for(uint32_t i = 0; i < this->" + field_data.name + ".size(); i++){", 2);
				if pro_type.is_sys_type(field_data.type):
					file.write_line("if(!msg_byte.write_" + field_data.type + "(this->" + field_data.name + "[i])) return false;", 3);
				else:
					file.write_line(r"if(!this->" + field_data.name + "[i].write(msg_byte)) return false;", 3);
				file.write_line(r"}", 2);
			elif "array" == field_data.mode:
				if pro_type.is_sys_type(field_data.type):
				############################3
					if pro_type.is_sys_type_integer(field_data.type):
						file.write_line(r"for(uint32_t i = 0; i < " + field_data.size + "; i++){", 2);
						file.write_line(r"if(!msg_byte.write_" + field_data.type + "(this->" + field_data.name + "[i])) return false;", 3);
						file.write_line(r"}", 2);
					else:
				##############################
						if "char" != field_data.type and "uint8_t" != field_data.type and "int8_t" != field_data.type:
							file.write_line(r"error! type", 2);
						else:
							file.write_line(r"if(!msg_byte.write_buf(this->" + field_data.name + ", sizeof(this->" + field_data.name +"))) return false;", 2);

		file.write_line(r"return true;", 2);
		file.write_line(r"}", 1);
		
		#show();
		file.write_line(r"virtual void show(uint32_t idx = 0){", 1);
		file.write_line(r"std::string pre_str__;", 2);
		file.write_line(r"for(uint32_t i = 0; i < idx; i++){", 2);
		file.write_line(r'pre_str__ += "<<";', 3);
		file.write_line(r"}", 2);
		file.write_line(r"idx++;", 2);
		for field_data in struct_data.fields:
			if "single" == field_data.mode:
				if pro_type.is_sys_type(field_data.type):
					if "std::string" == pro_type.get_type(field_data.type):
						file.write_line(r'TRACE_MSG_LOG("%s[' + field_data.name + r":" + pro_type.gen_show_type(field_data.type) + r']", pre_str__.c_str(), this->' + field_data.name + r'.c_str());', 2);
					else:
						file.write_line(r'TRACE_MSG_LOG("%s[' + field_data.name + r":" + pro_type.gen_show_type(field_data.type) + r']", pre_str__.c_str(), this->' + field_data.name + r');', 2);
				else:
					file.write_line(r"this->" + field_data.name + r".show(idx);", 2);
			elif "list" == field_data.mode:
				file.write_line(r'TRACE_MSG_LOG("%s[' + field_data.name + r'_size:%u]", pre_str__.c_str(), (uint32_t)this->' + field_data.name + r'.size());', 2);
				
				file.write_line(r"for(uint32_t i = 0; i < " + field_data.name + r".size(); i++){", 2);
				file.write_line(r'TRACE_MSG_LOG("idx__:%u", i + 1);', 3);
				if pro_type.is_sys_type(field_data.type):
					if "std::string" == pro_type.get_type(field_data.type):
						file.write_line(r'TRACE_MSG_LOG("%s[' + field_data.name + r":" + pro_type.gen_show_type(field_data.type) + r']", pre_str__.c_str(), ' + field_data.name + r'[i].c_str());', 3);
					else:
						file.write_line(r'TRACE_MSG_LOG("%s[' + field_data.name + r":" + pro_type.gen_show_type(field_data.type) + r']", pre_str__.c_str(), ' + field_data.name + r'[i]);', 3);
				else:
					file.write_line(r"this->" + field_data.name + r"[i].show(idx);", 3);

				file.write_line(r"}", 2);
			elif "array" == field_data.mode:
				if pro_type.is_sys_type(field_data.type):
				########################
					if pro_type.is_sys_type_integer(field_data.type):
						file.write_line(r"for(uint32_t i = 0; i < " + field_data.size + "; i++){", 2);
						file.write_line(r'TRACE_MSG_LOG("%s[' + field_data.name + r"_%u:" + pro_type.gen_show_type(field_data.type) + r']", pre_str__.c_str(), i, (uint32_t)this->' + field_data.name + r'[i]);', 3);
						file.write_line(r"}", 2);
					else:
				########################
						file.write_line(r'TRACE_MSG_LOG("%s[' + field_data.name + r":" + pro_type.gen_show_type(field_data.type) + r']", pre_str__.c_str(), this->' + field_data.name + r');', 2);
				else:
					file.write_line(r"error!", 2);
		file.write_line(r"}", 1);
		
		file.write_line(r"};");


#写cpp_bind文件
def gen_cpp_cmd(xml_data, gen_cpp_cmd_name_pre):
	#生成完整的xx_cmd.h
	xx_cmd = gen_cpp_cmd_name_pre + r"_cmd.h";
	g_file.open(xx_cmd, "w");
	for cmd_data in xml_data:
		server_list = "";
		if cmd_data.service == '':
			server_list = "null";
		else:
			server_list = cmd_data.service;
		g_file.write_line(r"BIND_PROTO_CMD(" + cmd_data.id + r", " + cmd_data.name + r", " + cmd_data.struct_in + r", " + cmd_data.struct_out + r", " + server_list + r");");
			
	g_file.close();
	
	file_map = {};
	for cmd_data in xml_data:
		server_names = cmd_data.service.split('|');
		for server_name in server_names:
			if server_name != '':
				if False == file_map.has_key(server_name):
					file_map[server_name] = [];
				file_map[server_name].append(r"BIND_PROTO_CMD(" + cmd_data.id + r", " + cmd_data.name + r", " + cmd_data.struct_in + r", " + cmd_data.struct_out + r", " + cmd_data.service + r");");
		
	for k in file_map:
		new_name = gen_cpp_cmd_name_pre + r"_" + k + r"_cmd.h";
		g_file.open(new_name, "w");
		for i in file_map[k]:
			g_file.write_line(i);
		g_file.close();

#生成文件的MD5码		
def gen_md5_key(filename):
	if not os.path.exists(filename):
		return 0;
	file_fd = open(filename, 'rb');
	key = md5.new();
	key.update(file_fd.read());
	file_fd.close();
	set_key = key.hexdigest();
	return set_key;
	
def gen_cpp(xml_path, xml_name, gen_cpp_path):
	xmlname = xml_path + xml_name + r".xml";
	gen_cpp_name = gen_cpp_path + xml_name + r".h";
	gen_cpp_name_new = gen_cpp_path + xml_name + r".h.new";
	gen_cpp_cmd_name_pre = gen_cpp_path + xml_name;# + r"_cmd.h";
	############################################################
	#写到临时文件 .xxx.h.new 中
	g_file.open(gen_cpp_name_new, "w");
	gen_cpp_header_detailed(g_file, xml_name);
	parse.get_xml_data_structs(xmlname);
	gen_cpp_detailed(g_file, parse.g_structs_data, " : public el::lib_msg_t");
	parse.get_xml_data_protocols(xmlname);
	gen_cpp_detailed(g_file, parse.g_structs_data, " : public el::lib_msg_t");
	g_file.write_line(r"#pragma pack()", 0, 2);
	g_file.close();
	
	new_md5 = gen_md5_key(gen_cpp_name_new);
	os.remove(gen_cpp_name_new);
	
	old_md5 = gen_md5_key(gen_cpp_name);
	
#	print new_md5;
#	print old_md5;
	
#	if new_md5 != old_md5:
	print "change xml file:" + xmlname;
	#############################################################
	g_file.open(gen_cpp_name, "w");
	gen_cpp_header_detailed(g_file, xml_name);
	parse.get_xml_data_structs(xmlname);
	gen_cpp_detailed(g_file, parse.g_structs_data, " : public el::lib_msg_t");
	parse.get_xml_data_protocols(xmlname);
	gen_cpp_detailed(g_file, parse.g_structs_data, " : public el::lib_msg_t");
	g_file.write_line(r"#pragma pack()", 0, 2);
	g_file.close();
	
	parse.get_xml_data_protocols_cmd(xmlname);
	gen_cpp_cmd(parse.g_structs_data, gen_cpp_cmd_name_pre);
	#############################################################
	
	

