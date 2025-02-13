/*++

Copyright (C) 1999-2001 Microsoft Corporation

Module Name:

Abstract:

History:

--*/

#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>

#define LOGFILE	"LOG.TXT"

// Logging macros

#define _LOGGING

#ifdef _LOGGING

#define INITLOG \
{\
DeleteFile(LOGFILE);\
}

#define LOG(msg) \
{\
FILE *f = fopen(LOGFILE, "a");\
SYSTEMTIME st;\
GetLocalTime(&st);\
fprintf(f, "%u\\%u\\%u %u:%u %s\n", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, msg);\
fclose(f);\
}

#define LOGERROR(msg) \
{\
FILE *f = fopen(LOGFILE, "a");\
SYSTEMTIME st;\
GetLocalTime(&st);\
fprintf(f, "%u\\%u\\%u %u:%2u **ERROR**: %s\n", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, msg);\
fclose(f);\
}

#else

#define INITLOG
#define LOG(msg)
#define ERROR(msg)

#endif // _LOGGING

#endif // _COMMON_H_