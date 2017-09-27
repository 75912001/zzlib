/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	���ִ����붨��
	brief:		ok
*********************************************************************/
#pragma once

namespace el{
	namespace ERR_DBPROXY{//[0xFFFF0000 - 0xFFFF0FFF]
		enum CODE{
			//ϵͳ��Χ[0xFFFF0000 - 0xFFFF02FF]
			MIN										= 0x0FFF0000,
			TIME_OUT								= 0x0FFF0002,//��ʱ,δ�յ�DB����
			DB_DISCONNECT							= 0x0FFF0003,//δ�����Ӷ�Ӧ��DB.
			DB_CONNECT_ADD							= 0x0FFF0004,//���DB������ʱ����
			GEN_PROTO_KEY							= 0x0FFF0005,//����Э��KEYʱ����
			MAX										= 0x0FFF02FF,
			//�߼���ΧԤ��[0xFFFF0300 - 0xFFFF0FFF]
		};
	}

	namespace ERR_DB{//[0xFFFF1000 - 0xFFFF1FFF]
		enum CODE{
			//ϵͳ��Χ[0xFFFF1000 - 0xFFFF12FF]
			MIN										= 0x0FFF1000,
			SYS										= 0x0FFF10EF,//���ݿ����
			ITEM_NOT_ENOUGH							= 0x0FFF10B4,//������������
			KEY_INEXIST								= 0x0FFF10EC,//��ֵ������
			KEY_EXIST								= 0x0FFF10ED,//KEY�Ѿ�������
			MAX										= 0x0FFF12FF,
			//�߼���ΧԤ��[0xFFFF1300 - 0xFFFF1FFF]
		};
	}
	namespace ERR_SYS{//[0xFFFF2000 - 0xFFFF2FFF]
		enum CODE{
			//ϵͳ��Χ[0xFFFF2000 - 0xFFFF22FF]
			MIN										= 0x0FFF2000,
			UNDEFINED_CMD							= 0x0FFF2001,//�����δ����
			PROTO_LEN								= 0x0FFF2002,//Э�鳤�Ȳ���
			OUT_OF_RANGE							= 0x0FFF2003,//Խ��:�������͵��޶���Χ.(��ĳһ�ֶ�ֻ����[0,1],ȴ����2)
			DISCONNECT								= 0x0FFF2004,//������
			DISCONNECT_PEER							= 0xFFFF2005,//�Ͽ��Է�����
			CMD_OUT_OF_RANGE						= 0x0FFF2006,//����ų�������Χ
			PROTO_READ								= 0x0FFF2007,//Э���������
			MAX										= 0x0FFF22FF,
			//�߼���ΧԤ��[0xFFFF2300 - 0xFFFF2FFF]
		};
	}
}
