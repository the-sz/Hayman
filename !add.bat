@echo off

rem set mydir to the current path
set mydir="%~p0"

rem replace \ with ;
set mydir=%mydir:\=;%

rem replace <space> with ;
set mydir=%mydir: =:%

rem find last path component
for /F "tokens=* delims=;" %%i IN (%mydir%) DO call :LAST_FOLDER %%i
goto :ADD

:LAST_FOLDER
if "%1"=="" (
  set LAST=%LAST::= %
  goto :EOF
)

set LAST=%1
SHIFT

goto :LAST_FOLDER

:ADD

echo.
echo Adding %LAST% to SymStore
echo.

set EXECUTABLE=%LAST%

set SOURCE_PATH=Release Unicode
rem set SOURCE_PATH=Release

set SYMSTORE_PATH=..\_SymStore\

set SYMSTORE_EXE=G:\Coding.TXT\Windows\WinDbg\WinDbg.6_11_1_404\symstore.exe

%SYMSTORE_EXE% add /r /f "%SOURCE_PATH%\%EXECUTABLE%.*" /s %SYMSTORE_PATH% /t %LAST%
