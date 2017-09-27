/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	ok
	brief:		linux version
*********************************************************************/

#pragma once

#include "lib_include.h"

//linux version code 大于等于 内核版本号
#define EL_IS_LINUX_VERSION_CODE_GE(__code1__, __code2__, __code3__) \
	(LINUX_VERSION_CODE >= KERNEL_VERSION((__code1__), (__code2__), (__code3__)))

