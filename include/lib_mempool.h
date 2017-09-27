/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	内存池
	brief:		内存池基类，继承该类的派生类及其派生类，使用new/delete/new []/delete []动态
	*             分配/释放对象内存时，操作的都是内存池。\n
	*             例如：\n
	*             class Obj : public MemPool; \n
	*             Obj* o = new Obj; \n
	*             delete o; \n
	*             Obj* oo = new Obj[100]; \n
	*             delete [] oo; \n
	*             这里的new/delete都是在操作内存池来分配/释放内存。\n
	*             注意：依赖glib，所以编译时，需要连接glib库以及加上glib.h的路径。\n
	*             例如：-I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include/ -lglib-2.0
	*             非线程安全
*********************************************************************/

#pragma once

#include "lib_include.h"

#include <glib.h>

namespace el {
/*
// @brief 内存池基类，依赖glib，所以编译时，需要连接glib库以及加上glib.h的路径。
// 例如：-I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include/ -lglib-2.0

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

