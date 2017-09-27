/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	
	brief:		Éú³ÉMD5Âë
*********************************************************************/
#pragma once

#include <string>

namespace el{
	class lib_md5_t
	{
	public:
		lib_md5_t(){}
		virtual ~lib_md5_t(){}
		static std::string md5_string(const std::string* in);
	protected:
		
	private:
		lib_md5_t(const lib_md5_t& cr);
		lib_md5_t& operator=(const lib_md5_t& cr);
	};
}
