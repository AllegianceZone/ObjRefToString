// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#if !defined(AFX_pchObjRef2String__INCLUDED_)
#define AFX_pchObjRef2String__INCLUDED_

#pragma warning (disable:4786)  // disable 'identifier was truncated to '255'...' warning
#pragma warning(disable:4355) // warning C4355: 'this' : used in base member initializer list

#if !defined (_WIN64) 
	#define FLOATASM //Imago guarded from x64 compilers on 6/20/09
	#define USEASM
#endif

// VS.Net 2003 port: typename is required for template
#if _MSC_VER >= 1310
	#define VSNET_TNFIX typename
#else
	#define VSNET_TNFIX
#endif
#if _MSC_VER >= 1000
  #pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers
#define _WIN32_DCOM         // Enable DCOM features
#define __MODULE__ "ObjRef2String"

// STL headers
#include <algorithm>
#include <map>
#include <set>
#include <vector>

// CRT headers
#include <crtdbg.h>
#include <windows.h>        // Win32 declarations
#include <malloc.h>
#include <io.h>
#include <fcntl.h>

// ATL headers
#include <atlbase.h>        // ATL base declarations
extern CComModule _Module;  // ATL mandatory symbol
#include <atlcom.h>         // ATL component object classes
#include <atlwin.h>         // ATL window classes
#include <comcat.h>
#include <comdef.h>
#include <commctrl.h>
#include <atlconv.h>
#include <objsafe.h>

// Most ZLib headers (not Window and Version, they don't work /w wide chars)
#include "zassert.h"
#include "alloc.h"
#include "mask.h"
#include "tref.h"
#include "cast.h"
#include "zmath.h"
#include "zadapt.h"
#include "tcompare.h"
#include "tarray.h"
#include "tvector.h"
#include "tstack.h"
#include "tlist.h"
#include "tmap.h"
#include "comobj.h"
#include "ztime.h"
#include "zstring.h"
#include "point.h"
#include "rect.h"
#include "vector.h"
#include "vertex.h"
#include "orientation.h"
#include "matrix.h"
#include "quaternion.h"
#include "color.h"
#include "win32app.h"
#include "base.h"
#include "event.h"
#include "genericlist.h"

// Some AGC / COM headers
#include <BinString.h>
#include <WinApp.h>
#include <..\TCAtl\SimpleStream.h>

#endif // !defined(AFX_pchObjRef2String__INCLUDED_)