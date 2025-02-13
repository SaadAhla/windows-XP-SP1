/*++

Copyright (c) 1997  Microsoft Corporation

Module Name:

	SampPrnt.cpp

Abstract:

	This module contains a sample implementation of a Server
	Extension Object.  This sample merely prints the properties
	of the event it is given.

Author:

	Andy Jacobs     (andyj@microsoft.com)

Revision History:

	andyj   1/06/97        created

--*/


// SampPrnt.cpp : Implementation of DLL Exports.

// You will need the NT SUR Beta 2 SDK or VC 4.2 or higher in order to build 
// this project.  This is because you will need MIDL 3.00.15 or higher and new
// headers and libs.  If you have VC 4.2 installed, then everything should
// already be configured correctly.

// Note: Proxy/Stub Information
//              To build a separate proxy/stub DLL, 
//              <<TBD>>.

#include "stdafx.h"

#include "resource.h"
#include "SampPrnt.h"
#include "printer.h"

#include "SampPrnt_i.c" // OLE Definitions
#include "..\seo_i.c"

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
        OBJECT_ENTRY(CLSID_CSEOSamplePrintExt, CSEOSamplePrintExt)
END_OBJECT_MAP()


/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/) {

	if (dwReason == DLL_PROCESS_ATTACH) {
		_Module.Init(ObjectMap,hInstance);
		DisableThreadLibraryCalls(hInstance);
	}
	else if (dwReason == DLL_PROCESS_DETACH) {
		_Module.Term();
	}
	return (TRUE);    // ok
}


/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void) {

	return ((_Module.GetLockCount()==0)?S_OK:S_FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv) {

	return (_Module.GetClassObject(rclsid,riid,ppv));
}


/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void) {

	// registers object, typelib and all interfaces in typelib
	return (_Module.RegisterServer(TRUE));
}


/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void) {

	_Module.UnregisterServer();
	return (S_OK);
}

