/*****************************************************************************

@file		WarningsOff.h
@brief	Turns off warnings so Microsoft provided STL headers will
			compile at warning level 4

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

   $Id: WarningsOff.h,v 1.1 2002/06/29 14:31:18 ecrahen Exp $
 
*****************************************************************************/

#ifndef WARNINGSOFF_H
#define WARNINGSOFF_H
/*lint -efile(766,WarningsOff.h)*/

#if defined(_WIN32)

/*
 * Warnings Disabled
 *   4514: unreferenced inline/local function has been removed
 *   4786: 'identifier' : identifier was truncated to 'number' characters in the debug information
 *   4503: 'identifier' : decorated name length exceeded, name was truncated
 */
#pragma warning ( disable : 4514 4786 4503 )

/*
 * RELEASE ONLY:
 *   4710: 'function' : function not inlined
 */
#ifndef _DEBUG
#pragma warning ( disable : 4710 )
#endif

/* Force everything to warning level 3 */
#pragma warning ( push , 3 )

#endif

#endif	/*!WARNINGSOFF_H*/
