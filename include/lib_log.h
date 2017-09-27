/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	ok
	brief:		���ڼ�¼��־��һ����9����־�ȼ���
	ע�⣬ÿ����־���ܳ���8000�ֽڡ�
	����������ʱ�����LOG_USE_SYSLOG���������syslog����¼��־��ʹ�õ�facility��LOG_USER��
*********************************************************************/

#pragma once

#ifdef WIN32
#else
//ֻ��linux��Ч ʹ���߳�
// Ŀǰʹ���̵߳Ļ�Ч�ʱȲ�ʹ�û���,Ӧ������NEW �� delete ,�����Ĺ���.
// ��ʹ���߳� ʹ�� 99�� ��ӡ10004990����־ 101060��/��(144927��/��, 4G�ڴ�)
// ʹ���߳�  ʹ�� 151�� ��ӡ10004990����־  66258��/��
//#define EL_USE_LOG_THREAD
#endif

#include "lib_include.h"
#include "lib_util.h"
#include "lib_thread.h"
#include "lib_platform.h"

namespace el{
#ifdef EL_USE_LOG_THREAD
	class lib_log_thread_t : public lib_thread_t{
	public:
		lib_log_thread_t();
		virtual ~lib_log_thread_t();
		virtual void do_work_fn(void* data);
		virtual int do_work_end(void* data);
		void add_event(int fd, const char* data, uint32_t len);
		//************************************
		// @brief:	����δ��ӡ����־����
		// return:	void
		//************************************
		void clear_log_buf_list();
	protected:
	private:
		struct log_buf_t {
			int fd;
			std::string buf;
		};
		typedef std::list<log_buf_t*> LOG_BUF_LIST;
		LOG_BUF_LIST log_buf_list;
		class lib_lock_mutex_t* lock;

		lib_log_thread_t(const lib_log_thread_t& cr);
		lib_log_thread_t& operator=(const lib_log_thread_t& cr);
	};
#else
	class lib_log_thread_t
	{
	public:
		lib_log_thread_t(){}
		virtual ~lib_log_thread_t(){}
		int start(void* pdata = NULL){return 0;}
		int stop(){return 0;}
		void clear_log_buf_list(){}
	protected:
		
	private:
		lib_log_thread_t(const lib_log_thread_t& cr);
		lib_log_thread_t& operator=(const lib_log_thread_t& cr);
	};
#endif

	class lib_log_t : public lib_log_thread_t{
	public:
		/**
		 * @enum  E_LOG_LEVEL
		 * @brief ��־�ȼ�
		 */
		enum E_LOG_LEVEL {
			/*! system is unusable -- 0 */
			E_LOG_LEVEL_EMERG = 0,
			/*! action must be taken immediately -- 1 */
			E_LOG_LEVEL_ALERT = 1,//��ɫ
			/*! critical conditions -- 2 */
			E_LOG_LEVEL_CRIT = 2,//����
			/*! error conditions -- 3 */
			E_LOG_LEVEL_ERROR = 3,//3,
			/*! warning conditions  -- 4 */
			E_LOG_LEVEL_WARNING = 4,//4,
			/*! normal but significant condition -- 5 */
			E_LOG_LEVEL_NOTICE = 5,//5,
			/*! informational -- 6 */
			E_LOG_LEVEL_INFO = 6,//6,
			/*! debug-level messages -- 7 */
			E_LOG_LEVEL_DEBUG = 7,
			/*! trace-level messages -- 8����������˺�LOG_USE_SYSLOG����log_lvl_trace==log_lvl_debug */
		#ifndef LOG_USE_SYSLOG
			E_LOG_LEVEL_TRACE = 8,//,
		#else
			E_LOG_LEVEL_TRACE = E_LOG_LEVEL_DEBUG,
		#endif
			E_LOG_LEVEL_MAX = 9,
		};
		/**
		 * @enum  log_dest
		 * @brief ��־�����ʽ
		 */
		enum E_LOG_DEST {//ʹ��BITλ��д������.
			/*! ���������Ļ  */
			E_LOG_DEST_TERMINAL	= 1,
			/*! ��������ļ� */
			E_LOG_DEST_FILE		= 2,
			/*! ���������Ļ��Ҳ������ļ� */
			E_LOG_DEST_BOTH		= 3
		};
	public:
		lib_log_t();
		virtual ~lib_log_t();
		/**
		* @brief ��ʼ����־��¼���ܡ���ʱ�����ڴ����µ���־�ļ���
		* @param dir ��־����Ŀ¼�������0��������Ļ�������־��
		* @param lvl ��־����ȼ����������Ϊe_lvl_notice����e_lvl_notice���ϵȼ�����־���������
		* @param pre_name ��־�ļ���ǰ׺��
		* @param logtime ÿ����־�ļ�����logtime���ӵ���־������ܳ���30000000���ӡ�
		*				����logtimeΪ15����ÿСʱ����4����־�ļ���ÿ���ļ�����15������־��
		* @see set_dest
		*
		* @return �ɹ�����0��ʧ�ܷ���-1��
		*/
		int init(const char* dir, E_LOG_LEVEL lvl, const char* pre_name, uint32_t logtime);

		/**
		* @brief ����log_init��ʼ����־���ܺ󣬿��Ե��øú�����̬������־�������ʽ��
		*		���������set_dest�Ļ����������ʽΪsetup_by_timeʱȷ���ķ�ʽ��
		*		ע�⣺������setup_by_timeʱָ������־����Ŀ¼���ſ��Ե��øú�����
		* @param dest ��־�����ʽ
		*
		* @see setup_by_time
		*/
		void set_dest(E_LOG_DEST dest);

		#ifdef __GNUC__
			#define LOG_CHECK_FMT(a,b) __attribute__((format(printf, a, b)))
		#else
			#define LOG_CHECK_FMT(a,b)
		#endif
		void boot(int ok, int space, const char* fmt, ...) LOG_CHECK_FMT(4, 5);
		void write(int lvl, uint32_t key, const char* fmt, ...) LOG_CHECK_FMT(4, 5);
		void write_sys(int lvl, const char* fmt, ...) LOG_CHECK_FMT(3, 4);
	protected:
	private:
		struct fds_t {
			int		seq;//˳���
			int		opfd;//�ļ�FD
			int		day;//һ���е�����
			std::string base_file_name;//�����ļ���
			fds_t();
			virtual ~fds_t();
			void init();
		};
		fds_t fds_info[el::lib_log_t::E_LOG_LEVEL_MAX];
		el::lib_log_t::E_LOG_LEVEL  level;	  // default log level
		std::string dir_name;//��־Ŀ¼
		std::string file_pre_name;//�ļ���ǰ׺
		el::lib_log_t::E_LOG_DEST log_dest;//��־���Ŀ��
		uint32_t logtime_interval_sec;// ÿ����־�ļ���¼��־����ʱ�䣨�룩
		uint16_t idx;//��־�е����
		/**
		 * @brief	������־�ļ�·��
		 * @param	int lvl	��־�ȼ�
		 * @param	int seq	��־���
		 * @param	char * file_name	��������־�ļ�·��
		 * @param	const struct tm & t_m	ʱ��
		 */
		void gen_log_file_path(int lvl, int seq, char* file_name, const struct tm& t_m);
		//************************************
		// Brief:     �л���Ҫд���ļ���FD
		// Returns:   int -1:ERROR >0:���ش򿪵��ļ���FD,
		// Parameter: int lvl
		// Parameter: const struct tm * tm
		//************************************
		int shift_fd(int lvl, const struct tm& t_m);
			//************************************
		// Brief:     ��ȡ��־���
		// Returns:   int	��־���
		// Parameter: int lvl	��־�ȼ�
		//************************************
		int gen_log_seq(int lvl);
		int open_fd(int lvl, const struct tm& t_m);
		lib_log_t(const lib_log_t& cr);
		lib_log_t& operator=(const lib_log_t& cr);
	};
}//end namespace el

extern el::lib_log_t* g_log;

#ifndef LOG_USE_SYSLOG
	#ifdef WIN32
		#define DETAIL(level, key, fmt, ...) \
			g_log->write(level, key, "[%s][%d]%s: " fmt "\n", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

		#define SIMPLY(level, key, fmt, ...) \
			g_log->write(level, key, fmt "\n", __VA_ARGS__)
	#else
		#define DETAIL(level, key, fmt, args...) \
			g_log->write(level, key, "[%s][%d]%s: " fmt "\n", __FILE__, __LINE__, __FUNCTION__, ##args)

		#define SIMPLY(level, key, fmt, args...) \
			g_log->write(level, key, fmt "\n", ##args)
	#endif
#else
	#ifdef WIN32
		#define DETAIL(level, key, fmt, ...) \
			g_log->write_sys(level, "[%s][%d]%s: " fmt "\n", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

		#define SIMPLY(level, key, fmt, ...) \
			g_log->write_sys(level, fmt "\n", __VA_ARGS__)
	#else
		#define DETAIL(level, key, fmt, args...) \
			g_log->write_sys(level, "[%s][%d]%s: " fmt "\n", __FILE__, __LINE__, __FUNCTION__, ##args)

		#define SIMPLY(level, key, fmt, args...) \
			g_log->write_sys(level, fmt "\n", ##args)
	#endif
#endif

#ifndef DISABLE_EMERG_LOG
	#ifdef WIN32
		#define EMERG_LOG(fmt, ...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_EMERG, 0, fmt, __VA_ARGS__)

		#define KEMERG_LOG(key, fmt, ...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_EMERG, key, fmt, __VA_ARGS__)
	#else
		#define EMERG_LOG(fmt, args...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_EMERG, 0, fmt, ##args)

		#define KEMERG_LOG(key, fmt, args...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_EMERG, key, fmt, ##args)
	#endif
#else
	#ifdef WIN32
		#define EMERG_LOG(fmt, ...)
		#define KEMERG_LOG(key, fmt, ...) 
	#else
		#define EMERG_LOG(fmt, args...)
		#define KEMERG_LOG(key, fmt, args...) 
	#endif
#endif

#ifndef DISABLE_ALERT_LOG
	#ifdef WIN32
		#define ALERT_LOG(fmt, ...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_ALERT, 0, fmt, __VA_ARGS__)

		#define KALERT_LOG(key, fmt, ...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_ALERT, key, fmt, __VA_ARGS__)
	#else
		#define ALERT_LOG(fmt, args...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_ALERT, 0, fmt, ##args)

		#define KALERT_LOG(key, fmt, args...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_ALERT, key, fmt, ##args)
	#endif
#else
	#ifdef WIN32
		#define ALERT_LOG(fmt, ...)
		#define KALERT_LOG(key, fmt, ...)
	#else
		#define ALERT_LOG(fmt, args...)
		#define KALERT_LOG(key, fmt, args...)
	#endif
#endif

#ifndef DISABLE_CRIT_LOG
	#ifdef WIN32
		#define CRIT_LOG(fmt, ...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_CRIT, 0, fmt, __VA_ARGS__)

		#define KCRIT_LOG(key, fmt, ...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_CRIT, key, fmt, __VA_ARGS__)
	#else
		#define CRIT_LOG(fmt, args...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_CRIT, 0, fmt, ##args)

		#define KCRIT_LOG(key, fmt, args...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_CRIT, key, fmt, ##args)
	#endif
#else
	#ifdef WIN32
		#define CRIT_LOG(fmt, ...)
		#define KCRIT_LOG(key, fmt, ...) 
	#else
		#define CRIT_LOG(fmt, args...)
		#define KCRIT_LOG(key, fmt, args...) 
	#endif
#endif

#ifndef DISABLE_ERROR_LOG
	#ifdef WIN32
		#define ERROR_LOG(fmt, ...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_ERROR, 0, fmt, __VA_ARGS__)

		#define KERROR_LOG(key, fmt, ...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_ERROR, key, fmt, __VA_ARGS__)
	#else
		#define ERROR_LOG(fmt, args...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_ERROR, 0, fmt, ##args)

		#define KERROR_LOG(key, fmt, args...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_ERROR, key, fmt, ##args)
	#endif
#else
	#ifdef WIN32
		#define ERROR_LOG(fmt, ...)
		#define KERROR_LOG(key, fmt, ...) 
	#else
		#define ERROR_LOG(fmt, args...)
		#define KERROR_LOG(key, fmt, args...) 
	#endif
#endif

#ifndef DISABLE_WARN_LOG
	#ifdef WIN32
		#define WARN_LOG(fmt, ...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_WARNING, 0, fmt, __VA_ARGS__)

		#define KWARN_LOG(key, fmt, ...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_WARNING, key, fmt, __VA_ARGS__)
	#else
		#define WARN_LOG(fmt, args...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_WARNING, 0, fmt, ##args)

		#define KWARN_LOG(key, fmt, args...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_WARNING, key, fmt, ##args)
	#endif
#else
	#ifdef WIN32
		#define WARN_LOG(fmt, ...)
		#define KWARN_LOG(key, fmt, ...) 
	#else
		#define WARN_LOG(fmt, args...)
		#define KWARN_LOG(key, fmt, args...) 
	#endif
#endif

#ifndef DISABLE_NOTI_LOG
	#ifdef WIN32
		#define NOTI_LOG(fmt, ...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_NOTICE, 0, fmt, __VA_ARGS__)

		#define KNOTI_LOG(key, fmt, ...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_NOTICE, key, fmt, __VA_ARGS__)
	#else
		#define NOTI_LOG(fmt, args...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_NOTICE, 0, fmt, ##args)

		#define KNOTI_LOG(key, fmt, args...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_NOTICE, key, fmt, ##args)
	#endif
#else
	#ifdef WIN32
	#define NOTI_LOG(fmt, ...)
	#define KNOTI_LOG(key, fmt, ...)
	#else
		#define NOTI_LOG(fmt, args...)
		#define KNOTI_LOG(key, fmt, args...)
	#endif
#endif

#ifndef DISABLE_INFO_LOG
	#ifdef WIN32
		#define INFO_LOG(fmt, ...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_INFO, 0, fmt, __VA_ARGS__)

		#define KINFO_LOG(key, fmt, ...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_INFO, key, fmt, __VA_ARGS__)

		#define INFO_MSG_LOG(fmt, ...) \
			SIMPLY(el::lib_log_t::E_LOG_LEVEL_INFO, 0, fmt, __VA_ARGS__)
	#else
		#define INFO_LOG(fmt, args...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_INFO, 0, fmt, ##args)

		#define KINFO_LOG(key, fmt, args...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_INFO, key, fmt, ##args)

		#define INFO_MSG_LOG(fmt, args...) \
			SIMPLY(el::lib_log_t::E_LOG_LEVEL_INFO, 0, fmt, ##args)
	#endif
#else
	#ifdef WIN32
		#define INFO_LOG(fmt, ...)
		#define KINFO_LOG(key, fmt, ...) 
	#else
		#define INFO_LOG(fmt, args...)
		#define KINFO_LOG(key, fmt, args...) 
	#endif
#endif

#ifndef DISABLE_DEBUG_LOG
	#ifdef WIN32
		#define DEBUG_LOG(fmt, ...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_DEBUG, 0, fmt, __VA_ARGS__)

		#define KDEBUG_LOG(key, fmt, ...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_DEBUG, key, fmt, __VA_ARGS__)
	#else
		#define DEBUG_LOG(fmt, args...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_DEBUG, 0, fmt, ##args)

		#define KDEBUG_LOG(key, fmt, args...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_DEBUG, key, fmt, ##args)
	#endif
#else
	#ifdef WIN32
		#define DEBUG_LOG(fmt, ...)
		#define KDEBUG_LOG(key, fmt, ...) 
	#else
		#define DEBUG_LOG(fmt, args...)
		#define KDEBUG_LOG(key, fmt, args...) 
	#endif
#endif

#ifdef ENABLE_TRACE_LOG
	#ifdef WIN32
		#define TRACE_LOG(fmt, ...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_TRACE, 0, fmt, __VA_ARGS__)

		#define KTRACE_LOG(key, fmt, ...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_TRACE, key, fmt, __VA_ARGS__)

		#define TRACE_MSG_LOG(fmt, ...) \
			SIMPLY(el::lib_log_t::E_LOG_LEVEL_TRACE, 0, fmt, __VA_ARGS__)
	#else
		#define TRACE_LOG(fmt, args...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_TRACE, 0, fmt, ##args)

		#define KTRACE_LOG(key, fmt, args...) \
			DETAIL(el::lib_log_t::E_LOG_LEVEL_TRACE, key, fmt, ##args)

		#define TRACE_MSG_LOG(fmt, args...) \
			SIMPLY(el::lib_log_t::E_LOG_LEVEL_TRACE, 0, fmt, ##args)
	#endif
#else
	#ifdef WIN32
		#define TRACE_LOG(fmt, ...)
		#define KTRACE_LOG(key, fmt, ...)
		#define TRACE_MSG_LOG(fmt, ...)
	#else
		#define TRACE_LOG(fmt, args...)
		#define KTRACE_LOG(key, fmt, args...)
		#define TRACE_MSG_LOG(fmt, args...)
	#endif
#endif

/**
 * @def BOOT_LOG
 * @brief �������������־����Ļ,������__state__
 *        �÷�ʾ����BOOT_LOG(-1, "dlopen error, %s", error);
 */
#ifdef WIN32
	#define BOOT_LOG(__state__, fmt, ...) \
		do { \
		g_log->boot(__state__, 0, fmt, __VA_ARGS__); \
		return __state__; \
		} while (0)
#else
	#define BOOT_LOG(__state__, fmt, args...) \
		do { \
		g_log->boot(__state__, 0, fmt, ##args); \
		return __state__; \
		} while (0)
#endif

//��ӡ16���Ƶ�����(ʹ��TRACE_MSG_LOG)
#ifdef SHOW_ALL_LOG_HEX
    #define TRACE_MSG_HEX_LOG(__data__, __len__) {\
            std::string outbuf;\
            el::bin2hex(outbuf, (char*)__data__, __len__);\
            TRACE_MSG_LOG("[len:%u, %s]", __len__, outbuf.c_str());\
    }
#else
	#define TRACE_MSG_HEX_LOG(__data__, __len__)
#endif