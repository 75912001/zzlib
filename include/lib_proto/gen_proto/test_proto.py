#!/usr/bin/python
# -*- coding:utf-8 -*-

import time
import sys
reload(sys)
sys.setdefaultencoding('utf-8')

sys.path.append("./gen_test_py/")

import proto_base
import msg_base
import show


################################################
sys.path.append("./gen_test_py/gen_py/")
import db 
import login
import share
################################################
##解析消息(返回1:不在该服务范围内)
def parse_msg(server, cmdid, result, primsg, self):
	if server.cmd_map.has_key(cmdid):
		if (0 != result):
			print "错误码! %#X"%(result);
			return;
		#得到类名
		classname = server.cmd_map[cmdid][2];
		if classname <> None:	
			pri_out = classname() ;
		else:
			pri_out = msg_base.msg_byte_t();
		ba = msg_base.msg_byte_t();
		ba.init_read_mode(primsg);
		ret = pri_out.read_from_buf(ba);
		if(not ret):
			print "解析出错:报文不够";
		if(ba.get_left_len() > 0):
			print "解析出错:有剩余报文";
			show.print_hex_16(primsg[ba.get_postion():]);
		try:
			#得到调用函数
			cmd_name=server.cmd_map[cmdid][0];
			func_name='do_%s'%(cmd_name) ;
			func = getattr(self, func_name);
			func(result, primsg);
		except AttributeError:
			print "deal:%s[%#X]:"%(cmd_name, cmdid);
			pri_out.echo(False, "    ");
			pass;
	else:
		return 1;
################################################
class Dealmsg: 
	def __init__(self):
		pass;
	def dealmsg(self, cmdid, result, primsg):
		if ( 1 == parse_msg(login, cmdid, result, primsg, self)):
			if ( 1 == parse_msg(db, cmdid, result, primsg, self)):
				pass;
			else:
				return;
		else:
			return;

		print "没有处理函数:%#X, %#X"%(cmdid, result);
		show.print_hex_16(primsg);
#	def dealmsg(self, cmdid, result, primsg):
#		if db.cmd_map.has_key(cmdid):
#			if (0 != result):
#				print "错误码!";
#				print result;
#				return;
			#得到类名
#			classname = db.cmd_map[cmdid][2];
#			if classname <> None:	
#				pri_out = classname() ;
#			else:
#				pri_out = msg_base.msg_byte_t();
#			ba = msg_base.msg_byte_t();
#			ba.init_read_mode(primsg);
#			ret = pri_out.read_from_buf(ba);
#			if(not ret):
#				print "解析出错:报文不够";
#			if(ba.get_left_len() > 0):
#				print "解析出错:有剩余报文";
#				show.print_hex_16(primsg[ba.get_postion():]);
#			try:
				#得到调用函数
#				cmd_name=db.cmd_map[cmdid][0];
#				func_name='do_%s'%(cmd_name) ;
#				func = getattr(self, func_name);
#				func(result, primsg);
#			except AttributeError:
#				print "deal:%s[%d]:"%(cmd_name, cmdid);
#				pri_out.echo(False, "    ");
#				pass;
#		else:
#			print "没有处理函数:", cmdid, result;
#			show.print_hex_16(primsg);

dm=Dealmsg();

#发送/接收 消息
def gen_msg(p, send_buf, cmd_id, recv = True):
	p.sendmsg(cmd_id, send_buf);
	if recv:
		rmsg = p.loop_get_recv_msg(cmd_id);
		print rmsg;
		#cmd,ret,body
		dm.dealmsg(rmsg[1], rmsg[4], rmsg[5]);

		#发送/接收 消息
def dwo_gen_msg(p, send_buf, cmd_id, recv = True):
	p.sendmsg(cmd_id, send_buf);
	if recv:
		rmsg = p.loop_get_recv_msg(cmd_id);
		print rmsg;
		#cmd,ret,body
		dm.dealmsg(rmsg[2], rmsg[5], rmsg[6]);
		
#连接服务参数
#ip="192.168.122.12";
#port=9101;
#ip="10.0.0.200";
#port=22401;
ip="192.168.122.12";
port=22200;
		
def main():
	p = proto_base.dwo_proto_t(ip, port);
################################################	
#add items
	item1 = share.item_t();
	item1.id = 1;
	item1.cnt = 2;
	
	item2 = share.item_t();
	item2.id = 3;
	item2.cnt = 4;
	
	items = [item1, item2];
	
	pri_in = db.db_add_itmes_in();
	
	pri_in.items = items;
	dwo_gen_msg(p, pri_in, 10001);

#################################################		
	time.sleep(1);

if __name__ == "__main__":
	main();
