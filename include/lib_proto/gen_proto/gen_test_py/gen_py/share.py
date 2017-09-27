#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
reload(sys)
sys.setdefaultencoding('utf-8')
sys.path.append("../gen_test_py/");
import show
import share


#物体位置
class object_pos_t:
	#坐标x
	#uint32_t x;
	#坐标y
	#uint32_t y;

	def read_from_buf(self, ba):
		#坐标x single uint32 x
		self.x = ba.read_uint32();
		if(self.x == None): return False;
		#坐标y single uint32 y
		self.y = ba.read_uint32();
		if(self.y == None): return False;

		return True;

	def write_to_buf(self, ba):
		#坐标x single uint32 x
		if(not ba.write_uint32(self.x)): return False;
		#坐标y single uint32 y
		if(not ba.write_uint32(self.y)): return False;

		return True;

	def echo(self, print_hex=False, tabs=""):
		#坐标x single uint32 x
		print tabs+r"======坐标x single uint32 x";
		print tabs+"[x]=%s"%(str(self.x));
		#坐标y single uint32 y
		print tabs+r"======坐标y single uint32 y";
		print tabs+"[y]=%s"%(str(self.y));

		return True;



#物体位置
class object_t:
	#坐标x
	#object_pos_t x;
	#坐标y
	#std::vector<object_pos_t> y;
	#坐标y
	#char ys[100];

	def read_from_buf(self, ba):
		#坐标x single object_pos_t x
		self.x = share.object_pos_t();
		if( not self.x.read_from_buf(ba)): return False;
		#坐标y list object_pos_t y
		y_list_cnt__ = ba.read_uint32();
		if(y_list_cnt__ == None): return False;
		self.y = [];
		for i in range(y_list_cnt__):
			self.y.append(share.object_pos_t());
			if( not self.y[i].read_from_buf(ba)): return False;
		#坐标y array char ys
		self.ys = ba.read_buf(100);
		if(self.ys == None): return False;

		return True;

	def write_to_buf(self, ba):
		#坐标x single object_pos_t x
		if(not isinstance(self.x, share.object_pos_t)): return False;
		if(not self.x.write_to_buf(ba)): return False;
		#坐标y list object_pos_t y
		y_list_cnt__ = len(self.y);
		ba.write_uint32(y_list_cnt__);
		for i in range(y_list_cnt__):
			if(not isinstance(self.y[i], share.object_pos_t)): return False;
			if(not self.y[i].write_to_buf(ba)): return False;
		#坐标y array char ys
		ba.write_buf(self.ys, 100);

		return True;

	def echo(self, print_hex=False, tabs=""):
		#坐标x single object_pos_t x
		print tabs+r"======坐标x single object_pos_t x";
		print tabs+"[x]=>{";
		self.x.echo(print_hex, tabs+"    ");
		print tabs+"}";
		#坐标y list object_pos_t y
		print tabs+r"======坐标y list object_pos_t y";
		print tabs+r"[y]=>{";
		for i in range(len(self.y)):
			print tabs+r"    [%d]=>{"%(i);
			self.y[i].echo(print_hex,tabs+"    "*2);
		print tabs+"}\n";
		#坐标y array char ys
		print tabs+r"======坐标y array char ys";
		tmp_str=str(self.ys);
		if(print_hex):tmp_str=show.bin2hex(tmp_str);
		print tabs+"[ys]=%s"%(tmp_str);

		return True;

cmd_map={};
