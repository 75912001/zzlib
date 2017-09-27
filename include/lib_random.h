/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	随机数	
	brief:		OK
	linux: The largest number rand will return (same as INT_MAX). 
*********************************************************************/

#pragma once

#include "lib_include.h"
#include "lib_util.h"

namespace el{

	struct lib_random_t{
	public:
		/**
		* @brief 产生给定范围内的随机数。如果需要更加随机，可以在程序起来的时候调用一次srand()。	
		* @param min 随机出来的最小数。	
		* @param max 随机出来的最大数。	
		* @return min和max之间的随机数，包括min和max。//return [min,max]
		linux: The largest number rand will return (same as INT_MAX). 
		*/
		static inline int random(int min, int max){
			return (::rand()%(max-min+1))+min;
		}
		static inline uint32_t random_u32(uint32_t min, uint32_t max){
			return (::rand()%(max-min+1))+min;
		}

		static inline uint64_t random_u64(uint64_t min, uint64_t max){
			return (::rand()%(max-min+1))+min;
		}
		static inline int64_t random_64(int64_t min, int64_t max){
			return (::rand()%(max-min+1))+min;
		}
		//************************************
		// @brief:	产生给定范围内的随机数。精度小数点后6位	
		// return:	float
		// parameter:	float min
		// parameter:	float max
		//************************************
		static inline float random_f(float min = 0.000000f, float max = 1.000000f){
			static const float EPSILON = 1000000.0f;
			float r = (float)((float)(lib_random_t::random_64((int64_t)(min * EPSILON), (int64_t)(max * EPSILON))) / EPSILON);
			if (r < min){
				return min;
			}
			if (r > max){
				return max;
			}
			return r;
		}
		/* Return a random float from 0.000001 to 100.000000 */
		static inline float random_chance_f(){
			return (float)lib_random_t::random_f(0.000001f, 100.000000f);
		}

		/**
		* @brief 根据输入的字符串，生成一个32位的哈希值	
		* @param p 需要生成哈希值的字符串	
		* @return 字符串对应的哈希值	
		*/
		static inline int hash(const char* p){
			int h = 0;
			while (*p) {
				h = h * 11 + (*p << 4) + (*p >> 4);
				p++;
			}
			return h;
		}
		/**
		* @brief 生成随机出的索引	
		* @param weight_vec 权重数组,对象必须有uint32_t get_weight();来获取权重	
		* @param sum_weight 权重的和,为权重数组中的和	
		* @return 索引	
		*/
		template <typename T>
		static inline uint32_t random_weight(std::vector<T>& weight_vec, uint32_t sum_weight){
			uint32_t i = 0;
			FOREACH(weight_vec, it){
				T& w = *it;
				uint32_t r = lib_random_t::random_u32(1, sum_weight);
				if (r <= w.get_weight()){
					break;
				}
				sum_weight -= w.get_weight();
				i++;
			}
			return i;
		}

		/**
		* @brief 生成随机出的索引	
		* @param weight_vec 权重数组,对象必须有uint32_t get_weight();来获取权重	
		* @param sum_weight 权重的和,为权重数组中的和	
		* @return 索引	
		*/
		template <typename T>
		static inline uint32_t random_weight(std::map<uint32_t, T>& weight_map, uint32_t sum_weight){
			FOREACH(weight_map, it){
				T w = it->second;
				uint32_t r = lib_random_t::random_u32(1, sum_weight);
				if (r <= w->get_weight()){
					return it->first;
				}
				sum_weight -= w->get_weight();
			}
			return 0;
		}
	protected:
		
	private:
		lib_random_t(const lib_random_t& cr);
		lib_random_t& operator=(const lib_random_t& cr);
	};

}//end namespace el
