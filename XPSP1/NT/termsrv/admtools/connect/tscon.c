//  Copyright (c) 1998-1999 Microsoft Corporation
/*************************************************************************
*
*  TSCON.C
*
*************************************************************************/

#include <stdio.h>
#include <windows.h>
// #include <ntddkbd.h>
// #include <ntddmou.h>
#include <winstaw.h>
#include <stdlib.h>
#include <utilsub.h>
#include <string.h>
#include <malloc.h>
#include <locale.h>

#include "tscon.h"
#include "printfoa.h"

HANDLE hServerName = SERVERNAME_CURRENT;
WCHAR  ServerName[MAX_IDS_LEN+1];
WINSTATIONNAME Source;
WINSTATIONNAME Destination;
WCHAR Password[ PASSWORD_LENGTH + 1 ];
USHORT help_flag = FALSE;
USHORT v_flag    = FALSE;

TOKMAP ptm[] =
{
#define TERM_PARM 0
   {TOKEN_SOURCE,       TMFLAG_REQUIRED, TMFORM_S_STRING,
                            WINSTATIONNAME_LENGTH,  Source},

  /* { TOKEN_SERVER,      TMFLAG_OPTIONAL, TMFORM_STRING,
                            MAX_IDS_LEN, ServerName}, */

   {TOKEN_DESTINATION,  TMFLAG_OPTIONAL, TMFORM_X_STRING,
                            WINSTATIONNAME_LENGTH, Destination},

   {TOKEN_PASSWORD,     TMFLAG_OPTIONAL, TMFORM_X_STRING,
                            PASSWORD_LENGTH, Password},

   {TOKEN_HELP,         TMFLAG_OPTIONAL, TMFORM_BOOLEAN,
                            sizeof(USHORT), &help_flag},

   {TOKEN_VERBOSE,      TMFLAG_OPTIONAL, TMFORM_BOOLEAN,
                            sizeof(USHORT), &v_flag},

   {0, 0, 0, 0, 0}
};


/*
 * Local function prototypes.
 */
void Usage( BOOLEAN bError );


/*************************************************************************
*
*  main
*     Main function and entry point of the TSCON utility.
*
*  ENTRY:
*     argc  - count of the command line arguments.
*     argv  - vector of strings containing the command line arguments.
*
*  EXIT
*     Nothing.
*
*************************************************************************/

int __cdecl
main(INT argc, CHAR **argv)
{
    BOOLEAN bCurrent = FALSE;
    int   rc, i;
    WCHAR *CmdLine;
    WCHAR **argvW, *endptr;
    ULONG SourceId, DestId;

    setlocale(LC_ALL, ".OCP");

    /*
     *  Massage the command line.
     */

    argvW = MassageCommandLine((DWORD)argc);
    if (argvW == NULL) {
        ErrorPrintf(IDS_ERROR_MALLOC);
        return(FAILURE);
    }

    /*
     *  parse the cmd line without parsing the program name (argc-1, argv+1)
     */
    rc = ParseCommandLine(argc-1, argvW+1, ptm, 0);

    /*
     *  Check for error from ParseCommandLine
     */
    if ( help_flag || rc ) {

        if ( !help_flag ) {

            Usage(TRUE);
            return(FAILURE);

        } else {

            Usage(FALSE);
            return(SUCCESS);
        }
    }

        // If SERVER or DEST are not specified, we need to run on TS
    // Check if we are running under Terminal Server
        if ( ( (!IsTokenPresent(ptm, TOKEN_SERVER) )
        || (!IsTokenPresent(ptm, TOKEN_DESTINATION)) )
        && (!AreWeRunningTerminalServices()) )
        {
            ErrorPrintf(IDS_ERROR_NOT_TS);
            return(FAILURE);
        }
    /*
     * Open the specified server
     */
    if( ServerName[0] ) {
        hServerName = WinStationOpenServer( ServerName );
        if( hServerName == NULL ) {
            StringErrorPrintf(IDS_ERROR_SERVER,ServerName);
            PutStdErr( GetLastError(), 0 );
            return(FAILURE);
        }
    }

    /*
     * Validate the source.
     */
    if ( !IsTokenPresent(ptm, TOKEN_SOURCE) ) {

        /*
         * No source specified; use current winstation.
         */
        SourceId = GetCurrentLogonId();

    } else if ( !iswdigit(*Source) ) {

        /*
         * Treat the source string as a WinStation name.
         */
        if ( !LogonIdFromWinStationName(hServerName, Source, &SourceId) ) {
            StringErrorPrintf(IDS_ERROR_WINSTATION_NOT_FOUND, Source);
            return(FAILURE);
        }

    } else {

        /*
         * Treat the source string as a LogonId.
         */
        SourceId = wcstoul(Source, &endptr, 10);
        if ( *endptr ) {
            StringErrorPrintf(IDS_ERROR_INVALID_LOGONID, Source);
            return(FAILURE);
        }
        if ( !WinStationNameFromLogonId(hServerName, SourceId, Source) ) {
            ErrorPrintf(IDS_ERROR_LOGONID_NOT_FOUND, SourceId);
            return(FAILURE);
        }
    }

    /*
     * Validate the destination.
     */
    if ( !IsTokenPresent(ptm, TOKEN_DESTINATION) ) {

        /*
         * No destination specified; use current winstation.
         */
        bCurrent = TRUE;
        DestId = GetCurrentLogonId();
        if ( !WinStationNameFromLogonId(hServerName, DestId, Destination) ) {
            ErrorPrintf(IDS_ERROR_CANT_GET_CURRENT_WINSTATION, GetLastError());
            PutStdErr(GetLastError(), 0);
            return(FAILURE);
        }

    } else {

        /*
         * Validate the destination WinStation name.
         */
        if ( !LogonIdFromWinStationName(hServerName, Destination, &DestId) ) {
            StringErrorPrintf(IDS_ERROR_WINSTATION_NOT_FOUND, Destination);
            return(FAILURE);
        }
    }

    /*
     * Perform the connect.
     */
    if ( v_flag )
        DwordStringMessage(IDS_WINSTATION_CONNECT, SourceId, Destination);

    if ( !WinStationConnect(hServerName, SourceId, DestId, Password, TRUE) ) {

        if ( bCurrent )
            ErrorPrintf(IDS_ERROR_WINSTATION_CONNECT_CURRENT,
                         SourceId, GetLastError());
        else
            ErrorPrintf(IDS_ERROR_WINSTATION_CONNECT,
                         SourceId, Destination, GetLastError());
        PutStdErr(GetLastError(), 0);
        return(FAILURE);
    }

    return(SUCCESS);

} /* main() */


/*******************************************************************************
 *
 *  Usage
 *
 *      Output the usage message for this utility.
 *
 *  ENTRY:
 *      bError (input)
 *          TRUE if the 'invalid parameter(s)' message should preceed the usage
 *          message and the output go to stderr; FALSE for no such error
 *          string and output goes to stdout.
 *
 *  EXIT:
 *
 *
 ******************************************************************************/

void
Usage( BOOLEAN bError )
{
    if ( bError ) {
        ErrorPrintf(IDS_ERROR_INVALID_PARAMETERS);
        ErrorPrintf(IDS_USAGE_1);
        ErrorPrintf(IDS_USAGE_2);
        ErrorPrintf(IDS_USAGE_3);
        ErrorPrintf(IDS_USAGE_4);
        ErrorPrintf(IDS_USAGE_5);
        // ErrorPrintf(IDS_USAGE_6);
        ErrorPrintf(IDS_USAGE_7);
        ErrorPrintf(IDS_USAGE_8);
        ErrorPrintf(IDS_USAGE_9);
    }
    else{
        Message(IDS_USAGE_1);
        Message(IDS_USAGE_2);
        Message(IDS_USAGE_3);
        Message(IDS_USAGE_4);
        Message(IDS_USAGE_5);
        //Message(IDS_USAGE_6);
        Message(IDS_USAGE_7);
        Message(IDS_USAGE_8);
        Message(IDS_USAGE_9);
    }

} /* Usage() */

