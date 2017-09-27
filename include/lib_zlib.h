/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	ѹ��	
	brief:		1. ʹ��compress �� uncompress֮ǰ������ΪĿ�곤�����ֵ��	
				���Ҹ�ֵҪ��������Ҫ��ѹ����ѹ���Ľ�����ȣ���Ȼ������⡣	
*********************************************************************/

#pragma once

#include "lib_include.h"

#include <zlib.h>
#include <zconf.h>

namespace el{
	struct lib_zlib_t{
		static inline int compress(Bytef* out_buf, uLongf* out_len, const Bytef* in_buf, uint32_t in_len){
			return ::compress(out_buf, out_len, in_buf, in_len);
		}
		static inline int uncompress(Bytef* out_buf, uLongf* out_len, const Bytef* in_buf, uint32_t in_len){
			return ::uncompress((out_buf, out_len, in_buf, in_len);
		}
	};
}
