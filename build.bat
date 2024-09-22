cls
@echo off
pushd %~dp0
setlocal EnableDelayedExpansion
set path=%~dp0assets;%path%

set input=ascencia_win32.c win32\*.c util\*.c
set dwarnings=4100 4189 4201 4820 4711 4668 4114
set cflags=-TC -MP -Oi -FC -GR- -WX -W4 -DWIN32 -D_CRT_SECURE_NO_WARNINGS

where /q cl.exe || goto WIN32_SET_VCVARS
goto WIN32_BUILD_START

:WIN32_SET_VCVARS
echo == Loading vcvarsall.bat x64 ==
set vcvarsallfile=
if not exist "C:\Program Files\Microsoft Visual Studio\" goto WIN32_SET_VCVARS_CHECK_C
pushd "C:\Program Files\Microsoft Visual Studio\"
for /f "delims=" %%a in ('dir vcvarsall.bat /s /p /b') do set "vcvarsallfile=%%a"
popd
if exist "%vcvarsallfile%" call "%vcvarsallfile%" x64
where /q cl.exe || goto WIN32_ERROR_VS_INSTALL
goto WIN32_BUILD_START

:WIN32_SET_VCVARS_CHECK_C
echo Didn't find vcvarsall.bat in Program Files, doing deeper search...
set vcvarsallfile=
pushd "C:\"
for /f "delims=" %%a in ('dir vcvarsall.bat /s /p /b') do set "vcvarsallfile=%%a"
popd
if exist "%vcvarsallfile%" call "%vcvarsallfile%" x64
where /q cl.exe || goto WIN32_ERROR_VS_INSTALL
goto WIN32_BUILD_START

:WIN32_ERROR_VS_INSTALL
echo == Error: Couldn't find Visual Studio installation ==
popd
exit /b 1

:WIN32_BUILD_START
if not exist build mkdir build
if not exist build\int mkdir build\int
pushd build\int
del /q *.*

set dwarnings= %dwarnings%
set dwarnings=%dwarnings: = -wd%
set cflags=%cflags% %dwarnings%
set input= %input%
set input=%input: = ..\..\src\%

copy %~dp0src\win32\resource.rc %~dp0build\int\resource.rc
copy %~dp0src\win32\resource.h %~dp0build\int\resource.h
copy %~dp0assets\asc256.ico %~dp0build\int\asc256.ico
copy %~dp0assets\asc64.ico %~dp0build\int\asc64.ico
copy %~dp0assets\asc48.ico %~dp0build\int\asc48.ico
copy %~dp0assets\asc40.ico %~dp0build\int\asc40.ico
copy %~dp0assets\asc32.ico %~dp0build\int\asc32.ico
copy %~dp0assets\asc24.ico %~dp0build\int\asc24.ico
copy %~dp0assets\asc20.ico %~dp0build\int\asc20.ico
copy %~dp0assets\asc16.ico %~dp0build\int\asc16.ico
rc resource.rc

cl -I..\..\src -Fe"ascencia-d.exe" %cflags% -Od -Zi -DDEBUG=1 %input% -link -debug -subsystem:console kernel32.lib resource.res

cl -I..\..\src -Fe"ascencia.exe" %cflags% -O2 -DRELEASE=1 %input% -link -release -subsystem:windows kernel32.lib resource.res

popd

if not exist %~dp0build\debug mkdir %~dp0build\debug
del /q %~dp0build\debug\*.*
if exist %~dp0build\int\ascencia-d.exe copy %~dp0build\int\ascencia-d.exe %~dp0build\debug\ascencia-d.exe
robocopy %~dp0data %~dp0build\debug /S /J /NDL /NJH /NJS /nc /ns /np

if not exist %~dp0build\release mkdir %~dp0build\release
del /q %~dp0build\release\*.*
if exist %~dp0build\int\ascencia.exe copy %~dp0build\int\ascencia.exe %~dp0build\release\ascencia.exe
robocopy %~dp0data %~dp0build\release /S /J /NDL /NJH /NJS /nc /ns /np

if exist build\debug\ascencia-d.exe (
    echo build\debug\ascencia-d.exe    SUCCEEDED
) else (
    echo build\debug\ascencia-d.exe    FAILED
)

if exist build\release\ascencia.exe (
    echo build\release\ascencia.exe    SUCCEEDED
) else (
    echo build\release\ascencia.exe    FAILED
)

popd