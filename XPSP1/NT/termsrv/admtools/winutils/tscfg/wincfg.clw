//Copyright (c) 1998 - 1999 Microsoft Corporation
; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CClientSettingsDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "wincfg.h"
VbxHeaderFile=wincfg.h
VbxImplementationFile=wincfg.cpp
LastPage=0

ClassCount=14
Class1=CAdvancedNASIDlg
Class2=CAppServerDoc
Class3=CAppServerView
Class4=CEchoEditControl
Class5=CAsyncTestDlg
Class6=CAppServerOpenDlg
Class7=CAdvancedWinStationDlg
Class8=CClientSettingsDlg
Class9=CKeyboardDlg
Class10=CAdvancedAsyncDlg
Class11=CEditWinStationDlg
Class12=CMainFrame
Class13=COptionsDlg
Class14=CWincfgApp

ResourceCount=10
Resource1=IDR_MAINFRAME
Resource2=IDD_NASI_ADVANCED
Resource3=IDD_ASYNC_TEST
Resource4=IDD_ADVANCED_WINSTATION
Resource5=IDD_ASYNC_ADVANCED
Resource6=IDD_CLIENT_SETTINGS
Resource7=IDD_OPSTATUS
Resource8=IDR_POPUP
Resource9=IDD_OPTDLG
Resource10=IDD_EDIT_WINSTATION

[CLS:CAdvancedNASIDlg]
Type=0
HeaderFile=anasidlg.h
ImplementationFile=anasidlg.cpp

[CLS:CAppServerDoc]
Type=0
HeaderFile=appsvdoc.h
ImplementationFile=appsvdoc.cpp

[CLS:CAppServerView]
Type=0
HeaderFile=appsvvw.h
ImplementationFile=appsvvw.cpp

[CLS:CEchoEditControl]
Type=0
HeaderFile=atdlg.h
ImplementationFile=atdlg.cpp

[CLS:CAsyncTestDlg]
Type=0
HeaderFile=atdlg.h
ImplementationFile=atdlg.cpp

[CLS:CAppServerOpenDlg]
Type=0
HeaderFile=dialogs.h
ImplementationFile=dialogs.cpp

[CLS:CAdvancedWinStationDlg]
Type=0
HeaderFile=dialogs.h
ImplementationFile=dialogs.cpp

[CLS:CClientSettingsDlg]
Type=0
HeaderFile=dialogs.h
ImplementationFile=dialogs.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_CS_MAPPING_AUDIO

[CLS:CKeyboardDlg]
Type=0
HeaderFile=dialogs.h
ImplementationFile=dialogs.cpp

[CLS:CAdvancedAsyncDlg]
Type=0
HeaderFile=dialogs.h
ImplementationFile=dialogs.cpp
LastObject=CAdvancedAsyncDlg

[CLS:CEditWinStationDlg]
Type=0
HeaderFile=ewsdlg.h
ImplementationFile=ewsdlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_NETWORK_LANADAPTER

[CLS:CMainFrame]
Type=0
HeaderFile=mainfrm.h
ImplementationFile=mainfrm.cpp

[CLS:COptionsDlg]
Type=0
HeaderFile=optdlg.h
ImplementationFile=optdlg.cpp

[CLS:CWincfgApp]
Type=0
HeaderFile=wincfg.h
ImplementationFile=wincfg.cpp

[DLG:IDD_NASI_ADVANCED]
Type=1
Class=CAdvancedNASIDlg
ControlCount=10
Control1=IDL_NASI_ADVANCED_SESSIONTYPE,button,1342308359
Control2=IDC_NASI_ADVANCED_PRIVATESESSION,button,1342373897
Control3=IDC_NASI_ADVANCED_GLOBALSESSION,button,1342177289
Control4=IDL_NASI_ADVANCED_FILESERVER,static,1342308352
Control5=IDC_NASI_ADVANCED_FILESERVER,edit,1350631552
Control6=IDL_NASI_ADVANCED_SESSIONNAME,static,1342308352
Control7=IDC_NASI_ADVANCED_SESSIONNAME,edit,1350631552
Control8=IDOK,button,1342242817
Control9=IDCANCEL,button,1342242816
Control10=ID_HELP,button,1342242816

[DLG:IDD_ASYNC_TEST]
Type=1
Class=CAsyncTestDlg
ControlCount=25
Control1=IDC_STATIC,button,1342308359
Control2=IDC_ATDLG_DTR,static,1342177287
Control3=IDC_ATDLG_RTS,static,1342177287
Control4=IDC_ATDLG_CTS,static,1342177287
Control5=IDC_ATDLG_DSR,static,1342177287
Control6=IDC_ATDLG_DCD,static,1342177287
Control7=IDC_ATDLG_RI,static,1342177287
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDL_ATDLG_DEVICE,static,1342308352
Control17=IDL_ATDLG_BAUD,static,1342308352
Control18=IDC_ATDLG_EDIT,edit,1353915524
Control19=IDC_ATDLG_MODEM_INIT,button,1342373888
Control20=IDC_ATDLG_MODEM_LISTEN,button,1342242816
Control21=IDC_ATDLG_MODEM_DIAL,button,1342242816
Control22=IDL_ATDLG_PHONE_NUMBER,static,1342308352
Control23=IDC_ATDLG_PHONE_NUMBER,edit,1350631552
Control24=IDOK,button,1342373888
Control25=ID_HELP,button,1342242816

[DLG:IDD_ADVANCED_WINSTATION]
Type=1
Class=CAdvancedWinStationDlg
ControlCount=55
Control1=IDL_AWS_WSSTATE,button,1342308359
Control2=IDC_AWS_WSDISABLED,button,1342373897
Control3=IDC_AWS_WSENABLED,button,1342177289
Control4=IDL_AWS_TIMEOUTS,button,1342308359
Control5=IDL_AWS_CONNECTION,static,1342308354
Control6=IDC_AWS_CONNECTION,edit,1350631552
Control7=IDC_AWS_CONNECTION_NONE,button,1342373891
Control8=IDC_AWS_CONNECTION_INHERIT,button,1342373891
Control9=IDL_AWS_DISCONNECTION,static,1342308354
Control10=IDC_AWS_DISCONNECTION,edit,1350631552
Control11=IDC_AWS_DISCONNECTION_NONE,button,1342373891
Control12=IDC_AWS_DISCONNECTION_INHERIT,button,1342373891
Control13=IDL_AWS_IDLE,static,1342308354
Control14=IDC_AWS_IDLE,edit,1350631552
Control15=IDC_AWS_IDLE_NONE,button,1342373891
Control16=IDC_AWS_IDLE_INHERIT,button,1342373891
Control17=IDL_AWS_SECURITY,button,1342177287
Control18=IDL_AWS_SECURITY_ENCRYPT1,static,1342308354
Control19=IDC_AWS_SECURITY_ENCRYPT,combobox,1344339971
Control20=IDC_AWS_SECURITY_DEFAULTGINA,button,1342242819
Control21=IDL_AWS_AUTOLOGON,button,1342308359
Control22=IDL_AWS_AUTOLOGON_USERNAME,static,1342308354
Control23=IDC_AWS_AUTOLOGON_USERNAME,edit,1350762624
Control24=IDL_AWS_AUTOLOGON_DOMAIN,static,1342308354
Control25=IDC_AWS_AUTOLOGON_DOMAIN,edit,1350631552
Control26=IDL_AWS_AUTOLOGON_PASSWORD,static,1342308354
Control27=IDC_AWS_AUTOLOGON_PASSWORD,edit,1350631584
Control28=IDL_AWS_AUTOLOGON_CONFIRM_PASSWORD,static,1342308354
Control29=IDC_AWS_AUTOLOGON_CONFIRM_PASSWORD,edit,1350631584
Control30=IDC_AWS_AUTOLOGON_PASSWORD_PROMPT,button,1342242851
Control31=IDC_AWS_AUTOLOGON_INHERIT,button,1342373891
Control32=IDL_AWS_INITIALPROGRAM,button,1342308359
Control33=IDL_AWS_INITIALPROGRAM_COMMANDLINE1,static,1342308354
Control34=IDL_AWS_INITIALPROGRAM_COMMANDLINE2,static,1342308354
Control35=IDC_AWS_INITIALPROGRAM_COMMANDLINE,edit,1350631552
Control36=IDL_AWS_INITIALPROGRAM_WORKINGDIRECTORY1,static,1342308354
Control37=IDL_AWS_INITIALPROGRAM_WORKINGDIRECTORY2,static,1342308354
Control38=IDC_AWS_INITIALPROGRAM_WORKINGDIRECTORY,edit,1350631552
Control39=IDC_AWS_INITIALPROGRAM_INHERIT,button,1342373891
Control40=IDC_AWS_INITIALPROGRAM_PUBLISHEDONLY,button,1342242819
Control41=IDL_AWS_USEROVERRIDES,button,1342308359
Control42=IDC_AWS_USEROVERRIDE_DISABLEWALLPAPER,button,1342242819
Control43=IDL_AWS_BROKEN1,static,1342308352
Control44=IDC_AWS_BROKEN,combobox,1344339971
Control45=IDL_AWS_BROKEN2,static,1342308352
Control46=IDC_AWS_BROKEN_INHERIT,button,1342373891
Control47=IDL_AWS_RECONNECT1,static,1342308352
Control48=IDC_AWS_RECONNECT,combobox,1344339971
Control49=IDC_AWS_RECONNECT_INHERIT,button,1342373891
Control50=IDL_AWS_SHADOW,static,1342308352
Control51=IDC_AWS_SHADOW,combobox,1344339971
Control52=IDC_AWS_SHADOW_INHERIT,button,1342373891
Control53=IDOK,button,1342373889
Control54=IDCANCEL,button,1342242816
Control55=ID_HELP,button,1342242816

[DLG:IDD_CLIENT_SETTINGS]
Type=1
Class=CClientSettingsDlg
ControlCount=15
Control1=IDL_CS_CONNECTION,button,1342308359
Control2=IDC_CS_CONNECTION_DRIVES,button,1342242819
Control3=IDC_CS_CONNECTION_PRINTERS,button,1342242819
Control4=IDC_CS_CONNECTION_FORCEPRTDEF,button,1342242819
Control5=IDC_CS_CONNECTION_INHERIT,button,1342373923
Control6=IDL_CS_MAPPING,button,1342308359
Control7=IDC_CS_MAPPING_DRIVES,button,1342242819
Control8=IDC_CS_MAPPING_WINDOWSPRINTERS,button,1342242819
Control9=IDC_CS_MAPPING_DOSLPTS,button,1342242819
Control10=IDC_CS_MAPPING_COMPORTS,button,1342242819
Control11=IDC_CS_MAPPING_CLIPBOARD,button,1342242819
Control12=IDC_CS_MAPPING_AUDIO,button,1342242819
Control13=IDOK,button,1342373889
Control14=IDCANCEL,button,1342242816
Control15=ID_HELP,button,1342242816

[DLG:IDD_ASYNC_ADVANCED]
Type=1
Class=CAdvancedAsyncDlg
ControlCount=40
Control1=IDL_ASYNC_ADVANCED_FLOWCONTROL,button,1342308359
Control2=IDC_ASYNC_ADVANCED_FLOWCONTROL_HARDWARE,button,1342373897
Control3=IDC_ASYNC_ADVANCED_FLOWCONTROL_SOFTWARE,button,1342177289
Control4=IDC_ASYNC_ADVANCED_FLOWCONTROL_NONE,button,1342177289
Control5=IDL_ASYNC_ADVANCED_HARDWARE,button,1342308359
Control6=IDC_ASYNC_ADVANCED_HWRX,combobox,1344339971
Control7=IDL_ASYNC_ADVANCED_HWRX,static,1342177280
Control8=IDL_ASYNC_ADVANCED_HWTX,static,1342177282
Control9=IDC_ASYNC_ADVANCED_HWTX,combobox,1344339971
Control10=IDL_ASYNC_ADVANCED_SOFTWARE,button,1342308359
Control11=IDL_ASYNC_ADVANCED_XON,static,1342177282
Control12=IDC_ASYNC_ADVANCED_XON,edit,1350631552
Control13=IDL_ASYNC_ADVANCED_XOFF,static,1342177282
Control14=IDC_ASYNC_ADVANCED_XOFF,edit,1350631552
Control15=IDL_ASYNC_ADVANCED_BASE,button,1342177287
Control16=IDC_ASYNC_ADVANCED_BASEDEC,button,1342373897
Control17=IDC_ASYNC_ADVANCED_BASEHEX,button,1342177289
Control18=IDL_ASYNC_ADVANCED_DTRSTATE,button,1342308359
Control19=IDL_ASYNC_ADVANCED_DTRALWAYS,static,1342177282
Control20=IDC_ASYNC_ADVANCED_DTROFF,button,1342373897
Control21=IDC_ASYNC_ADVANCED_DTRON,button,1342177289
Control22=IDL_ASYNC_ADVANCED_RTSSTATE,button,1342308359
Control23=IDL_ASYNC_ADVANCED_RTSALWAYS,static,1342177282
Control24=IDC_ASYNC_ADVANCED_RTSOFF,button,1342373897
Control25=IDC_ASYNC_ADVANCED_RTSON,button,1342177289
Control26=IDL_ASYNC_ADVANCED_PARITY,button,1342308359
Control27=IDC_ASYNC_ADVANCED_PARITY_NONE,button,1342373897
Control28=IDC_ASYNC_ADVANCED_PARITY_EVEN,button,1342177289
Control29=IDC_ASYNC_ADVANCED_PARITY_ODD,button,1342177289
Control30=IDC_ASYNC_ADVANCED_PARITY_MARK,button,1342177289
Control31=IDC_ASYNC_ADVANCED_PARITY_SPACE,button,1342177289
Control32=IDL_ASYNC_ADVANCED_STOPBITS,button,1342308359
Control33=IDC_ASYNC_ADVANCED_STOPBITS_1,button,1342373897
Control34=IDC_ASYNC_ADVANCED_STOPBITS_2,button,1342177289
Control35=IDL_ASYNC_ADVANCED_BYTESIZE,button,1342308359
Control36=IDC_ASYNC_ADVANCED_BYTESIZE_7,button,1342373897
Control37=IDC_ASYNC_ADVANCED_BYTESIZE_8,button,1342177289
Control38=IDOK,button,1342373889
Control39=IDCANCEL,button,1342242816
Control40=ID_HELP,button,1342242816

[DLG:IDD_EDIT_WINSTATION]
Type=1
Class=CEditWinStationDlg
ControlCount=56
Control1=IDL_WINSTATIONNAME,static,1342308354
Control2=IDC_WINSTATIONNAME,edit,1350762624
Control3=IDL_WDNAME,static,1342308354
Control4=IDC_WDNAME,combobox,1344340035
Control5=IDL_TDNAME,static,1342308354
Control6=IDC_TDNAME,combobox,1344340291
Control7=IDL_WSCOMMENT,static,1342308354
Control8=IDC_WSCOMMENT,edit,1350631552
Control9=IDL_ASYNC,button,1208090631
Control10=IDL_ASYNC_DEVICENAME,static,1207959554
Control11=IDC_ASYNC_DEVICENAME,combobox,1210122499
Control12=IDC_ASYNC_MODEMINSTALL,button,1208156160
Control13=IDC_ASYNC_MODEMCONFIG,button,1208156160
Control14=IDL_ASYNC_MODEMCALLBACK,button,1207959559
Control15=IDL_ASYNC_MODEMCALLBACK1,static,1208090624
Control16=IDC_ASYNC_MODEMCALLBACK,combobox,1210122243
Control17=IDC_ASYNC_MODEMCALLBACK_INHERIT,button,1208156163
Control18=IDL_ASYNC_MODEMCALLBACK_PHONENUMBER,static,1208090624
Control19=IDC_ASYNC_MODEMCALLBACK_PHONENUMBER,edit,1216413824
Control20=IDC_ASYNC_MODEMCALLBACK_PHONENUMBER_INHERIT,button,1208156163
Control21=IDL_ASYNC_CONNECT,static,1208090626
Control22=IDC_ASYNC_CONNECT,combobox,1210122243
Control23=IDL_ASYNC_BAUDRATE,static,1207959554
Control24=IDC_ASYNC_BAUDRATE,combobox,1210122242
Control25=IDC_ASYNC_DEFAULTS,button,1208156160
Control26=IDC_ASYNC_ADVANCED,button,1208025088
Control27=IDC_ASYNC_TEST,button,1208025088
Control28=IDL_NETWORK,button,1207959559
Control29=IDL_NETWORK_LANADAPTER,static,1208090626
Control30=IDC_NETWORK_LANADAPTER,combobox,1210122499
Control31=IDL_NETWORK_INSTANCECOUNT,static,1208090626
Control32=IDC_NETWORK_INSTANCECOUNT,edit,1216413824
Control33=IDC_NETWORK_INSTANCECOUNT_UNLIMITED,button,1208025091
Control34=IDL_OEMTD,button,1207959559
Control35=IDL_OEMTD_DEVICENAME,static,1208090624
Control36=IDC_OEMTD_DEVICENAME,combobox,1210122499
Control37=IDL_OEMTD_INSTANCECOUNT,static,1208090626
Control38=IDC_OEMTD_INSTANCECOUNT,edit,1216413824
Control39=IDC_OEMTD_INSTANCECOUNT_UNLIMITED,button,1208025091
Control40=IDL_NASI,button,1207959559
Control41=IDL_NASI_USERNAME,static,1208090626
Control42=IDC_NASI_USERNAME,edit,1216413824
Control43=IDL_NASI_PASSWORD,static,1208090626
Control44=IDC_NASI_PASSWORD,edit,1216413856
Control45=IDL_NASI_PORTNAME,static,1208090626
Control46=IDC_NASI_PORTNAME,combobox,1210122498
Control47=IDL_NASI_INSTANCECOUNT,static,1208090626
Control48=IDC_NASI_INSTANCECOUNT,edit,1216413824
Control49=IDC_NASI_INSTANCECOUNT_UNLIMITED,button,1208025091
Control50=IDC_NASI_ADVANCED,button,1208156160
Control51=IDC_ADVANCED_WINSTATION,button,1342242816
Control52=IDC_CLIENT_SETTINGS,button,1342242816
Control53=IDC_EXTENSION_BUTTON,button,1073807360
Control54=IDOK,button,1342242817
Control55=IDCANCEL,button,1342242816
Control56=ID_HELP,button,1342242816

[DLG:IDD_OPTDLG]
Type=1
Class=COptionsDlg
ControlCount=11
Control1=IDC_STATIC,static,1342308353
Control2=IDC_STATIC,static,1342177280
Control3=IDL_OPTDLG_HELP_SWITCH,static,1342177280
Control4=IDC_STATIC,static,1342177280
Control5=IDL_OPTDLG_REGISTRYONLY_SWITCH,static,1342177280
Control6=IDC_STATIC,static,1342177280
Control7=IDL_OPTDLG_ADD_SWITCH,static,1342177280
Control8=IDC_STATIC,static,1342177280
Control9=IDL_OPTDLG_COUNT_SWITCH,static,1342177280
Control10=IDC_STATIC,static,1342177280
Control11=IDOK,button,1342242817

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_WINSTATION_ADD
Command2=ID_WINSTATION_COPY
Command3=ID_WINSTATION_DELETE
Command4=ID_WINSTATION_RENAME
Command5=ID_WINSTATION_EDIT
Command6=ID_WINSTATION_ENABLE
Command7=ID_WINSTATION_DISABLE
Command8=ID_APP_EXIT
Command9=ID_OPTIONS_CONFIRMATION
Command10=ID_OPTIONS_SAVE_SETTINGS_ON_EXIT
Command11=ID_OPTIONS_FONT
Command12=ID_SECURITY_PERMISSIONS
Command13=ID_HELP_INDEX
Command14=ID_HELP_SEARCH_FOR
Command15=ID_HELP_USING
Command16=ID_APP_ABOUT
CommandCount=16

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_WINSTATION_DELETE
Command2=ID_WINSTATION_DISABLE
Command3=ID_WINSTATION_NEXT
Command4=ID_HELP
Command5=ID_WINSTATION_COPY
Command6=ID_WINSTATION_ADD
Command7=ID_WINSTATION_ENABLE
Command8=ID_WINSTATION_EDIT
Command9=ID_WINSTATION_PREV
CommandCount=9

[DLG:IDD_OPSTATUS]
Type=1
Class=?
ControlCount=3
Control1=IDI_OPSTATUS_ICON,static,1342308355
Control2=IDL_OPSTATUS_CURRENTOP,static,1342308352
Control3=IDC_OPSTATUS_ABORT,button,1342242816

[MNU:IDR_POPUP]
Type=1
Command1=ID_WINSTATION_COPY
Command2=ID_WINSTATION_DELETE
Command3=ID_WINSTATION_RENAME
Command4=ID_WINSTATION_EDIT
Command5=ID_WINSTATION_ENABLE
Command6=ID_WINSTATION_DISABLE
Command7=ID_SECURITY_PERMISSIONS
CommandCount=7

