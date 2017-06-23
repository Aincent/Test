/*
 * ServerCommonDefine.h
 *
 *  Created on: 2014年5月21日
 *      Author: root
 */

#ifndef SERVERCOMMONDEFINE_H_
#define SERVERCOMMONDEFINE_H_

//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
#define GT_INVALID				(-1)
#define GT_VALID(n)				(((int)(n)) != GT_INVALID)
#define P_VALID(n)				( (((int)(n)) != GT_INVALID) && ((n) != NULL) )

#ifndef SAFE_DEL
#define SAFE_DEL(p)				{ if(p) { delete(p);		(p) = NULL; } }
#endif
#ifndef SAFE_DEL_ARRAY
#define SAFE_DEL_ARRAY(p)		{ if(p) { delete[](p);		(p) = NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)			{ if(p) { (p)->Release();	(p) = NULL; } }
#endif

#endif /* SERVERCOMMONDEFINE_H_ */
