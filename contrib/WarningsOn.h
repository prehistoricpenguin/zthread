/*****************************************************************************

@file		WarningsOn.h
@brief	Turns on warnings after being disabled so Microsoft provided STL
			headers will compile at warning level 4

	Taken/modified from:
		"Debugging Applications" (Microsoft Press)
		Copyright 1997-2000 John Robbins

	Can be used for any header that cannot compile at warning level 4,
	not just the STL headers.

	USAGE:
		- Set compiler to warning level 4
		- #include "WarningsOff.h" before the offending headers
		- #include the offending headers
		- #include "WarningsOn.h" after the offending headers

	EAMPLE:
		#include "WarningsOff.h"
		#include <ostream>
		#include "WarningsOn.h"

******************************************************************************

 PC-lint flags modified
	766		Header file FileName not used in module String

******************************************************************************

   $Id: WarningsOn.h,v 1.1 2002/06/29 14:31:18 ecrahen Exp $
 
*****************************************************************************/

#ifndef WARNINGSON_H
#define WARNINGSON_H
/*lint -efile(766,WarningsOn.h)*/

#if defined(_WIN32)

/* Get the warning level back to level 4 */
#pragma warning ( pop )

/*
 * Warnings Disabled
 *		4290 : C++ Exception Specification ignored
 */
#pragma warning ( disable : 4290 )

#endif

#endif	/*!WARNINGSON_H*/
