/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	压缩	
	brief:		1. 使用compress 与 uncompress之前，必须为目标长度设初值，	
				而且该值要大于你需要解压或者压缩的结果长度，不然会出问题。	
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
