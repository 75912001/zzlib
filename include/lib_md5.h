/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	
	brief:		����MD5��
*********************************************************************/
#pragma once

#include <string>

namespace el{
	struct lib_md5_t
	{
		static std::string md5_string(const std::string* in);
	};
}
