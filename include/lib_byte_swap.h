/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	LITTLE ENDIAN <-> BIG ENDIAN
	用法：		val = EL_BYTE_SWAP(val);
				val = el::g_bswap(val);可以对任意大小的整数(float)类型进行字节序转换。
	brief:		ok
*********************************************************************/
#pragma once

#include "lib_include.h"
#include "lib_platform.h"

//#define EL_USE_BYTE_SWAP
//# 

#ifdef EL_USE_BYTE_SWAP
	namespace {
		inline int8_t bswap(int8_t x){
			return x;
		}

		inline uint8_t bswap(uint8_t x){
			return x;
		}

		inline int16_t bswap(int16_t x){
			return bswap_16(x);
		}

		inline uint16_t bswap(uint16_t x){
			return bswap_16(x);
		}

		inline int32_t bswap(int32_t x){
			return bswap_32(x);
		}

		inline uint32_t bswap(uint32_t x){
			return bswap_32(x);
		}

		inline float bswap(float x){
			int i;
			::memcpy(&i, &x, sizeof(i));
			i = bswap_32(i);
			::memcpy(&x, &i, sizeof(i));
			return x;
		}

		inline int64_t bswap(int64_t x){
			return bswap_64(x);
		}

		inline uint64_t bswap(uint64_t x){
			return bswap_64(x);
		}

	#if __WORDSIZE == 32
		inline long bswap(long x){
			return bswap_32(x);
		}

		inline unsigned long bswap(unsigned long x){
			return bswap_32(x);
		}
	#endif
	}

	//////////////////////////////////////////////////////////////////////////
	//接口
	namespace el{
		//************************************
		// @brief:	直接反转字节序
		//************************************
		template <typename T>
		inline T g_bswap(T value){
			return bswap(value);
		}
	}//end namespace el


	#define EL_BYTE_SWAP(__value__) (el::g_bswap(__value__))
#else
	#define EL_BYTE_SWAP(__value__) (__value__)
#endif
