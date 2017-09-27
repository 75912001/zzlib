#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
reload(sys)
sys.setdefaultencoding('utf-8')
sys.path.append("../gen_test_py/");
import show
import share


#获取用户走路输入
class cli_walk_in:
	#x位置
	#object_pos_t pos;

	def read_from_buf(self, ba):
		#x位置 single object_pos_t pos
		self.pos = share.object_pos_t();
		if( not self.pos.read_from_buf(ba)): return False;

		return True;

	def write_to_buf(self, ba):
		#x位置 single object_pos_t pos
		if(not isinstance(self.pos, share.object_pos_t)): return False;
		if(not self.pos.write_to_buf(ba)): return False;

		return True;

	def echo(self, print_hex=False, tabs=""):
		#x位置 single object_pos_t pos
		print tabs+r"======x位置 single object_pos_t pos";
		print tabs+"[pos]=>{";
		self.pos.echo(print_hex, tabs+"    ");
		print tabs+"}";

		return True;



#获取用户走路输出(广播)
class cli_walk_out:
	#x位置
	#object_pos_t pos;

	def read_from_buf(self, ba):
		#x位置 single object_pos_t pos
		self.pos = share.object_pos_t();
		if( not self.pos.read_from_buf(ba)): return False;

		return True;

	def write_to_buf(self, ba):
		#x位置 single object_pos_t pos
		if(not isinstance(self.pos, share.object_pos_t)): return False;
		if(not self.pos.write_to_buf(ba)): return False;

		return True;

	def echo(self, print_hex=False, tabs=""):
		#x位置 single object_pos_t pos
		print tabs+r"======x位置 single object_pos_t pos";
		print tabs+"[pos]=>{";
		self.pos.echo(print_hex, tabs+"    ");
		print tabs+"}";

		return True;



#获取用户说话输入
class cli_talk_peer_in:
	#对方ID
	#uint32_t peer_id;
	#聊天内容
	#std::string content;
	#聊天内容
	#std::vector<uint32_t> ccc;

	def read_from_buf(self, ba):
		#对方ID single uint32 peer_id
		self.peer_id = ba.read_uint32();
		if(self.peer_id == None): return False;
		#聊天内容 single string16 content
		self.content_len___ = ba.read_uint16();
		self.content = ba.read_string16(self.content_len___, 256);
		if(self.content == None): return False;
		#聊天内容 list uint32 ccc
		ccc_list_cnt__ = ba.read_uint32();
		if(ccc_list_cnt__ == None): return False;
		self.ccc = [];
		for i in range(ccc_list_cnt__):
			self.ccc.append(ba.read_uint32());
			if(self.ccc[i] == None): return False;

		return True;

	def write_to_buf(self, ba):
		#对方ID single uint32 peer_id
		if(not ba.write_uint32(self.peer_id)): return False;
		#聊天内容 single string16 content
		self.content_len__ = len(self.content);
		if(not ba.write_uint16(self.content_len__)): return False;
		if(not ba.write_string16(self.content, self.content_len__, 256)): return False;
		#聊天内容 list uint32 ccc
		ccc_list_cnt__ = len(self.ccc);
		ba.write_uint32(ccc_list_cnt__);
		for i in range(ccc_list_cnt__):
			if(not ba.write_uint32(self.ccc[i])): return False;

		return True;

	def echo(self, print_hex=False, tabs=""):
		#对方ID single uint32 peer_id
		print tabs+r"======对方ID single uint32 peer_id";
		print tabs+"[peer_id]=%s"%(str(self.peer_id));
		#聊天内容 single string16 content
		print tabs+r"======聊天内容 single string16 content";
		print tabs+"[content]=%s"%(str(self.content));
		#聊天内容 list uint32 ccc
		print tabs+r"======聊天内容 list uint32 ccc";
		print tabs+r"[ccc]=>{";
		for i in range(len(self.ccc)):
			print tabs+"    [%d]="%(i)+str(self.ccc[i]); 
		print tabs+"}\n";

		return True;



#获取用户说话输出(广播)
class cli_talk_peer_out:
	#对方ID
	#uint32_t peer_id;
	#聊天内容
	#std::string content;

	def read_from_buf(self, ba):
		#对方ID single uint32 peer_id
		self.peer_id = ba.read_uint32();
		if(self.peer_id == None): return False;
		#聊天内容 single string16 content
		self.content_len___ = ba.read_uint16();
		self.content = ba.read_string16(self.content_len___, 256);
		if(self.content == None): return False;

		return True;

	def write_to_buf(self, ba):
		#对方ID single uint32 peer_id
		if(not ba.write_uint32(self.peer_id)): return False;
		#聊天内容 single string16 content
		self.content_len__ = len(self.content);
		if(not ba.write_uint16(self.content_len__)): return False;
		if(not ba.write_string16(self.content, self.content_len__, 256)): return False;

		return True;

	def echo(self, print_hex=False, tabs=""):
		#对方ID single uint32 peer_id
		print tabs+r"======对方ID single uint32 peer_id";
		print tabs+"[peer_id]=%s"%(str(self.peer_id));
		#聊天内容 single string16 content
		print tabs+r"======聊天内容 single string16 content";
		print tabs+"[content]=%s"%(str(self.content));

		return True;



#获取用户说话输入
class cli_change_nick_in:
	#新昵称
	#char nick[16];

	def read_from_buf(self, ba):
		#新昵称 array char nick
		self.nick = ba.read_buf(16);
		if(self.nick == None): return False;

		return True;

	def write_to_buf(self, ba):
		#新昵称 array char nick
		ba.write_buf(self.nick, 16);

		return True;

	def echo(self, print_hex=False, tabs=""):
		#新昵称 array char nick
		print tabs+r"======新昵称 array char nick";
		tmp_str=str(self.nick);
		if(print_hex):tmp_str=show.bin2hex(tmp_str);
		print tabs+"[nick]=%s"%(tmp_str);

		return True;



#获取用户说话输出(广播)
class cli_change_nick_out:
	#新昵称
	#char nick[16];

	def read_from_buf(self, ba):
		#新昵称 array char nick
		self.nick = ba.read_buf(16);
		if(self.nick == None): return False;

		return True;

	def write_to_buf(self, ba):
		#新昵称 array char nick
		ba.write_buf(self.nick, 16);

		return True;

	def echo(self, print_hex=False, tabs=""):
		#新昵称 array char nick
		print tabs+r"======新昵称 array char nick";
		tmp_str=str(self.nick);
		if(print_hex):tmp_str=show.bin2hex(tmp_str);
		print tabs+"[nick]=%s"%(tmp_str);

		return True;

cmd_map={};
cmd_map[0x10001]=('cli_walk', cli_walk_in, cli_walk_out);
cmd_map[10002]=('cli_talk_peer', cli_talk_peer_in, cli_talk_peer_out);
cmd_map[10003]=('cli_change_nick', cli_change_nick_in, cli_change_nick_out);
