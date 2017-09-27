/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	时间函数	
	brief:		ok
*********************************************************************/

#pragma once

#include "lib_include.h"
#include "lib_platform.h"

namespace el{
	struct lib_time_t{
	public:
		static const uint32_t ONE_HOUR_SEC = 60*60;
		static const uint32_t ONE_DAY_SEC = ONE_HOUR_SEC*24;
		static const uint32_t WEEK_SEC = ONE_DAY_SEC * 7;
		static const uint32_t TIME_SEC_MAX = INT_MAX;
	public:
		//************************************
		// Brief:     returns the time as the number of seconds since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).	
		// Returns:   time_t	
		//************************************
		static time_t now_second(){
			return ::time(NULL);
		}
		static void now_tm(struct tm& rtm){
			time_t now = lib_time_t::now_second();
			::localtime_r(&now, &rtm);
		}

		//************************************
		// @brief:	计算下一个时分秒的时间		
		//************************************
		static inline uint32_t cal_next_sec(uint32_t now_sec, uint32_t next_hour = 0, uint32_t next_min = 0, uint32_t next_sec = 0)
		{
			struct tm t;
			el::lib_time_t::now_tm(t);
			t.tm_hour = next_hour;
			t.tm_min = next_min;
			t.tm_sec = next_sec;
			time_t gen_t;
			gen_t = ::mktime(&t);
			if ((uint32_t)gen_t < now_sec){
				gen_t += el::lib_time_t::ONE_DAY_SEC;
			}
			
			return (uint32_t)gen_t;
		}
		/* @brief 由年，月，日，小时，分钟，秒计算时间秒数	
		 */
		static inline uint32_t cal_ymdhms(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
		{
			time_t rawtime;
			time(&rawtime);

 			struct tm timeinfo;
 			::localtime_r(&rawtime, &timeinfo);

			timeinfo.tm_year = year - 1900;
			timeinfo.tm_mon = month - 1;
			timeinfo.tm_mday = day;
			timeinfo.tm_hour = hour;
			timeinfo.tm_min = minute;
			timeinfo.tm_sec = second;
			rawtime = ::mktime(&timeinfo);
			return (uint32_t)rawtime;
		}
		//由传入sec,计算出ymdhms	
		static inline void cal_ymdhms(uint32_t sec, uint16_t& year, 
			uint8_t& month, uint8_t& day, uint8_t& hour, uint8_t& minute, uint8_t& second)
		{
			time_t rawtime;
			rawtime = sec;
			struct tm stm;
			::localtime_r(&rawtime, &stm);
			year = (uint16_t)(stm.tm_year + 1900);
			month = (uint8_t)(stm.tm_mon + 1);
			day = (uint8_t)stm.tm_mday;
			hour = (uint8_t)stm.tm_hour;
			minute = (uint8_t)stm.tm_min;
			second = (uint8_t)stm.tm_sec;
		}

		//************************************
		// @brief:	计算新的时间	
		// parameter:	uint32_t add_aicro 增加的毫秒数,默认增加100毫秒	
		// return:	timeval	
		//************************************
		static inline timeval cal_micro(timeval tv, uint32_t add_aicro = 100000){
			tv.tv_usec += add_aicro;
			while(1000000 <= tv.tv_usec){
				tv.tv_sec += 1;
				tv.tv_usec -= 1000000;
			}
			return tv;
		}

		/**
		* @brief 把`tm_cur`按小时取整点时间。比如`tm_cur`的时间是20081216-16:10:25，则返回的时间是20081216-16:00:00。	
		*
		* @param struct tm tm_cur,  需要按小时取整点的时间。	
		*
		* @return time_t, 按小时取整点后从Epoch开始的秒数。	
		*/
		static inline time_t get_integral_tm_hour(tm* tm_cur){
			tm_cur->tm_min  = 0;
			tm_cur->tm_sec  = 0;

			return ::mktime(tm_cur);
		}

		/* @brief 获取当天日期	
		 * @return 格式为"YYYYMMDD"	
		 */
		static inline int get_date_yyyymmdd(time_t rawtime)
		{
			struct tm tm_tmp;
			::localtime_r(&rawtime, &tm_tmp) ;
			return (tm_tmp.tm_year+1900)*10000+(tm_tmp.tm_mon+1)*100+tm_tmp.tm_mday;
		}

		/* @brief 获取当天年月	
		 * @return 格式为"YYYYMM"	
		 */
		static inline int get_date_yyyymm(time_t rawtime)
		{
			struct tm tm_tmp;
			::localtime_r(&rawtime, &tm_tmp) ;
			return (tm_tmp.tm_year+1900)*100+tm_tmp.tm_mon+1;
		}

		/* @brief 判断时间是否是今天	
		 * @param time_seconds 要判断的时间	
		 */
		static inline bool time_is_today(uint32_t time_seconds, uint32_t time_now = (uint32_t)::time(NULL))
		{
			return (get_date_yyyymmdd(time_now) == get_date_yyyymmdd(time_seconds));
		}

		/** @brief 由星期，小时，分钟, 秒计算下次时间秒数(当前时间)	
		 */
		static uint32_t cal_next_time_whms(uint8_t wday, uint8_t hour = 0, uint8_t minute = 0, uint8_t second = 0)
		{
			time_t rawtime = lib_time_t::now_second();

			struct tm timeinfo;
			::localtime_r(&rawtime, &timeinfo);

			if(timeinfo.tm_wday == wday){
				uint32_t next_s = hour * ONE_HOUR_SEC + minute * 60 + second ;
				uint32_t s = timeinfo.tm_hour * ONE_HOUR_SEC + timeinfo.tm_min * 60 + timeinfo.tm_sec;
				if(s == next_s){
					return (uint32_t)rawtime;
				}
				if(next_s > s){
					return (uint32_t)(rawtime + (next_s - s));
				}
				if(next_s < s){
					timeinfo.tm_mday +=  7 - (timeinfo.tm_wday - wday);
				}
			} else if(timeinfo.tm_wday < wday){
				timeinfo.tm_mday +=  wday - timeinfo.tm_wday;  
			} else {
				timeinfo.tm_mday +=  7 - (timeinfo.tm_wday - wday);
			}

			timeinfo.tm_hour = hour;
			timeinfo.tm_min = minute;
			timeinfo.tm_sec = second;

			return (uint32_t)::mktime(&timeinfo);
		}

		/** @brief 计算当日零点时间	
		 */
		static inline uint32_t today_start_time()
		{
			time_t rawtime;
			::time(&rawtime);
			
 			struct tm timeinfo;
 			::localtime_r(&rawtime, &timeinfo);

			timeinfo.tm_hour = 0;
			timeinfo.tm_min = 0;
			timeinfo.tm_sec = 0;
			rawtime = ::mktime(&timeinfo);
			return (uint32_t)rawtime;
		}

		/** @brief 计算本小时开始时间	
		 */
		static inline uint32_t hour_start_time()
		{
			time_t rawtime;
			::time(&rawtime);
			
 			struct tm timeinfo;
 			::localtime_r(&rawtime, &timeinfo);

			time_t hour_start_sec = rawtime - timeinfo.tm_min * 60 - timeinfo.tm_sec;

			return (uint32_t)hour_start_sec;
		}

		/** @brief 计算零点时间	
		 */
		static inline uint32_t day_start_time(time_t rawtime)
		{
 			struct tm timeinfo;
 			::localtime_r(&rawtime, &timeinfo);

			timeinfo.tm_hour = 0;
			timeinfo.tm_min = 0;
			timeinfo.tm_sec = 0;
			rawtime = ::mktime(&timeinfo);
			return (uint32_t)rawtime;
		}

		/** @brief 计算从0开始,第几天		
		 */
		static inline uint32_t day_number(time_t rawtime)
		{
			uint32_t sec = lib_time_t::day_start_time(rawtime);
			return sec/ONE_DAY_SEC;
		}

		/** @brief 计算明日零点时间	
		 */
		static inline uint32_t tomorrow_start_time()
		{
			time_t rawtime;
			::time(&rawtime);

			struct tm timeinfo;
			::localtime_r(&rawtime, &timeinfo);

			timeinfo.tm_mday += 1;
			timeinfo.tm_hour = 0;
			timeinfo.tm_min = 0;
			timeinfo.tm_sec = 0;
			rawtime = ::mktime(&timeinfo);
			return (uint32_t)rawtime;
		}

		/** @brief 计算下个月开始零点时间	
		 */
		static inline uint32_t next_month_start_time(time_t rawtime)
		{
			int yyyymm = lib_time_t::get_date_yyyymm(rawtime);
			uint16_t yyyy = (uint16_t)(yyyymm/100);
			uint8_t mm = (uint8_t)(yyyymm%100);
			if (12 == mm){
				yyyy++;
				mm = 1;
			} else {
				mm++;
			}
			
			return lib_time_t::cal_ymdhms(yyyy, mm, 1, 0, 0, 0);
		}

		// @brief:	计算是从时间戳开始,是第几周(按照周一开始,周日结束计算)	

		static inline uint32_t time_week_num(time_t t){
			const time_t t1 = 0;
		#ifdef WIN32
			struct tm* ptm = ::localtime(&t1);
			int tm_wday = (0 == ptm->tm_wday?7:ptm->tm_wday);
			int sec = ptm->tm_hour*ONE_HOUR_SEC + ptm->tm_min*60 + ptm->tm_sec + (tm_wday-1)*ONE_DAY_SEC;
		#else
			struct tm t_m;
			::localtime_r(&t1, &t_m);
			int tm_wday = (0 == t_m.tm_wday?7:t_m.tm_wday);
			int sec = t_m.tm_hour*ONE_HOUR_SEC + t_m.tm_min*60 + t_m.tm_sec + (tm_wday-1)*ONE_DAY_SEC;
		#endif
			t += sec;
			return (uint32_t)(t/WEEK_SEC + 1);
		}


		// @brief:	计算近天是周几(周一:1 ~ 周日:7)	
		static inline uint32_t time_week_today(void)
		{
			time_t now_time;
			::time(&now_time);
		#ifdef WIN32
			struct tm* ptm = ::localtime(&now_time);
			return 0 == ptm->tm_wday ? 7 : ptm->tm_wday;
		#else
			struct tm t_m;
			::localtime_r(&now_time, &t_m);
			return 0 == t_m.tm_wday ? 7 : t_m.tm_wday;
		#endif
		}

		//get zodiac index
		static inline int zodiac( int mon, int mon_day ){
			int zodiac = 0;
			static const int zodiac_day[12][2] = {
				{1,20},//2
				{2,19},//3
				{3,21},//4
				{4,21},//5
				{5,21},//6
				{6,22},//7
				{7,23},//8
				{8,23},//9
				{9,23},//10
				{10,23},//11
				{11,22},//12
				{12,22}//1
			};
			for(int i = 0; i<12 ;i++){
				if(mon == zodiac_day[i][0]){
					if(mon_day >= zodiac_day[i][1]){
						zodiac = (i + 2) % 12;
						break;
					}else{
						i == 0 ? zodiac = 1 : zodiac = i + 1;
						break;
					}
				}
			}
			return zodiac;
		}
	protected:
	private:
		lib_time_t(const lib_time_t& cr);
		lib_time_t& operator=(const lib_time_t& cr);
	};
}
