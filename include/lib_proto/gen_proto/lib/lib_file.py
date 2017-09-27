#!/usr/bin/python
# -*- coding:utf-8 -*-

#ÎÄ¼ş²Ù×÷
class  lib_file_t:
	def __init__(self):
		self.fd = "";
		
	def open(self, name, flag):
		self.fd = open(name, flag);
		
	def close(self):
		self.fd.close();
	
	def new_line(self, cnt = 1):
		for i in range(0, cnt):
			self.fd.write("\n");

	def new_table(self, cnt = 1):
		for i in range(0, cnt):
			self.fd.write("\t");

	def write_line(self, data, tab = 0, line = 1):
		self.new_table(tab);
		self.fd.write(data);
		self.new_line(line);