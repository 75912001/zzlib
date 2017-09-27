#!/usr/bin/python
# -*- coding:utf-8 -*-

#系统已知类型定义
g_type_conf = {
	"int8":"int8_t",
	"int16":"int16_t",
	"int32":"int32_t",
	"int64":"int64_t",
	"uint8":"uint8_t",
	"uint16":"uint16_t",
	"uint32":"uint32_t",
	"uint64":"uint64_t",
	"char":"char",
	"float":"float",
	"string16":"std::string",#最大长度为uint16的字符串
};

def is_sys_type(type):
	return g_type_conf.has_key(type);

def is_sys_type_integer(type):
	return "int8" == type or "int16" == type or "int32" == type or "int64" == type or "uint8" == type or "uint16" == type or "uint32" == type or "uint64" == type;



#获取类型名称
def get_type(typestr):
	if is_sys_type(typestr):
		return g_type_conf[typestr];
	else:
		return typestr;
		
#获取打印时的字段类型
def gen_show_type(type):
	if "int8" == type:
		return "%d";
	if "int16" == type:
		return "%d";
	if "int32" == type:
		return "%d";
	if "int64" == type:
		return '%"PRIdFAST64"';
#		return '%ld';
	if "uint8" == type:
		return "%u";
	if "uint16" == type:
		return "%u";
	if "uint32" == type:
		return "%u";
	if "uint64" == type:
		return '%"PRIu64"';
#		return '%lu';
	if "char" == type:
		return "%s";
	if "float" == type:
		return "%f";
	if "string16" == type:
		return "%s";		

	
		
