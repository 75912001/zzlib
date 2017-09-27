/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	brief:		ok
*********************************************************************/

#pragma once

#define CLASS_MEMBER_SIZEOF(__class__, __member__) \
	sizeof(((__class__*)0)->__member__)

namespace el{
	class lib_non_copyable_t{
	protected:
		lib_non_copyable_t(){}
	private:
		lib_non_copyable_t( const lib_non_copyable_t& );
		const lib_non_copyable_t& operator=( const lib_non_copyable_t& );
	};
}
