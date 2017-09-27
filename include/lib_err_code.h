/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	各种错误码定义
	brief:		ok
*********************************************************************/
#pragma once

namespace el{
	namespace ERR_DBPROXY{//[0xFFFF0000 - 0xFFFF0FFF]
		enum CODE{
			//系统范围[0xFFFF0000 - 0xFFFF02FF]
			MIN										= 0x0FFF0000,
			TIME_OUT								= 0x0FFF0002,//超时,未收到DB返回
			DB_DISCONNECT							= 0x0FFF0003,//未能连接对应的DB.
			DB_CONNECT_ADD							= 0x0FFF0004,//添加DB服务器时出错
			GEN_PROTO_KEY							= 0x0FFF0005,//生成协议KEY时出错
			MAX										= 0x0FFF02FF,
			//逻辑范围预留[0xFFFF0300 - 0xFFFF0FFF]
		};
	}

	namespace ERR_DB{//[0xFFFF1000 - 0xFFFF1FFF]
		enum CODE{
			//系统范围[0xFFFF1000 - 0xFFFF12FF]
			MIN										= 0x0FFF1000,
			SYS										= 0x0FFF10EF,//数据库出错
			ITEM_NOT_ENOUGH							= 0x0FFF10B4,//道具数量不足
			KEY_INEXIST								= 0x0FFF10EC,//键值不存在
			KEY_EXIST								= 0x0FFF10ED,//KEY已经存在了
			MAX										= 0x0FFF12FF,
			//逻辑范围预留[0xFFFF1300 - 0xFFFF1FFF]
		};
	}
	namespace ERR_SYS{//[0xFFFF2000 - 0xFFFF2FFF]
		enum CODE{
			//系统范围[0xFFFF2000 - 0xFFFF22FF]
			MIN										= 0x0FFF2000,
			UNDEFINED_CMD							= 0x0FFF2001,//命令号未定义
			PROTO_LEN								= 0x0FFF2002,//协议长度不符
			OUT_OF_RANGE							= 0x0FFF2003,//越界:不在类型的限定范围.(如某一字段只允许[0,1],却填了2)
			DISCONNECT								= 0x0FFF2004,//无链接
			DISCONNECT_PEER							= 0xFFFF2005,//断开对方连接
			CMD_OUT_OF_RANGE						= 0x0FFF2006,//命令号超出处理范围
			PROTO_READ								= 0x0FFF2007,//协议解析出错
			MAX										= 0x0FFF22FF,
			//逻辑范围预留[0xFFFF2300 - 0xFFFF2FFF]
		};
	}
}
