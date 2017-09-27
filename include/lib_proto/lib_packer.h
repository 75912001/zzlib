/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	���ڶ����ݰ����н�������߰����ݴ�������ݰ�
	brief:		OK
*********************************************************************/

#pragma once

#include "lib_include.h"
#include "lib_byte_swap.h"

namespace el{
	class lib_packer_t{
	public:
		/**
		* @brief ��valת���ֽ��򣬲��Ҵ����(uint8_t*)pkg + idx��Ȼ������idx��ֵ��idx������Ϊval����ռ�õ��ֽ�����\n
		*        ����val��������uint32_t����idx��ֵ����4��
		* @param pkg ���ڱ������ݰ����ڴ�顣
		* @param val ��Ҫ�����pkg���ֵ��
		* @param idx ָʾvalӦ�ñ������pkg��ƫ����Ϊidx���ڴ��С�
		* @return void
		*/		
		template <typename T>
		static inline void pack(void* pkg, T val, uint32_t& idx){
			*(reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(pkg) + idx)) = EL_BYTE_SWAP(val);
			idx += (uint32_t)sizeof(val);
		}

		template <typename T>
		static inline void pack(void* pkg, T val){
			*(reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(pkg))) = EL_BYTE_SWAP(val);
		}

		/**
		* @brief ���ڴ��val�����(uint8_t*)pkg + idx��Ȼ������idx��ֵ��idx������Ϊlen��
		* @param pkg ���ڱ������ݰ����ڴ�顣
		* @param val ��Ҫ�����pkg����ڴ�顣
		* @param len �ڴ��val�Ĵ�С��byte����
		* @param idx ָʾvalӦ�ñ������pkg��ƫ����Ϊidx���ڴ��С�
		* @return void
		*/
		static inline void pack(void* pkg, const void* val, std::size_t len, uint32_t& idx){
			::memcpy(reinterpret_cast<uint8_t*>(pkg) + idx, val, len);
			idx += (uint32_t)len;
		}

		/**
		* @brief ��ת��val���ֽ��򣬲��Ҵ����(uint8_t*)pkg + idx��Ȼ������idx��ֵ��idx������Ϊval����ռ�õ��ֽ�����\n
		*        ����val��������uint32_t����idx��ֵ����4��h == host��
		* @param pkg ���ڱ������ݰ����ڴ�顣
		* @param val ��Ҫ�����pkg���ֵ��
		* @param idx ָʾvalӦ�ñ������pkg��ƫ����Ϊidx���ڴ��С�
		* @return void
		*/
		template <typename T>
		static inline void pack_raw(void* pkg, T val, uint32_t& idx){
			*(reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(pkg) + idx)) = val;
			idx += (uint32_t)sizeof(val);
		}

		/**
		 * @brief ��(uint8_t*)pkg + idx�����val������ת���ֽ���Ȼ������idx��ֵ��idx������Ϊval����ռ�õ��ֽ�����\n
		 *        ����val��������uint32_t����idx��ֵ����4��
		 * @param pkg ��Ҫ������ڴ�顣
		 * @param val ���������ݱ��浽val�
		 * @param idx ָʾӦ�ð�pkg��ƫ������idx��ʼ�����ݽ����val�С�
		 * @return void
		 */
		template <typename T>
		static inline void unpack(const void* pkg, T& val, uint32_t& idx){
			val = EL_BYTE_SWAP(*(reinterpret_cast<const T*>(reinterpret_cast<const uint8_t*>(pkg) + idx)));
			idx += (uint32_t)sizeof(val);
		}

		/**
		 * @brief ��(uint8_t*)pkg + idx�����val��Ȼ������idx��ֵ��idx������Ϊlen��
		 * @param pkg ��Ҫ������ڴ�顣
		 * @param val ���������ݱ��浽�ڴ��val�
		 * @param len �ڴ��val�Ĵ�С��byte����
		 * @param idx ָʾӦ�ð�pkg��ƫ������idx��ʼ�����ݽ����val�С�
		 * @return void
		 */
		static inline void unpack(const void* pkg, void* val, std::size_t len, uint32_t& idx){
			::memcpy(val, reinterpret_cast<const uint8_t*>(pkg) + idx, len);
			idx += (uint32_t)len;
		}

		/**
		 * @brief ��(uint8_t*)pkg + idx�����val������ת���ֽ���Ȼ������idx��ֵ��idx������Ϊval����ռ�õ��ֽ�����\n
		 *        ����val��������uint32_t����idx��ֵ����4��
		 * @param pkg ��Ҫ������ڴ�顣
		 * @param val ���������ݱ��浽val�
		 * @param idx ָʾӦ�ð�pkg��ƫ������idx��ʼ�����ݽ����val�С�
		 * @return void
		 */
		template <typename T>
		static inline void unpack_raw(const void* pkg, T& val, uint32_t& idx){
			val = *(reinterpret_cast<const T*>(reinterpret_cast<const uint8_t*>(pkg) + idx));
			idx += (uint32_t)sizeof(val);
		}
	protected:
		
	private:
		lib_packer_t(const lib_packer_t& cr);
		lib_packer_t& operator=(const lib_packer_t& cr);
	};

}//end namespace el


