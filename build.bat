@echo off
setlocal enabledelayedexpansion
pushd %~dp0
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64
if not exist build mkdir build

set INPUT=%~dp0code\ascencia.c
set LINKS=shell32.lib user32.lib gdi32.lib opengl32.lib winmm.lib

set DOPTS=-nologo -MT -Gm- -GR- -EHa -Oi -Od -FC -Z7
set DDEFS=-D_CRT_SECURE_NO_WARNINGS -DDEBUG
set DLINK=-DEBUG %LINKS%

set ROPTS=-nologo -MT -Gm- -GR- -EHa -Oi -O2 -FC
set RDEFS=-D_CRT_SECURE_NO_WARNINGS -DRELEASE
set RLINK=%LINKS%

if exist build rmdir /s /q build
mkdir build
mkdir build\debug
mkdir build\release

pushd build\debug
cl -Fe"Ascencia-d.exe" %DOPTS% %DDEFS% %INPUT% -link -subsystem:windows %DLINK%
popd
if not exist %~dp0build\debug\Ascencia-d.exe exit /b 1

pushd build\release
cl -Fe"Ascencia.exe" %ROPTS% %RDEFS% %INPUT% -link -subsystem:windows %RLINK%
del /q *.obj
popd
if not exist %~dp0build\release\Ascencia.exe exit /b 1

robocopy %~dp0data %~dp0build\debug /S /J /NDL /NJH /NJS /nc /ns /np
robocopy %~dp0data %~dp0build\release /S /J /NDL /NJH /NJS /nc /ns /np
%~dp0rh\rh.exe -open %~dp0build\debug\Ascencia-d.exe -resource %~dp0rh\Ascencia.ico -mask ICONGROUP,MAINICON, -action addskip -save %~dp0build\debug\Ascencia-d.exe
%~dp0rh\rh.exe -open %~dp0build\release\Ascencia.exe -resource %~dp0rh\Ascencia.ico -mask ICONGROUP,MAINICON, -action addskip -save %~dp0build\release\Ascencia.exe

popd
