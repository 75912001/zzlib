/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	�ڴ��
	brief:		�ڴ�ػ��࣬�̳и���������༰�������࣬ʹ��new/delete/new []/delete []��̬
	*             ����/�ͷŶ����ڴ�ʱ�������Ķ����ڴ�ء�\n
	*             ���磺\n
	*             class Obj : public MemPool; \n
	*             Obj* o = new Obj; \n
	*             delete o; \n
	*             Obj* oo = new Obj[100]; \n
	*             delete [] oo; \n
	*             �����new/delete�����ڲ����ڴ��������/�ͷ��ڴ档\n
	*             ע�⣺����glib�����Ա���ʱ����Ҫ����glib���Լ�����glib.h��·����\n
	*             ���磺-I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include/ -lglib-2.0
	*             ���̰߳�ȫ
*********************************************************************/

#pragma once

#include "lib_include.h"

#include <glib.h>

namespace el {
/*
// @brief �ڴ�ػ��࣬����glib�����Ա���ʱ����Ҫ����glib���Լ�����glib.h��·����
// ���磺-I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include/ -lglib-2.0

class MemPool {
public:
	virtual ~MemPool(){}
public:
	void* operator new(std::size_t n){
		void* p = g_slice_alloc(n);
		if (p) {
			return p;
		}

		throw std::bad_alloc();
	}
	void* operator new(std::size_t n, const std::nothrow_t& nt){
		return g_slice_alloc(n);
	}
	void  operator delete(void* p, std::size_t n){
		g_slice_free1(n, p);
	}

	void* operator new[](std::size_t n){
		return operator new(n);
	}
	void* operator new[](std::size_t n, const std::nothrow_t& nt){
		return operator new(n, nt);
	}
	void  operator delete[](void* p, std::size_t n){
		operator delete(p, n);
	}
};
*/
}

