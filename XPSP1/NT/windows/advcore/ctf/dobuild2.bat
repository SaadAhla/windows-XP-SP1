
if (%1)==() goto ERROR
if (%2)==() goto ERROR

Echo ***************************************************************
Echo Makesure you have given correct version no like 2260.0 
Echo Please specify the .0 , its necessary for Setup packages
Echo ***************************************************************
pause 

C:
cd %SDXROOT%\Windows\advcore\%2
call %SDXROOT%\TOOLS\RAZZLE.CMD FREE
set __BUILDMACHINE__=Lab06_N
REM BBT ADDs

REM deleting old binaries
rd /q /s \binaries.x86fre

rem Generate codeview debug syms.
set NTDBGFiles=
set NTBBT=1

REM End BBT ADDs

call Build -cZ
call qdump %2
call nonbbtcp.bat %2 %1

cd %SDXROOT%\Windows\advcore\%2
call dobbt.bat %1 %2

cd %SDXROOT%\Windows\advcore\%2
call %SDXROOT%\TOOLS\RAZZLE.CMD no_opt
set __BUILDMACHINE__=Lab06_N
set BUILD_ALT_DIR=d
call Build -cZ
call qdump %2 d


cd C:\nt6\Windows\advcore\CTF\setup
call cdrop.bat
call cdropd.bat

md \\CICEROBM\EXE\%1
copy uim*.exe \\CICEROBM\EXE\%1

md \\CICEROBM\EXE\%1\SYMBOLS\DEBUG

cd c:\nt6\Windows\advcore\CTF\setup\srcd
copy *.pdb \\CICEROBM\EXE\%1\SYMBOLS\DEBUG
copy *.sym \\CICEROBM\EXE\%1\SYMBOLS\DEBUG
call ..\resetpdbd.bat %1

Echo ***************************************************************
Echo Please make 64bit version & then copy the Sanp
Echo ***************************************************************
pause

Echo ***************************************************************
Echo Copying files to Snap Folder..Continue ?
Echo ***************************************************************
pause

cd %SDXROOT%\Windows\advcore\%2
snapit %1

goto END

:ERROR

Echo Usage DoBuild VersionNo CTFTree
Echo Like DoBuild 1428.2 CTF.beta1
Echo OR Like DoBuild 1428.2 CTF

:END
