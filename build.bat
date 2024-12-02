@echo off
setlocal enabledelayedexpansion
pushd %~dp0
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64
if not exist build mkdir build

set INPUT=%~dp0code\win32_ascencia.c
set LINKS=shell32.lib user32.lib gdi32.lib opengl32.lib winmm.lib
set DEFS=-D_CRT_SECURE_NO_WARNINGS
set DEBUG=1
set FAST=1

::===================================================================================

set BASEOPTS=-nologo -MT -Gm- -GR- -EHa -Oi -FC
set OPTS=
set OPTS1=
set DEF=
set LINK=
if %DEBUG%==1 (
set OPTS1=%BASEOPTS% -Z7
set DEF=%DEFS% -DDEBUG
set LINK=-DEBUG %LINKS%
) else (
set OPTS1=%BASEOPTS%
set DEF=%DEFS% -DRELEASE
set LINK=%LINKS%
)
if %FAST%==1 (
set OPTS=%OPTS1% -O2
) else (
set OPTS=%OPTS1% -Od
)
if exist build rmdir /s /q build
mkdir build
pushd build
cl -Fe"Ascencia.exe" -I"%~dp0code" %OPTS% %DEF% %INPUT% -link -subsystem:windows %LINK%
if %DEBUG%==0 (
del /q *.obj
del /q *.ilk
del /q *.pdb
)
popd
if not exist %~dp0build\Ascencia.exe exit /b 1

::====================================================================================

set ACOPTS=-nologo -MT -Gm- -GR- -EHa -O2 -FC
set ACDEFS=-D_CRT_SECURE_NO_WARNINGS
set ACINPUT=%~dp0code\asset_compiler\asset_compiler.c
set ACLINK=shell32.lib user32.lib
pushd build
cl -Fe"ac.exe" -I"%~dp0code" %ACOPTS% %ACDEFS% %ACINPUT% -link -subsystem:console %ACLINK%
popd
if not exist %~dp0build\ac.exe exit /b 1

::====================================================================================

%~dp0build\ac.exe %~dp0assets %~dp0data
if %DEBUG%==0 (
pushd build
del /q *.obj
del /q *.ilk
del /q *.pdb
del /q ac.exe
popd
)

robocopy %~dp0data %~dp0build /S /J /NDL /NJH /NJS /nc /ns /np
if exist %~dp0rh\rh.exe %~dp0rh\rh.exe -open %~dp0build\Ascencia.exe -resource %~dp0rh\Ascencia.ico -mask ICONGROUP,MAINICON, -action addskip -save %~dp0build\Ascencia.exe

popd
