@echo off
pushd %~dp0

:DoStart
where /q cl.exe || goto DoSetVCVars
if not exist build mkdir build
pushd build

set INPUT=..\src\ascencia.cpp
set OUTNAME=ascencia
set INCLUDES=-I..\src -I..\include
set LIBS=%~dp0lib
set DEBUG_BUILD=1

set COPTS=-nologo -MT -Gm- -GR- -EHa -Oi -FC -W4 -wd4201 -wd4100 -wd4211 -D_CRT_SECURE_NO_WARNINGS
set LINK=-LIBPATH:%LIBS% -opt:ref SDL3.lib ws2_32.lib OpenAL32.lib sndfile.lib ogg.lib opus.lib vorbis.lib vorbisenc.lib vorbisfile.lib FLAC.lib libmp3lame-static.lib libmpghip-static.lib mpg123.lib shlwapi.lib libcrypto.lib libssl.lib advapi32.lib user32.lib crypt32.lib winmm.lib archive.lib bz2.lib zlib.lib lz4.lib lzma.lib zstd.lib

:DoBuild
if %DEBUG_BUILD%==1 goto DoDebug
if %DEBUG_BUILD%==0 goto DoRelease

:DoDebug
echo.
echo ===== DEBUG =====
echo.
set DBG_COPTS=-Z7 -Fm %COPTS%
set DBG_DEF=-DBUILD_DEBUG=1 -DWIN32
del /q *.*
cl %INCLUDES% -Fe"%OUTNAME%.exe" %DBG_COPTS% %DBG_DEF% %INPUT% -link -subsystem:console %LINK%
del /q *.obj
goto DoEnd

:DoRelease
echo.
echo ===== RELEASE =====
echo.
set RLS_COPTS=-O2 %COPTS%
set RLS_DEF=-DWIN32
del /q *.*
cl %INCLUDES% -Fe"%OUTNAME%.exe" %RLS_COPTS% %RLS_DEF% %INPUT% -link -subsystem:windows %LINK%
del /q *.obj
goto DoEnd

:DoSetVCVars
echo == Searching for vcvarsall.bat ==
set vcvarsallfile=
pushd "c:\Program Files"
for /f "delims=" %%a in ('dir vcvarsall* /s /p /b') do set "vcvarsallfile=%%a"
popd
call "%vcvarsallfile%" x64
where /q cl.exe || goto DoErrorNoVCVars
goto DoStart

:DoErrorNoVCVars
goto DoEnd

:DoEnd
popd
popd
