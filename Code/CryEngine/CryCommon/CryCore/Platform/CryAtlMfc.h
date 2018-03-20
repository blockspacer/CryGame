// Copyright 2001-2017 Crytek GmbH / Crytek Group. All rights reserved. 

#pragma once

// platform.h sets _WIN32_WINNT, NTDDI_VERSION and WINVER, which need to be set before including any of the ATL/MFC files.
#if !defined(CRY_PLATFORM)
	#error Must include platform.h first
#endif
#if !CRY_PLATFORM_WINDOWS
	#error This header can only be used on Windows
#endif

#define CRY_INCLUDE_WINDOWS_VIA_MFC_OR_ATL_INCLUDES

// For identical behavior as CryWindows.h
#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif

// min and max macro's are in use by MFC, ATL and XT
// Therefore, we just undefine them later on instead of using NOMINMAX
#undef NOMINMAX

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

// Remember previous warning settings
#pragma warning(push)

// Disable warnings generated by MFC headers
#pragma warning(disable: 4103) // 'filename' : alignment changed after including header, may be due to missing #pragma pack(pop)
#pragma warning(disable: 4263) // 'function' : member function does not override any base class virtual member function
#pragma warning(disable: 4264) // 'virtual_function' : no override available for virtual member function from base 'class'; function is hidden
#pragma warning(disable: 4266) // 'function' : no override available for virtual member function from base 'type'; function is hidden

#if defined(CRY_USE_MFC) || defined(CRY_USE_XT)
// MFC goes first, because it will #error otherwise
	#define _AFX_ALL_WARNINGS
	#include <afx.h>
	#include <afxwin.h>
	#include <afxext.h>
	#include <afxdialogex.h>
	#include <afxole.h>
	#include <afxodlgs.h>
	#include <afxdisp.h>
	#include <afxcmn.h>
	#include <afxwinappex.h>
	#include <afxcolorbutton.h>
	#include <afxcolordialog.h>
	#include <afxcolorpropertysheet.h>
	#include <afxeditbrowsectrl.h>
	#undef CRY_USE_MFC
	#undef _AFX_ALL_WARNINGS

// WinVista common controls
	#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")

	#if defined(USE_CRY_ASSERT)
// Request MFC assert macro's to be remapped to CRY_ASSERT
		#define CRY_REMAP_MFC_ASSERT
	#endif
#endif

#if defined(CRY_USE_ATL)
// ATL goes after MFC, but before XT
	#include <atlbase.h>
	#include <atlctl.h>
	#include <atltypes.h>
	#include <atlimage.h>
	#include <atlwin.h>
	#include <atlhost.h>
	#undef CRY_USE_ATL
#endif

#if defined(CRY_USE_XT)
// XT goes after MFC and ATL
	#if _MSC_VER >= 1900
		#define _XTPLIB_VISUALSTUDIO_VERSION "vc140"
	#elif _MSC_VER >= 1800
		#define _XTPLIB_VISUALSTUDIO_VERSION "vc120"
	#elif _MSC_VER >= 1700
		#define _XTPLIB_VISUALSTUDIO_VERSION "vc110"
	#endif
	#define _XTPLIB_NOAUTOLINK
	#define _XTP_INCLUDE_DEPRECATED
	#include <XTToolKitPro.h>

// We suppressed auto-link from XT header because it spams the compilation log
// Instead, we link it here in the same way, but without #pragma message
	#if defined(_DEBUG)
		#define _XTPLIB_DEBUG "D"
	#else
		#define _XTPLIB_DEBUG ""
	#endif
	#pragma comment(lib, "ToolkitPro1340" _XTPLIB_VISUALSTUDIO_VERSION "x64" _XTPLIB_DEBUG ".lib")

// Clean up XT macro's
	#undef _XTPLIB_NOAUTOLINK
	#undef _XTP_INCLUDE_DEPRECATED
	#undef _XTPLIB_VISUALSTUDIO_VERSION
	#undef _XTPLIB_DEBUG
	#undef CRY_USE_XT
#endif

// Undefine function-names used in CryEngine interfaces.
// This does the same as CryWindows.h.
// However, we allow the suppression of this logic for use by Sandbox, which relies on renaming.
#if !defined(CRY_SUPPRESS_CRYENGINE_WINDOWS_FUNCTION_RENAMING)
	#undef GetCommandLine
	#undef GetObject
	#undef PlaySound
	#undef GetClassName
	#undef DrawText
	#undef GetCharWidth
	#undef GetUserName
	#undef LoadLibrary
#endif

// Make sure windows.h has had it's chance to define min and max.
// It's possible that neither CRY_MFC, CRY_ATL or CRY_XT is set, in which case we would miss that.
#include <CryCore/Platform/CryWindows.h>
#undef _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#undef CRY_SUPPRESS_CRYENGINE_WINDOWS_FUNCTION_RENAMING
// Because we did #undef NOMINMAX, we have to clean min/max macro's up.
// Otherwise, they can conflict with std::min and std::max.
#undef min
#undef max

// XT pulls in the standard assert.
// We should restore CRY_ASSERT behavior.
#include <CryCore/Assert/CryAssert.h>

// Replacement for MFC's ASSERT, ASSERT_KINDOF and ASSERT_VALID macro's
#if defined(CRY_REMAP_MFC_ASSERT)
	#undef ASSERT
	#undef ASSERT_VALID
	#undef ASSERT_KINDOF
	#define ASSERT(x)           CRY_ASSERT_MESSAGE(x, "MFC ASSERT")
	#define ASSERT_KINDOF(t, p) CRY_ASSERT_MESSAGE(p && p->IsKindOf(RUNTIME_CLASS(t)), "MFC ASSERT_KINDOF")
	#ifdef _DEBUG
		#define ASSERT_VALID(x)   CRY_ASSERT_MESSAGE(x && (x->AssertValid(), true), "MFC ASSERT_VALID")
	#else
		#define ASSERT_VALID(x)   CRY_ASSERT_MESSAGE(x, "MFC ASSERT_VALID")
	#endif
	#undef CRY_REMAP_MFC_ASSERT
#endif

// Restore previous warning settings
// This re-enables all the warnings that were disabled for/by MFC/ATL/XT
#pragma warning(pop)