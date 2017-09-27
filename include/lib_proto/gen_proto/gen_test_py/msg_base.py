#!/usr/bin/python
# -*- coding:utf-8 -*-

import struct

class msg_byte_t:
	def __init__(self):
		self.m_buf = "";
		self.m_size = 0;
		self.m_postion = 0;
		self.m_is_read_mode = True;
		
	def is_end(self):
		return self.m_postion == self.m_size;

	def init_read_mode(self, buf):
		self.m_buf = buf;
		self.m_postion = 0;
		self.m_size = len(buf);
		self.m_is_read_mode = True;
		
	def init_write_mode(self):
		self.m_buf = "";
		self.m_postion = 0;
		self.m_size = 0;
		self.m_is_read_mode = False;

	def get_left_len(self):
		return self.m_size - self.m_postion;

	def read_buf(self, item_len):
		if not self.m_is_read_mode : return None;
		if (self.m_postion + item_len > self.m_size): return None;
		value = self.m_buf[self.m_postion : self.m_postion + item_len] ;
		self.m_postion = self.m_postion + item_len
		return value;
		
	def read_string16(self, item_len, max_len__):
		if not self.m_is_read_mode : return None;
		if (self.m_postion + item_len > self.m_size): return None;
		if (item_len > max_len__): return None;
		value = self.m_buf[self.m_postion : self.m_postion + item_len] ;
		self.m_postion = self.m_postion + item_len
		return value;
		
	def read_single_value(self, frm_str, item_len):
		if not self.m_is_read_mode: return None;
		if (self.m_postion + item_len > self.m_size): return None;
		value = struct.unpack("<" + frm_str, self.m_buf[self.m_postion : self.m_postion + item_len]);
		self.m_postion = self.m_postion + item_len
		return value[0];

	def write_buf(self, buf, item_len):
		if self.m_is_read_mode : return False;
		buf_len = len(buf);
		if (buf_len >= item_len):
			self.m_buf = self.m_buf + buf[0 : item_len];
		else:
			self.m_buf = self.m_buf + buf + "\0" * (item_len - buf_len);
		return True;
		
	def write_string16(self, buf, item_len, max_len__):
		if self.m_is_read_mode : return False;
		if item_len > max_len__: return False;
		buf_len = len(buf);
		if (buf_len >= item_len):
			self.m_buf = self.m_buf + buf[0 : item_len];
		else:
			self.m_buf = self.m_buf + buf + "\0" * (item_len - buf_len);
		return True;

	def write_single_value(self, frm_str, value):
		if self.m_is_read_mode : return False;
		self.m_buf = self.m_buf + struct.pack("<" + frm_str, value);
		return True;
		
	def read_uint32(self):
		return self.read_single_value("L", 4);
		
	def write_uint32(self, value):
		return self.write_single_value("L", value);

	def read_uint16(self):
		return self.read_single_value("H", 2);

	def write_uint16(self, value):
		return self.write_single_value("H", value);

	def read_uint8(self):
		return self.read_single_value("B", 1);

	def write_uint8(self, value):
		return self.write_single_value("B", value);

	def read_int32(self):
		return self.read_single_value("l", 4);

	def write_int32(self, value):
		return self.write_single_value("l", value);

	def read_int16(self):
		return self.read_single_value("h", 2);

	def write_int16(self, value):
		return self.write_single_value("h", value);

	def read_int8(self):
		return self.read_single_value("b", 1);

	def write_int8(self, value):
		return self.write_single_value("b", value);
		
	def read_char(self):
		return self.read_single_value("c", 1);

	def write_char(self, value):
		return self.write_single_value("c", value);
		
	def read_float(self):
		return self.read_single_value("f", 4);

	def write_float(self, value):
		return self.write_single_value("f", value);