#!/usr/bin/python
# -*- coding:utf-8 -*-

import struct
import socket
import time
import msg_base


#协议通信
class  proto_base_t:
	def __init__(self, ip, port, user_id = 0):
		self.len = 0;#协议的长度
		self.cmd = 0;#协议的命令号
		self.id = user_id;#账号
		self.seq = 0;#序列号
		self.ret = 0;#S->C, 错误码
		
		self.headerlen = 20;
		self.ip = ip;
		self.port = port;
		self.set_unpack_header_fmt_str("<LLLLL");
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);   
		self.sock.connect((self.ip, self.port));
		self.recvbuf="";
		#设置为非阻塞连接	
		self.sock.setblocking(0);

	def pack(self, cmd, pri_in):
		ba = msg_base.msg_byte_t();
		ba.init_write_mode();
		if (pri_in <> None):
			pri_in.write_to_buf(ba);
		headermsg = self.pack_proto_header(len(ba.m_buf), cmd);
		msg = headermsg + ba.m_buf;
		return msg;

	#得到头部的报文,可能需要子类实现
	def pack_proto_header(self, pri_len, cmd):
		headermsg = struct.pack("<LLLLL", self.headerlen + pri_len,
				cmd, self.id, self.seq, self.ret);
		return headermsg;

	#可能需要子类设置
	def set_unpack_header_fmt_str(self, frm_str="<LLLLL"):
		self.unpack_header_fmt_str = frm_str;

	#得到返回的消息
	def getrecvmsg(self):
		tmp_recvfbuf="";
		try:
			tmp_recvfbuf = self.sock.recv(819200);   
			if(tmp_recvfbuf == ""):
				print "socket close..";
			 	exit(1);
		except socket.error, msg:
			if(msg[0] <> 11):
				print "socket error:" + msg[0];
				exit(1);
		self.recvbuf = self.recvbuf + tmp_recvfbuf;
		#得到报文		
		if (len(self.recvbuf) <= 4):
			return False ;
		else:
		 	#得到长度
		 	recvmsg_arr = struct.unpack("<L", self.recvbuf[0:4]);
			recvmsg_len = recvmsg_arr[0];
			if(recvmsg_len > len(self.recvbuf)):
				return False;
			if(recvmsg_len < self.headerlen):
				return False;
			returnmsg = self.recvbuf[0:recvmsg_len];
			self.recvbuf = self.recvbuf[recvmsg_len:];

			pri_len = recvmsg_len - self.headerlen;
			return struct.unpack(self.unpack_header_fmt_str + str(pri_len) + "s", returnmsg);

	def loop_get_recv_msg(self, cmd_id):
		self.recvbuf="";
		while True:
			out_info = self.getrecvmsg();
			if(out_info):
				self.len,self.cmd,self.id,self.seq,self.ret,pri_msg = out_info;
				#print "len:";print self.len;print "cmd:";print self.cmd;print "id:";print self.id;print "seq:";print self.seq;print "ret:";print self.ret;
				if(cmd_id == self.cmd):
					#return pri_msg;
					return out_info;
			time.sleep(0.001);

	def sendmsg(self, cmdid, pri_in):
		sendbuf = self.pack(cmdid, pri_in);
		while(True):
			try:
				self.sock.send(sendbuf);
				return;
			except socket.error:
				continue;
			
		
#斗龙协议通信
class  dwo_proto_t(proto_base_t):
	def __init__(self, ip, port, user_id = 0):
		self.majic = 21316;#不加密的code
		self.len = 0;#协议的长度
		self.cmd = 0;#协议的命令号
		self.id = user_id;#账号
		self.seq = 0;#序列号
		self.ret = 0;#S->C, 错误码
		
		self.headerlen = 0;
		self.ip = ip;
		self.port = port;
		self.set_unpack_header_fmt_str("<HLLLLL");
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);   
		self.sock.connect((self.ip, self.port));
		self.recvbuf="";
		#设置为非阻塞连接	
		self.sock.setblocking(0);

	#得到头部的报文,可能需要子类实现
	def pack_proto_header(self, pri_len, cmd):
		headermsg = struct.pack("<HLLLLL", self.majic, self.headerlen + pri_len,
				cmd, self.id, self.seq, self.ret);
		return headermsg;

	#可能需要子类设置
	def set_unpack_header_fmt_str(self, frm_str="<HLLLLL"):
		self.unpack_header_fmt_str = frm_str;

	#得到返回的消息
	def getrecvmsg(self):
		tmp_recvfbuf="";
		try:
			tmp_recvfbuf = self.sock.recv(819200);   
			if(tmp_recvfbuf == ""):
				print "socket close..";
			 	exit(1);
		except socket.error, msg:
			if(msg[0] <> 11):
				print "socket error:" + msg[0];
				exit(1);
		self.recvbuf = self.recvbuf + tmp_recvfbuf;
		#得到报文		
		if (len(self.recvbuf) <= 6):
			return False ;
		else:
		 	#得到长度
		 	recvmsg_arr = struct.unpack("<L", self.recvbuf[2:6]);
			recvmsg_len = recvmsg_arr[0];
			if(recvmsg_len > len(self.recvbuf)):
				return False;
			if(recvmsg_len < self.headerlen):
				return False;
			returnmsg = self.recvbuf[0:recvmsg_len + 22];
			self.recvbuf = self.recvbuf[recvmsg_len:];
			pri_len = recvmsg_len - self.headerlen;
			return struct.unpack(self.unpack_header_fmt_str + str(pri_len) + "s", returnmsg);

	def loop_get_recv_msg(self, cmd_id):
		self.recvbuf="";
		while True:
			out_info = self.getrecvmsg();
			if(out_info):
				self.majic,self.len,self.cmd,self.id,self.seq,self.ret,pri_msg = out_info;
				#print "len:";print self.len;print "cmd:";print self.cmd;print "id:";print self.id;print "seq:";print self.seq;print "ret:";print self.ret;
				if(cmd_id == self.cmd):
					#return pri_msg;
					return out_info;
				else:
					print "len:";print self.len;print "cmd:";print self.cmd;print "id:";print self.id;print "seq:";print self.seq;print "ret:";print self.ret;
					if(0 == cmd_id):
						self.recvbuf = "";
					self.recvbuf = "";
			time.sleep(0.001);
			
############################
#gametree协议通信
class  gt_proto_t(proto_base_t):
	def __init__(self, ip, port):
		self.len = 0;#协议的长度
		self.cmd = 0;#协议的命令号

		self.headerlen = 8;
		self.ip = ip;
		self.port = port;
		self.set_unpack_header_fmt_str("<LL");
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);   
		self.sock.connect((self.ip, self.port));
		self.recvbuf="";
		#设置为非阻塞连接	
		self.sock.setblocking(0);

	#得到头部的报文,可能需要子类实现
	def pack_proto_header(self, pri_len, cmd):
		headermsg = struct.pack("<LL", self.headerlen + pri_len, cmd);
		return headermsg;

	#可能需要子类设置
	def set_unpack_header_fmt_str(self, frm_str="<LL"):
		self.unpack_header_fmt_str = frm_str;

	#得到返回的消息
	def getrecvmsg(self):
		tmp_recvfbuf="";
		try:
			tmp_recvfbuf = self.sock.recv(819200);   
			if(tmp_recvfbuf == ""):
				print "socket close..";
			 	exit(1);
		except socket.error, msg:
			if(msg[0] <> 11):
				print "socket error:" + msg[0];
				exit(1);
		self.recvbuf = self.recvbuf + tmp_recvfbuf;
		#得到报文		
		if (len(self.recvbuf) <= 4):
			return False ;
		else:
		 	#得到长度
		 	recvmsg_arr = struct.unpack("<L", self.recvbuf[0:4]);
			recvmsg_len = recvmsg_arr[0];
			if(recvmsg_len > len(self.recvbuf)):
				return False;
			if(recvmsg_len < self.headerlen):
				return False;
			returnmsg = self.recvbuf[0:recvmsg_len];
			self.recvbuf = self.recvbuf[recvmsg_len:];
			pri_len = recvmsg_len; #- self.headerlen;
			return struct.unpack(self.unpack_header_fmt_str + str(pri_len) + "s", returnmsg);

	def loop_get_recv_msg(self, cmd_id):
		self.recvbuf="";
		while True:
			out_info = self.getrecvmsg();
			if(out_info):
				self.len,self.cmd,pri_msg = out_info;
				#print "len:";print self.len;print "cmd:";print self.cmd;print "id:";print self.id;print "seq:";print self.seq;print "ret:";print self.ret;
				if(cmd_id == self.cmd):
					#return pri_msg;
					return out_info;
				else:
					print "len:";print self.len;print "cmd:";print self.cmd;
					if(0 == cmd_id):
						self.recvbuf = "";
					self.recvbuf = "";
			time.sleep(0.001);
