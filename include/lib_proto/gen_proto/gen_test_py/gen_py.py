#!/usr/bin/python
# -*- coding:utf-8 -*-

import sys
reload(sys)
sys.setdefaultencoding('utf-8')

sys.path.append("../lib/")
import lib_file
sys.path.append("../gen_cpp/");
import parse
import pro_type

g_file = lib_file.lib_file_t();

def gen_py_header_detailed(file):
	file.write_line(r"#!/usr/bin/python", 0);
	file.write_line(r"# -*- coding:utf-8 -*-", 0);
	file.write_line(r"import sys", 0);
	file.write_line(r"reload(sys)", 0);
	file.write_line(r"sys.setdefaultencoding('utf-8')", 0);
	file.write_line(r'sys.path.append("../gen_test_py/");', 0);
	file.write_line(r"import show", 0);
	file.write_line(r"import share", 0);

share_struct = r"share.";
#写py文件
def gen_py_detailed(file, xml_data):
	for struct_data in xml_data:
		file.new_line(2);
		file.write_line(r"#" + struct_data.desc);
		file.write_line(r"class " + struct_data.name + r":");

		for field_data in struct_data.fields:
			file.write_line(r"#" + field_data.desc, 1);
			if "single" == field_data.mode:
				file.write_line(r"#" + pro_type.get_type(field_data.type) + r" " + field_data.name + r";", 1);
			elif "list" == field_data.mode:
				file.write_line(r"#" + r"std::vector<" + pro_type.get_type(field_data.type) + r"> " + field_data.name + r";", 1);
			elif "array" == field_data.mode:
				file.write_line(r"#" + pro_type.get_type(field_data.type) + r" " + field_data.name + r"[" + field_data.size + r"];", 1);
		file.new_line();

		#read();
		file.write_line(r"def read_from_buf(self, ba):", 1);
		for field_data in struct_data.fields:
			file.write_line(r"#" + field_data.desc + r" " + field_data.mode + r" " + field_data.type + r" " + field_data.name, 2);
			if "single" == field_data.mode:
				if pro_type.is_sys_type(field_data.type):
					if "string16" == field_data.type:
						file.write_line(r"self." + field_data.name + "_len___ = ba.read_uint16" + "();", 2);
						file.write_line(r"self." + field_data.name + r" = ba.read_" + field_data.type + "(" + r"self." + field_data.name + r"_len___, " + field_data.max_len + r");", 2);
						file.write_line(r"if(self." + field_data.name + " == None): return False;", 2);
						
					else:
						file.write_line(r"self." + field_data.name + " = ba.read_" + field_data.type + "();", 2);
						file.write_line(r"if(self." + field_data.name + " == None): return False;", 2);
				else:
					file.write_line(r"self." + field_data.name + " = " + share_struct + field_data.type + "();", 2);
					file.write_line(r"if( not self." + field_data.name + ".read_from_buf(ba)): return False;", 2);
			elif "list" == field_data.mode:
				field_data_name_list_cnt = field_data.name + "_list_cnt__";
				field_data_name_list_item = field_data.name + "_list_item__";
				file.write_line(field_data_name_list_cnt + " = ba.read_uint32();", 2);
				file.write_line(r"if(" + field_data_name_list_cnt + r" == None): return False;", 2);
				file.write_line(r"self." + field_data.name + r" = [];", 2);
				file.write_line(r"for i in range(" + field_data_name_list_cnt + r"):", 2);
				if pro_type.is_sys_type(field_data.type):
					file.write_line(r"self." + field_data.name + ".append(ba.read_" + field_data.type + r"());", 3);
					file.write_line(r"if(self." + field_data.name + r"[i] == None): return False;", 3);
				else:
					file.write_line(r"self." + field_data.name + ".append(" + share_struct + field_data.type + r"());", 3);
					file.write_line(r"if( not self." + field_data.name + r"[i].read_from_buf(ba)): return False;", 3);
			elif "array" == field_data.mode:
				if pro_type.is_sys_type(field_data.type):
					file.write_line(r"self." + field_data.name + r" = ba.read_buf(" + field_data.size + r");", 2);
					file.write_line(r"if(self." + field_data.name + " == None): return False;", 2);
				else:
					file.write_line(r"error!", 2);
		file.new_line();
		file.write_line(r"return True;", 2, 2);

		#write();
		file.write_line(r"def write_to_buf(self, ba):", 1);
		for field_data in struct_data.fields:
			file.write_line(r"#" + field_data.desc + r" " + field_data.mode + r" " + field_data.type + r" " + field_data.name, 2);
			if "single" == field_data.mode:
				if pro_type.is_sys_type(field_data.type):
					if "string16" == field_data.type:
						file.write_line(r"self." + field_data.name + "_len__ = len(self." + field_data.name + r");", 2);
						file.write_line(r"if(not ba.write_uint16" + r"(self." + field_data.name + "_len__)): return False;", 2);	
						file.write_line(r"if(not ba.write_" + field_data.type + r"(self." + field_data.name + ", self." + field_data.name + "_len__, " + field_data.max_len + r")): return False;", 2);	
					else:
						file.write_line(r"if(not ba.write_" + field_data.type + r"(self." + field_data.name + ")): return False;", 2);
				else:
					file.write_line(r"if(not isinstance(self." + field_data.name + r", " + share_struct + field_data.type +")): return False;", 2);
					file.write_line(r"if(not self." + field_data.name + r".write_to_buf(ba)): return False;", 2);
			elif "list" == field_data.mode:
				field_data_name_list_cnt = field_data.name + "_list_cnt__";
				field_data_name_list_item = field_data.name + "_list_item__";
				file.write_line(field_data_name_list_cnt + " = len(self." + field_data.name + r");", 2);
				file.write_line(r"ba.write_uint32(" +field_data_name_list_cnt+r");", 2);
				file.write_line(r"for i in range(" + field_data_name_list_cnt + r"):", 2);
				if pro_type.is_sys_type(field_data.type):
					file.write_line(r"if(not ba.write_" + field_data.type + r"(self." +field_data.name+r"[i])): return False;", 3);
				else:
					file.write_line(r"if(not isinstance(self." +field_data.name+r"[i], "+ share_struct + field_data.type+")): return False;", 3);
					file.write_line(r"if(not self." +field_data.name+r"[i].write_to_buf(ba)): return False;", 3);
			elif "array" == field_data.mode:
				if pro_type.is_sys_type(field_data.type):
					file.write_line(r"ba.write_buf(self." + field_data.name + ", "+field_data.size+r");", 2);
				else:
					file.write_line(r"error!", 2);
		file.new_line();
		file.write_line(r"return True;", 2, 2);
		
		#echo();
		file.write_line(r'def echo(self, print_hex=False, tabs=""):', 1);
		for field_data in struct_data.fields:
			file.write_line(r"#" + field_data.desc + r" " + field_data.mode + r" " + field_data.type + r" " + field_data.name, 2);
			file.write_line(r'print tabs+r"======' + field_data.desc + r" " + field_data.mode + r" " + field_data.type + r" " + field_data.name + r'";', 2);
			if "single" == field_data.mode:
				if pro_type.is_sys_type(field_data.type):
					file.write_line(r'print tabs+"[' + field_data.name +  r']=%s"%(str(self.' + field_data.name + r'));', 2);
				else:
					file.write_line(r'print tabs+"[' + field_data.name + r']=>{";', 2);
					file.write_line(r'self.' + field_data.name +r'.echo(print_hex, tabs+"    ");', 2);
					file.write_line(r'print tabs+"}";', 2);
			elif "list" == field_data.mode:
				file.write_line(r'print tabs+r"[' + field_data.name + r']=>{";', 2);
				file.write_line(r'for i in range(len(self.' + field_data.name + ')):', 2);
				if pro_type.is_sys_type(field_data.type):
					file.write_line(r'print tabs+"    [%d]="%(i)+str(self.' + field_data.name + r'[i]); ', 3);
				else:
					file.write_line(r'print tabs+r"    [%d]=>{"%(i);', 3);
					file.write_line(r'self.' + field_data.name + r'[i].echo(print_hex,tabs+"    "*2);', 3);
				file.write_line(r'print tabs+"}\n";', 2);
			elif "array" == field_data.mode:
				if pro_type.is_sys_type(field_data.type):
					file.write_line(r'tmp_str=str(self.' + field_data.name + r');', 2);
					file.write_line(r'if(print_hex):tmp_str=show.bin2hex(tmp_str);', 2);
					file.write_line(r'print tabs+"['+ field_data.name +r']=%s"%(tmp_str);', 2);
				else:
					file.write_line(r"error!", 2);
		file.new_line();
		file.write_line(r"return True;", 2, 2);
		
		
def gen_py_cmd_map(file, xml_data):
	#cmd_map
	file.write_line(r"cmd_map={};");
	for struct_data in xml_data:
		file.write_line(r"cmd_map[" + struct_data.id +r"]=('" + struct_data.name + r"', " + struct_data.struct_in + r", " + struct_data.struct_out + r");");

def gen_py(xml_path, xml_name, gen_py_path):
	xmlname = xml_path + xml_name + r".xml";
	gen_py_name = gen_py_path + xml_name + r".py";
	print xmlname;
	print gen_py_name;
############################################################
	#生成.py
	g_file.open(gen_py_name, "w");
	gen_py_header_detailed(g_file);
	parse.get_xml_data_structs(xmlname);
	gen_py_detailed(g_file, parse.g_structs_data);
	parse.get_xml_data_protocols(xmlname);
	gen_py_detailed(g_file, parse.g_structs_data);
	
	parse.get_xml_data_protocols_cmd(xmlname);
	gen_py_cmd_map(g_file, parse.g_structs_data);
	g_file.close();
	
