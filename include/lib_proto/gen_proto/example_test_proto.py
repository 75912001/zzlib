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
#增加需要处理的消息模块
import example
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
		if ( 1 == parse_msg(example, cmdid, result, primsg, self)):
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
ip="172.24.32.232";
port=9001;
		
def main():
	p = proto_base.dwo_proto_t(ip, port);

################################################	
#位置
	object_pos = share.object_pos_t();
	object_pos.x = 1;
	object_pos.y = 2;
	
	pri_in = example.cli_walk_in();
	pri_in.pos = object_pos;

#	while True:
#		dwo_gen_msg(p, pri_in, 0x10001);
	for i in range(1, 1000):
		dwo_gen_msg(p, pri_in, 0x10001);
		print i;
	else:
		print("for循环结束");


#################################################		
	time.sleep(1);

if __name__ == "__main__":
	main();
