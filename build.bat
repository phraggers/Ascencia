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

set ACOPTS=-nologo -MT -Gm- -GR- -EHa -O2 -FC
set ACINPUT=%~dp0code\util\asset_compiler.c
set ACLINK=shell32.lib user32.lib

if exist build rmdir /s /q build
mkdir build
mkdir build\debug
mkdir build\release
mkdir build\asset_compiler

pushd build\debug
cl -Fe"Ascencia-d.exe" -I"%~dp0code" %DOPTS% %DDEFS% %INPUT% -link -subsystem:windows %DLINK%
popd
if not exist %~dp0build\debug\Ascencia-d.exe exit /b 1

pushd build\release
cl -Fe"Ascencia.exe" -I"%~dp0code" %ROPTS% %RDEFS% %INPUT% -link -subsystem:windows %RLINK%
del /q *.obj
popd
if not exist %~dp0build\release\Ascencia.exe exit /b 1

pushd build\asset_compiler
cl -Fe"AssetCompiler.exe" -I"%~dp0code" %ACOPTS% %ACINPUT% -link -subsystem:console %ACLINK%
popd
if not exist %~dp0build\asset_compiler\AssetCompiler.exe exit /b 1

%~dp0build\asset_compiler\AssetCompiler.exe %~dp0assets %~dp0data

robocopy %~dp0data %~dp0build\debug /S /J /NDL /NJH /NJS /nc /ns /np
robocopy %~dp0data %~dp0build\release /S /J /NDL /NJH /NJS /nc /ns /np
if exist %~dp0rh\rh.exe %~dp0rh\rh.exe -open %~dp0build\debug\Ascencia-d.exe -resource %~dp0rh\Ascencia.ico -mask ICONGROUP,MAINICON, -action addskip -save %~dp0build\debug\Ascencia-d.exe
if exist %~dp0rh\rh.exe %~dp0rh\rh.exe -open %~dp0build\release\Ascencia.exe -resource %~dp0rh\Ascencia.ico -mask ICONGROUP,MAINICON, -action addskip -save %~dp0build\release\Ascencia.exe

popd
