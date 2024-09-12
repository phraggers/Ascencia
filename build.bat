cls
@echo off
pushd %~dp0
setlocal EnableDelayedExpansion
set path=%~dp0assets;%path%

set input=*.c win32\*.c platform\*.c
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

cl -nologo -I..\..\src -Fe"ascencia-d.exe" %cflags% -Od -Zi -DDEBUG=1 %input% -link -debug -subsystem:console kernel32.lib

cl -nologo -I..\..\src -Fe"ascencia.exe" %cflags% -O2 -DRELEASE=1 %input% -link -release -subsystem:windows kernel32.lib

popd

:: It would obviously be a lot faster to just distribute ResourceHacker WITH
:: the build repo but that would go against AngusJ's licence agreement.
if not exist assets\rh.exe (
	if not exist assets\tmp mkdir assets\tmp
	bitsadmin.exe /transfer "ResourceHacker" https://www.angusj.com/resourcehacker/resource_hacker.zip %~dp0assets\tmp\rh.zip
	powershell "Expand-Archive -Path """%~dp0assets\tmp\rh.zip""" -DestinationPath """%~dp0assets\tmp\rh""""
	copy assets\tmp\rh\ResourceHacker.exe assets\rh.exe
	del /q assets\tmp\rh\*.*
	del /q assets\tmp\rh\help\*.*
	rmdir assets\tmp\rh\help
	del /q assets\tmp\rh\samples\*.*
	rmdir assets\tmp\rh\samples
	rmdir assets\tmp\rh\
	del /q assets\tmp\*.*
	rmdir assets\tmp
)

if not exist %~dp0build\debug mkdir %~dp0build\debug
del /q %~dp0build\debug\*.*
if exist %~dp0build\int\ascencia-d.exe copy %~dp0build\int\ascencia-d.exe %~dp0build\debug\ascencia-d.exe
rh -open %~dp0build\debug\ascencia-d.exe -resource %~dp0assets\ascencia.ico -mask ICONGROUP,MAINICON, -action addskip -save %~dp0build\debug\ascencia-d.exe
robocopy %~dp0data %~dp0build\debug /S /J /NDL /NJH /NJS /nc /ns /np

if not exist %~dp0build\release mkdir %~dp0build\release
del /q %~dp0build\release\*.*
if exist %~dp0build\int\ascencia.exe copy %~dp0build\int\ascencia.exe %~dp0build\release\ascencia.exe
rh -open %~dp0build\release\ascencia.exe -resource %~dp0assets\ascencia.ico -mask ICONGROUP,MAINICON, -action addskip -save %~dp0build\release\ascencia.exe
robocopy %~dp0data %~dp0build\release /S /J /NDL /NJH /NJS /nc /ns /np

echo.
echo ResourceHacker by angusj used to inject icon into .exe files.
echo Please go to https://www.angusj.com/resourcehacker/ for info.
echo.

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