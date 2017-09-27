#include "lib_time.h"
#include "lib_lock.h"
#include "lib_log.h"
#include "lib_util.h"
#include "lib_file.h"
#include "lib_timer.h"
#include "lib_platform.h"

el::lib_log_t* g_log = NULL;

namespace{

	const uint32_t MAX_LOG_CNT = 10000000; //日志文件最大数量
	const uint32_t LOG_BUF_SIZE = 81920; //每条日志的最大字节数
#ifdef WIN32
	const char* s_log_color[] = { "", "", 
		"", "", "", "",
		"", "", ""};
	const char* s_color_end = "";
#else
	const char* s_log_color[] = { "\e[1m\e[35m", "\e[1m\e[33m", 
		"\e[1m\e[34m", "\e[1m\e[31m", "\e[1m\e[32m", "\e[1m\e[36m",
		"\e[1m\e[1m", "\e[1m\e[37m", "\e[1m\e[37m"};
	const char* s_color_end = "\e[m";
#endif



	static const char* log_names[] = { "emerg", "alert", "crit", 
		"error", "warn", "notice", "info", "debug", "trace" };
}//end of namespace 

int el::lib_log_t::init( const char* dir, E_LOG_LEVEL lvl, const char* pre_name, uint32_t logtime )
{
	int ret_code = -1;
	assert(logtime <= 30000000);

	this->logtime_interval_sec = logtime * 60;

	if (NULL == dir || (0 == ::strlen(dir))) {
		::fprintf(stderr, "init log dir is NULL!!!\n");
		ret_code = 0;
		goto loop_return;
	}

	if ((lvl < E_LOG_LEVEL_EMERG) || (lvl >= E_LOG_LEVEL_MAX)) {
		::fprintf(stderr, "init log error, invalid log level=%d\n", lvl);
		goto loop_return;
	}

	//必须可写
	if (0 != ::access(dir, W_OK)) {
		::fprintf(stderr, "access log dir %s error\n", dir);
		goto loop_return;
	}

	//填写  日志等级/目录/文件名前缀
	this->level = lvl;
	this->dir_name = dir;
	if (NULL == pre_name){
		this->file_pre_name.clear();
	}else{
		this->file_pre_name = pre_name;
	}

	for (int i = E_LOG_LEVEL_EMERG; i < E_LOG_LEVEL_MAX; i++) {
		this->fds_info[i].init();
		this->fds_info[i].base_file_name = this->file_pre_name + log_names[i];
		this->fds_info[i].seq  = gen_log_seq(i);
		if (this->fds_info[i].seq < 0) {
			goto loop_return;
		}
	}

	this->log_dest = E_LOG_DEST_FILE;
	ret_code    = 0;

loop_return:
	BOOT_LOG(ret_code, "log [dir:%s, per_name:%s, interval_time:%u]", dir, pre_name, logtime);
}



void el::lib_log_t::set_dest( E_LOG_DEST dest )
{
	this->log_dest = dest;
}

void el::lib_log_t::boot( int ok, int space, const char* fmt, ... )
{
	int end, pos;
	va_list ap;

	char log_buffer[LOG_BUF_SIZE];

	va_start(ap, fmt);
	end = ::vsprintf(log_buffer, fmt, ap);
	va_end(ap);
	static const int SCREEN_COLS = 80;
	pos = SCREEN_COLS - 10 - (end - space) % SCREEN_COLS;
	int i = 0;
	for (; i < pos; i++){
		log_buffer[end + i] = ' ';
	}
	log_buffer[end + i] = '\0';
#ifdef WIN32
	static const char* BOOT_OK = "[ OK ]";
	static const char* BOOT_FAIL = "[ ERR ]";
#else
	static const char* BOOT_OK = "\e[1m\e[32m[ OK ]\e[m";
	static const char* BOOT_FAIL = "\e[1m\e[31m[ ERR ]\e[m";
#endif

	::strcat(log_buffer, SUCC == ok ? BOOT_OK : BOOT_FAIL);
	::printf("\r%s\n", log_buffer);

	if (0 != ok){
		::exit(ok);
	}
}

int el::lib_log_t::shift_fd( int lvl, const struct tm& t_m )
{
	if ( unlikely((this->fds_info[lvl].opfd < 0) && (open_fd(lvl, t_m) < 0)) ) {
		return ERR;
	}

	//off_t length = lseek(fds_info[lvl].opfd, 0, SEEK_END);
	if (this->logtime_interval_sec) {
		if (likely(this->fds_info[lvl].seq == gen_log_seq(lvl))) {

			return SUCC;
		}
	} else {
		if (likely((this->fds_info[lvl].day == t_m.tm_yday))) {

			return SUCC;
		}
	}

	el::lib_file_t::close_fd(this->fds_info[lvl].opfd);

	if (this->logtime_interval_sec) {
		this->fds_info[lvl].seq = gen_log_seq(lvl);
	} else {
		// if logfile recycle is not used
		if (this->fds_info[lvl].day != t_m.tm_yday) {
			this->fds_info[lvl].seq = 0;
		} else {
			this->fds_info[lvl].seq = (this->fds_info[lvl].seq + 1) % MAX_LOG_CNT;
		}
	}

	int ret = open_fd(lvl, t_m);

	return ret;
}

int el::lib_log_t::gen_log_seq( int lvl )
{
	uint32_t seq = 0;
	if (0 != this->logtime_interval_sec){
		seq = (uint32_t)el::lib_time_t::now_second() / this->logtime_interval_sec;
	}else{
		char file_name[FILENAME_MAX];

		struct tm t_m;
		el::lib_time_t::now_tm(t_m);

		for (; seq != MAX_LOG_CNT; ++seq) {
			gen_log_file_path(lvl, seq, file_name, t_m);
			if (::access(file_name, F_OK) == -1) {
				break;
			}
		}
		seq = (seq ? (seq - 1) : 0);
	}
	return seq;
}

int el::lib_log_t::open_fd( int lvl, const struct tm& t_m )
{
	//O_APPEND 有该选项,write时是线程安全的.请看本页write函数
	int flag = O_WRONLY | O_CREAT | O_APPEND/* | O_LARGEFILE*/;

	char file_name[FILENAME_MAX];
	gen_log_file_path(lvl, this->fds_info[lvl].seq, file_name, t_m);

	this->fds_info[lvl].opfd = ::open(file_name, flag, 0644);
	if (-1 != this->fds_info[lvl].opfd) {
		if (this->fds_info[lvl].day != t_m.tm_yday ) {
			this->fds_info[lvl].day = t_m.tm_yday;
		}
#ifndef WIN32
		el::lib_file_t::fcntl_add_flag(this->fds_info[lvl].opfd, FD_CLOEXEC);
#endif
	}

	return this->fds_info[lvl].opfd;
}

void el::lib_log_t::gen_log_file_path( int lvl, int seq, char* file_name, const struct tm& t_m )
{
	assert((lvl >= el::lib_log_t::E_LOG_LEVEL_EMERG) && (lvl < el::lib_log_t::E_LOG_LEVEL_MAX));

	if (this->logtime_interval_sec) {
		time_t t = ::time(0) / this->logtime_interval_sec * this->logtime_interval_sec;
		struct tm tmp_tm;
		::localtime_r(&t, &tmp_tm);

		::sprintf(file_name, "%s/%s%04d%02d%02d%02d%02d.txt", 
			this->dir_name.c_str(), this->fds_info[lvl].base_file_name.c_str(),
			tmp_tm.tm_year + 1900, tmp_tm.tm_mon + 1, 
			tmp_tm.tm_mday, tmp_tm.tm_hour, tmp_tm.tm_min);
	} else {
		::sprintf(file_name, "%s/%s%04d%02d%02d%07d.txt", 
			this->dir_name.c_str(), this->fds_info[lvl].base_file_name.c_str(),
			t_m.tm_year + 1900, t_m.tm_mon + 1, t_m.tm_mday, seq);
	}
}

el::lib_log_t::~lib_log_t()
{
}

el::lib_log_t::lib_log_t()
{
	this->level = el::lib_log_t::E_LOG_LEVEL_DEBUG;
#ifndef WIN32
	this->log_dest = el::lib_log_t::E_LOG_DEST_TERMINAL;
#else
	this->log_dest = el::lib_log_t::E_LOG_DEST_FILE;
#endif
	this->logtime_interval_sec = 0;
	this->idx = 0;
}

void el::lib_log_t::write( int lvl,uint32_t key, const char* fmt, ... )
{
	if (lvl > this->level) {
		return;
	}

	va_list ap;

	const struct tm* t_m = &g_timer->now_tm;

	va_start(ap, fmt);

#ifndef WIN32
	if (unlikely(E_LOG_DEST_TERMINAL & this->log_dest)) {
		va_list aq;
		va_copy(aq, ap);
		switch (lvl) {
		case E_LOG_LEVEL_EMERG:
		case E_LOG_LEVEL_ALERT:
		case E_LOG_LEVEL_CRIT:
		case E_LOG_LEVEL_ERROR:
			::fprintf(stderr, "%s%02d:%02d:%02d ", s_log_color[lvl],
				t_m->tm_hour, t_m->tm_min, t_m->tm_sec);
			::vfprintf(stderr, fmt, aq);
			::fprintf(stderr, "%s", s_color_end);
			break;
		default:
			::fprintf(stdout, "%s%02d:%02d:%02d ", s_log_color[lvl],
				t_m->tm_hour, t_m->tm_min, t_m->tm_sec);
			::vfprintf(stdout, fmt, aq);
			::fprintf(stdout, "%s", s_color_end);
			break;
		}
		va_end(aq);
	}
#endif

	if (unlikely(!(this->log_dest & E_LOG_DEST_FILE) || (shift_fd(lvl, *t_m) < 0))) {
		va_end(ap);
		return;
	}

	char log_buffer[LOG_BUF_SIZE];
	//int pid = ::getpid();

	int pos = snprintf(log_buffer, sizeof(log_buffer), "[%02d:%02d:%02d][%05d][%u]",
		t_m->tm_hour, t_m->tm_min, t_m->tm_sec, this->idx++, key);

	int end = ::vsnprintf(log_buffer + pos, sizeof(log_buffer) - pos, fmt, ap);
	va_end(ap);

#ifdef EL_USE_LOG_THREAD
	this->add_event(this->fds_info[lvl].opfd, log_buffer, end + pos);
#else
	//kevin_mark 判断返回值(会被信号中断等处理方式).. on linux :  man write 
	//多线程写文件,这里不需要上锁!请 man 2 write 
	// If the file  was open(2)ed  with  O_APPEND,  
	// the file offset is first set to the end of the file before writing.
	// The adjustment of the file offset and the write operation are performed as an atomic step.
	HANDLE_EINTR(::write(this->fds_info[lvl].opfd, log_buffer, end + pos));
#endif
}

void el::lib_log_t::write_sys( int lvl, const char* fmt, ... )
{
#ifndef WIN32
	if (lvl > this->level) {
		return;
	}

	va_list ap;

	const struct tm* t_m = &g_timer->now_tm;

	va_start(ap, fmt);

	if (unlikely(E_LOG_DEST_TERMINAL & this->log_dest)) {
		switch (lvl) {
		case E_LOG_LEVEL_EMERG:
		case E_LOG_LEVEL_ALERT:
		case E_LOG_LEVEL_CRIT:          
		case E_LOG_LEVEL_ERROR:
			::fprintf(stderr, "%s%02d:%02d:%02d ", s_log_color[lvl],
				t_m->tm_hour, t_m->tm_min, t_m->tm_sec);
			::vfprintf(stderr, fmt, ap);
			::fprintf(stderr, "%s", s_color_end);
			break;
		default:
			::fprintf(stdout, "%s%02d:%02d:%02d ", s_log_color[lvl],
				t_m->tm_hour, t_m->tm_min, t_m->tm_sec);
			::vfprintf(stdout, fmt, ap);
			::fprintf(stdout, "%s", s_color_end);
			break;
		}
	}

	if (E_LOG_DEST_FILE & this->log_dest) {         
		char log_buffer[LOG_BUF_SIZE];
		int pos = ::snprintf(log_buffer, LOG_BUF_SIZE, "[%02d:%02d:%02d][%05d]",
			t_m->tm_hour, t_m->tm_min, t_m->tm_sec, ::getpid());
		::vsnprintf(log_buffer + pos, LOG_BUF_SIZE - pos, fmt, ap);
		::syslog(lvl, "%s", log_buffer);
	}

	va_end(ap);
#endif
}

#ifdef EL_USE_LOG_THREAD

void el::lib_log_thread_t::do_work_fn( void* data )
{
	LOG_BUF_LIST log_bufs;

	this->lock->lock();
	log_bufs = this->log_buf_list;
	this->log_buf_list.clear();
	this->lock->ulock();

	FOREACH(log_bufs, it){
		log_buf_t* buf = *it;
		//buf->buf.push_back('\n');

		//kevin_mark 判断返回值(会被信号中断等处理方式).. on linux :  man write 
		//多线程写文件,这里不需要上锁!请 man 2 write 
		// If the file  was open(2)ed  with  O_APPEND,  
		// the file offset is first set to the end of the file before writing.
		// The adjustment of the file offset and the write operation are performed as an atomic step.
		HANDLE_EINTR(::write(buf->fd, buf->buf.c_str(), buf->buf.size()));
		SAFE_DELETE(buf);
	}
}

int el::lib_log_thread_t::do_work_end( void* data )
{
	this->do_work_fn(data);
	return SUCC;
}

void el::lib_log_thread_t::add_event( int fd, const char* data, uint32_t len )
{
	log_buf_t* buf = new log_buf_t;
	buf->fd = fd;
	buf->buf.assign(data, len);

	this->lock->lock();
	this->log_buf_list.push_back(buf);
	this->lock->ulock();

	this->notify_event();
}

void el::lib_log_thread_t::clear_log_buf_list()
{
	this->lock->lock();
	FOREACH(this->log_buf_list, it){
		log_buf_t* buf = *it;
		SAFE_DELETE(buf);
	}
	this->log_buf_list.clear();
	this->lock->ulock();
}

el::lib_log_thread_t::lib_log_thread_t()
{
	this->lock = new el::lib_lock_mutex_t;
}

el::lib_log_thread_t::~lib_log_thread_t()
{
	SAFE_DELETE(this->lock);
}



#endif

void el::lib_log_t::fds_t::init()
{
	this->seq = 0;
	this->opfd = -1;
	this->day = 0;
	this->base_file_name.clear();
}

el::lib_log_t::fds_t::fds_t()
{
	this->init();
}

el::lib_log_t::fds_t::~fds_t()
{
	el::lib_file_t::close_fd(this->opfd);
}
