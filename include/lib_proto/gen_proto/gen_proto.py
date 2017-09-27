#!/usr/bin/python
# -*- coding:utf-8 -*-

import sys
reload(sys)
sys.setdefaultencoding('utf-8')

sys.path.append("./gen_cpp/")
import gen_cpp
sys.path.append("./gen_test_py/")
import gen_py

import os
proto_src = os.environ.get('SERVER_PROTO')

#################################################################
#生成代码的源XML文件
xml_path=proto_src+r"/share/proto/xml/";
#生成cpp文件的路径
gen_cpp_path=proto_src+r"/share/proto/"
#生成测试py文件的路径
gen_py_path = r"./gen_test_py/gen_py/";

def gen_proto(xml_name):
	gen_cpp.gen_cpp(xml_path, xml_name, gen_cpp_path);
	gen_py.gen_py(xml_path, xml_name, gen_py_path);
#################################################################
def main():
	#生成代码的XML文件名
	gen_proto(r"share");
	gen_proto(r"example");
#	gen_proto(r"verify");
#	gen_proto(r"switch");
#	gen_proto(r"gateway");
#	gen_proto(r"world");
#	gen_proto(r"db");
#	gen_proto(r"db_web");
#	gen_proto(r"role");
#	gen_proto(r"enum");
#	gen_proto(r"event");
#	gen_proto(r"err_code");
#	gen_proto(r"btl");
#	gen_proto(r"btl_mate");
#	gen_proto(r"cache");
#	gen_proto(r"annouce");
	
if __name__ == "__main__":
	main();
