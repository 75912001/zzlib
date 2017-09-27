/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	����	
	brief:		ok
*********************************************************************/

#pragma once

#include "lib_include.h"
#include "lib_assistant/lib_class.h"

#ifndef likely
	#ifdef WIN32
		#define likely(x) (x)
	#else
		#define likely(x)  __builtin_expect(!!(x), 1)
	#endif
#endif

#ifndef unlikely
	#ifdef WIN32
		#define unlikely(x) (x)
	#else
		#define unlikely(x)  __builtin_expect(!!(x), 0)
	#endif
#endif

//////////////////////////////////////////////////////////////////////////
//�ڴ�ɾ��	
#define SAFE_DELETE(__p__){\
		delete (__p__);\
		(__p__) = NULL;\
	}

#define SAFE_DELETE_ARR(__p__){\
		delete [](__p__);\
		(__p__) = NULL;\
	}

#define SAFE_FREE(__p__){\
		free (__p__);\
		(__p__) = NULL;\
	}

//////////////////////////////////////////////////////////////////////////
//����ɱ�ϵͳ�жϵĺ�������EINTRʱѭ������.(��������ֵ����Ϊint)	
//example:int nRes = HANDLE_EINTR(::close(s));	
#ifdef WIN32	
	#define HANDLE_EINTR(x) (x)
#else
	#define HANDLE_EINTR(x) ({\
		decltype(x) __eintr_code__;\
		do {\
		__eintr_code__ = x;\
		} while(unlikely(__eintr_code__ < 0 && EINTR == errno));\
		__eintr_code__;\
		})
#endif

#define SUCC 0
#define ERR -1
#define FAIL -1
#define INVALID_FD -1

//Э��	
#define NOTE//��Ҫע��	
#define TODO//��Ҫ���	
#define TO_DEBUG//Ϊ�˵���	
#define OUT//�������	
#define IN//�������	
#define TO_TEST 0

//�ж�	
#define	IS_NULL(__p__)	(NULL == (__p__))
#define	IS_ZERO(__p__)	(0 == (__p__))
#define IS_SUCC(__p__) (SUCC == (__p__))
#define IS_ERR(__p__) (ERR == (__p__))
#define IS_FAIL(__p__) (FAIL == (__p__))

#define FOREACH(container, it) \
	for(auto it = (container).begin(); (it) != (container).end(); ++(it))

#define FOREACH_P(__p__, it) \
	for(auto it = (*__p__).begin(); (it) != (*__p__).end(); ++(it))

#define FOREACH_R(container, it) \
	for(auto it = (container).rbegin(); (it) != (container).rend(); ++(it))

#define FOREACH_PB(container, index) \
	auto __cnt__ = container##_size(); \
	for(auto index = 0; index < __cnt__; index++)

//����std::map�е�valָ��,����KEY������	
#define FIND_MAP_SECOND_POINTER_RETURN_POINTER(container, key) \
	auto __it__ = (container).find(key);\
	if ((container).end() != __it__){\
		return __it__->second;\
	}\
	return NULL;

//����std::map�е�val�����ָ��,����KEY������	
#define FIND_MAP_SECOND_VALUE_RETURN_POINTER(container, key) \
	auto __it__ = (container).find(key);\
		if ((container).end() != __it__){\
		return &__it__->second;\
	}\
	return NULL;
		
//��̬����	
//c11
//static_assert(0==1, "000");
//��C11�������	
#define ASSERT_STATIC(__e__) \
	do {\
		enum {__assert_static__ = 1/(__e__)};\
	}while(0)

#define TO_STRING(__str__) #__str__

namespace el{
	//************************************
	// Brief:     ���ַ���ת��Ϊ��������	
	// Returns:   void	
	// Parameter: T & value	��������	
	// Parameter: const std::string & s	��ת�����ַ���	
	// notice: Parameter: const std::string & s	
	// 1.(�м��пո�Ļ�,�����ÿո�ָ�,ֻȡ�ո�ǰ�Ĳ���)	
	//convert_from_string(x, "");	���ַ���,ת������0		
	//************************************
	template <typename T> 
	inline void convert_from_string(T &value, const std::string &s) {
		if (s.empty()){
			std::string ts = "0";
			std::stringstream ss(ts);
			ss >> value;
		} else {
			std::stringstream ss(s);
			ss >> value;
		}
	}

	template <typename T> 
	std::string convert_to_string(T value) {
		std::stringstream ss;
		ss << value;
		return ss.str();
	}

	//************************************
	// Brief:     ��������,��ȡ�������	
	// Returns:   uint32_t	�������	
	// Parameter: const T & name ��������	
	//************************************
	template <typename T>
	inline uint32_t get_arr_num(const T& name){
		return sizeof(name)/sizeof(name[0]);
	};

	//************************************
	// Brief:     �и��ַ�(�Ե�������Ϊ���)like: 1,2,3 or 1;2;3 or aa/b/cc ...	
	// Returns:   void	
	// Parameter: std::vector<T> & result �и��Ľ��	
	// Parameter: std::string & src	���и���ַ���	
	// Parameter: char tag	�и����ݷ���	
	//************************************
	template <typename T>
	inline void g_cat_string(std::vector<T>& result, const std::string& src, char tag = ','){
		std::stringstream ss(src);        
		std::string sub_str;        
		while(std::getline(ss, sub_str, tag)){
			//��tagΪ����ָ�str������	
			T t;
			convert_from_string(t, sub_str);
			result.push_back(t);
		}
	};

	//std::string str="1,2;3,4;5,6;7,8";
	//std::vector<std::vector<uint32_t>> para_vec = el::g_cat_string<uint32_t>(production, ';', ',');
	template <typename T>
	inline std::vector<std::vector<T>> g_cat_string(const std::string& src, char tag_main, char tag_minor){
		std::vector<std::vector<T>> para_vec;

		std::vector<std::string> str1;
		el::g_cat_string(str1, src, tag_main);
		if (!str1.empty()){
			FOREACH(str1, it){
				std::string s = *it;
				std::vector<T> para;
				
				el::g_cat_string(para, s, tag_minor);
				para_vec.push_back(para);
			}
		}
		return para_vec;
	}

	template <typename T>
	inline void g_concat_string(std::string& dist, const std::vector<T>& source, char tag) {
		for (auto it = source.begin(); source.end() != it; ++it) {
			dist += el::convert_to_string(*it);
			dist += tag;
		}
		
		if (!source.empty()) {
			dist.pop_back();
		}
	}

	/**
	* @brief	�ָ�(separator��ÿ���ַ���������Ϊ�ָ���)	
	* @param	const std::string & src	
	* @param	const std::string & separator	
	* @param	std::vector<T> & dest	
	Example	
	std::string src = "1jf456j89";	
	std::vector<std::string> v;	
	split(src, "jf",v);	
	1	
	456	
	89	
	*/
	template <typename T>
	inline void g_split(const std::string& src, const std::string& separator, std::vector<T>& dest){
		std::string str = src;
		std::string substring;
		std::string::size_type start = 0, index;

		do{
			index = str.find_first_of(separator, start);
			if (index != std::string::npos){    
				substring = str.substr(start, index-start);

				T t;
				convert_from_string(t, substring);
				dest.push_back(t);

				start = str.find_first_not_of(separator, index);
				if (start == std::string::npos){
					return;
				}
			}
		}while(index != std::string::npos);

		//the last token
		substring = str.substr(start);
		if ("" != substring){
			T t;
			convert_from_string(t, substring);
			dest.push_back(t);
		}
	}

	//ɾ��(�ǿ�)ָ�벢�ÿ�	
	//ʹ�÷���://for_each(vector.begin(),vector.end(),lib_delete_ptr());	
	struct lib_delete_ptr_t{
		template <typename T>
		void operator() (const T* ptr) const{
			if (NULL != ptr){
				SAFE_DELETE(ptr);
			}
		}
	};

	//ɾ��(�ǿ�)map��valλ���ϵ�ָ�벢�ÿ�	
	//�÷�://for_each(map.begin(), map.end(), lib_delete_pair_t());	 
	struct lib_delete_pair_t{
		template <typename Ty1, typename Ty2>
		void operator() (std::pair<Ty1, Ty2> &ptr) const {
			if (NULL != ptr.second){
				SAFE_DELETE(ptr.second);
			}		
		}
	};

	//sort,����(��->С)	
	struct lib_desc_sort_t{
		uint32_t level;
		bool operator < (const lib_desc_sort_t& r) const {
			return level > r.level;
		}
	};

	/**
	 * @brief	�Ƿ�С���ֽ���	
	 * @return	bool	
	 */
	static inline bool is_little_endian(){
		union ut{
			short s;
			char c[2];
		};

		ut u;
		u.s = 0x1234;
		if (0x34 == u.c[0] && 0x12 == u.c[1]){
			return true;
		}
		return false;
	}

	/**
	* @brief	��װ����16����	
	* max_len: ����������	
	*/
	static inline void bin2hex(std::string& dst, char* src, uint32_t len, uint32_t max_len = 8000){
		unsigned char hex;
		unsigned char c;
		static const unsigned char c_10 = 10;
		static const unsigned char c_A = 'A';
		static const unsigned char c_0 = '0';
		for(uint32_t i=0;i<len;i++){
			hex=(unsigned char)(src[i]>>4);
			c = (hex < c_10) ? (unsigned char)(c_0 + hex) : (unsigned char)(c_A - c_10 + hex);
			dst += c;

			hex = ((unsigned char)src[i]) & 0x0F;
			c = (hex < c_10) ? (unsigned char)(c_0 + hex) : (unsigned char)(c_A- c_10 + hex);
			dst += c;

			dst += ' ';
		}
		if (0 != max_len && dst.size() >= max_len){
			dst = dst.substr(0, max_len);
			dst += "xxxxxx";
		}
	}

	static inline uint64_t atoi_safe(char* str){
#ifdef WIN32
		if (NULL != str){
			uint64_t v = 0;
			std::string s(str);
			convert_from_string(v, s);
			return v;
		}
		return 0;
#else
		return (str!= NULL ? ::atoll(str) : 0);
#endif
	}

	//return 1  :f1 > f2	
	//return 0  :f1 == f2	
	//retrun -1 :f1 < f2	
	static inline int float_compare(double f1, double f2){
		static const double epsilon = FLT_EPSILON;//0.000001;	
		static const double fepsilon = -FLT_EPSILON;//-0.000001;	

		if (f1 - f2 < fepsilon){
			return -1;
		}
		if (f1 - f2 > epsilon){
			return 1;
		}
		return 0;
	}

	static inline uint32_t g_hash_str(const char * key ){
		register unsigned int h;
		register unsigned char *p; 
		for(h=0, p = (unsigned char *)key; *p ; p++)
			h = 31 * h + *p; 
		return h;
	}

	static inline char* g_set_space_end(char* src, uint32_t len){
		uint32_t i=0;
		while (i<len &&  src[i] !='\0' ){
			i++;
		}

		for (;i<len;i++){
			src[i]=' ';
		}
		return src ;
	}

	static inline uint64_t g_make_guid_64(uint32_t h, uint32_t l){
		uint64_t guid = 0;
		guid |= h;
		guid = (guid << 32);
		guid |= l;
		return guid;
	}

	// ��ȡ�������������	 	
	//1,3,4,5,7,8,9,10  =>  4
	static inline uint32_t g_serial_max_cnt(const std::set<uint32_t>& v){
		uint32_t max_cnt = 0;
		uint32_t cnt = 1;
		FOREACH(v, it){
			uint32_t i = *it;
			if (v.end() == v.find(i+1)){
				cnt = 1;
			} else {
				cnt++;
			}
			if (max_cnt < cnt){
				max_cnt = cnt;
			}
		}
		return max_cnt;
	}
	//////////////////////////////////////////////////////////////////////////
	//Ȩ��	

// 	bool g_weight(uint32_t sum){
// 
// 	}

	//////////////////////////////////////////////////////////////////////////
	

	/**
	 * @brief ����������limit���Ʒ�Χ�ڵ����к�(������0)��ɾ��֮������ظ�ʹ��	
	 */
	struct lib_seq_gen_t
	{
	public:
		lib_seq_gen_t(uint32_t limit = UINT_MAX) : m_limit(limit) {
			this->curr_seq = 0;
		}
		/**
		 * @brief ѡȡһ�����к�	
		 * @return ���к�	
		 */
		uint32_t gen() {
// 			while (0 == this->curr_seq || this->seqs.end() != this->seqs.find(this->curr_seq)){
// 				this->curr_seq = (this->curr_seq + 1) % m_limit;
// 			}

			this->curr_seq = (this->curr_seq + 1) % m_limit;
			
			this->seqs.insert(this->curr_seq);
			return this->curr_seq;
		}
		/**
		 * @brief �������к�	
		 * @param seq Ҫ���յ����к�	
		 */
		void del(uint32_t seq) {
			this->seqs.erase(seq);
		}
		void clear(){
			this->seqs.clear();
		}
	private:
		std::set<uint32_t> seqs;
		uint32_t curr_seq;
		const uint32_t m_limit;
	};
	
	template <typename T_ELEM_KEY, typename T_ELEM>
	class lib_elem_mgr_base_t
	{
	public:
		typedef std::map<T_ELEM_KEY, T_ELEM> KEY_ELEM_MAP;
	public:
		lib_elem_mgr_base_t(){}
		virtual ~lib_elem_mgr_base_t(){}
		T_ELEM* find(const T_ELEM_KEY& elem_key){
			FIND_MAP_SECOND_POINTER_RETURN_POINTER(this->key_elem_map, elem_key);
		}

		int add(const T_ELEM_KEY& elem_key, T_ELEM elem){
			if (!this->key_elem_map.insert(std::make_pair(elem_key, elem)).second){
				return ERR;
			}
			return SUCC;
		}

		int del(const T_ELEM_KEY& elem_key){
			if (0 == this->key_elem_map.erase(elem_key)){
				return ERR;
			}
			return SUCC;
		}
	protected:
		KEY_ELEM_MAP key_elem_map;
	};
}
