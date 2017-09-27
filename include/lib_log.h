/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	ok
	brief:		用于记录日志，一共分9种日志等级。
	注意，每条日志不能超过8000字节。
	如果编译程序时定义宏LOG_USE_SYSLOG，则会利用syslog来记录日志，使用的facility是LOG_USER。
*********************************************************************/

#pragma once

#ifdef WIN32
#else
//只在linux生效 使用线程
// 目前使用线程的话效率比不使用还低,应该在于NEW 和 delete ,加锁的过程.
// 不使用线程 使用 99秒 打印10004990行日志 101060行/秒(144927行/秒, 4G内存)
// 使用线程  使用 151秒 打印10004990行日志  66258行/秒
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
		// @brief:	清理未打印的日志缓冲
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
		 * @brief 日志等级
		 */
		enum E_LOG_LEVEL {
			/*! system is unusable -- 0 */
			E_LOG_LEVEL_EMERG = 0,
			/*! action must be taken immediately -- 1 */
			E_LOG_LEVEL_ALERT = 1,//黄色
			/*! critical conditions -- 2 */
			E_LOG_LEVEL_CRIT = 2,//深蓝
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
			/*! trace-level messages -- 8。如果定义了宏LOG_USE_SYSLOG，则log_lvl_trace==log_lvl_debug */
		#ifndef LOG_USE_SYSLOG
			E_LOG_LEVEL_TRACE = 8,//,
		#else
			E_LOG_LEVEL_TRACE = E_LOG_LEVEL_DEBUG,
		#endif
			E_LOG_LEVEL_MAX = 9,
		};
		/**
		 * @enum  log_dest
		 * @brief 日志输出方式
		 */
		enum E_LOG_DEST {//使用BIT位来写入数据.
			/*! 仅输出到屏幕  */
			E_LOG_DEST_TERMINAL	= 1,
			/*! 仅输出到文件 */
			E_LOG_DEST_FILE		= 2,
			/*! 既输出到屏幕，也输出到文件 */
			E_LOG_DEST_BOTH		= 3
		};
	public:
		lib_log_t();
		virtual ~lib_log_t();
		/**
		* @brief 初始化日志记录功能。按时间周期创建新的日志文件。
		* @param dir 日志保存目录。如果填0，则在屏幕中输出日志。
		* @param lvl 日志输出等级。如果设置为e_lvl_notice，则e_lvl_notice以上等级的日志都不输出。
		* @param pre_name 日志文件名前缀。
		* @param logtime 每个日志文件保存logtime分钟的日志，最大不能超过30000000分钟。
		*				假设logtime为15，则每小时产生4个日志文件，每个文件保存15分钟日志。
		* @see set_dest
		*
		* @return 成功返回0，失败返回-1。
		*/
		int init(const char* dir, E_LOG_LEVEL lvl, const char* pre_name, uint32_t logtime);

		/**
		* @brief 调用log_init初始化日志功能后，可以调用该函数动态调整日志的输出方式。
		*		如果不调用set_dest的话，则输出方式为setup_by_time时确定的方式。
		*		注意：必须在setup_by_time时指定了日志保存目录，才可以调用该函数。
		* @param dest 日志输出方式
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
			int		seq;//顺序号
			int		opfd;//文件FD
			int		day;//一年中的天数
			std::string base_file_name;//基本文件名
			fds_t();
			virtual ~fds_t();
			void init();
		};
		fds_t fds_info[el::lib_log_t::E_LOG_LEVEL_MAX];
		el::lib_log_t::E_LOG_LEVEL  level;	  // default log level
		std::string dir_name;//日志目录
		std::string file_pre_name;//文件名前缀
		el::lib_log_t::E_LOG_DEST log_dest;//日志输出目的
		uint32_t logtime_interval_sec;// 每个日志文件记录日志的总时间（秒）
		uint16_t idx;//日志中的序号
		/**
		 * @brief	生成日志文件路径
		 * @param	int lvl	日志等级
		 * @param	int seq	日志序号
		 * @param	char * file_name	产生的日志文件路径
		 * @param	const struct tm & t_m	时间
		 */
		void gen_log_file_path(int lvl, int seq, char* file_name, const struct tm& t_m);
		//************************************
		// Brief:     切换打开要写入文件的FD
		// Returns:   int -1:ERROR >0:返回打开的文件的FD,
		// Parameter: int lvl
		// Parameter: const struct tm * tm
		//************************************
		int shift_fd(int lvl, const struct tm& t_m);
			//************************************
		// Brief:     获取日志序号
		// Returns:   int	日志序号
		// Parameter: int lvl	日志等级
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
 * @brief 输出程序启动日志到屏幕,并返回__state__
 *        用法示例：BOOT_LOG(-1, "dlopen error, %s", error);
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

//打印16进制的数据(使用TRACE_MSG_LOG)
#ifdef SHOW_ALL_LOG_HEX
    #define TRACE_MSG_HEX_LOG(__data__, __len__) {\
            std::string outbuf;\
            el::bin2hex(outbuf, (char*)__data__, __len__);\
            TRACE_MSG_LOG("[len:%u, %s]", __len__, outbuf.c_str());\
    }
#else
	#define TRACE_MSG_HEX_LOG(__data__, __len__)
#endif