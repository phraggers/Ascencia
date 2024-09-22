
@echo off
pushd %~dp0
setlocal EnableDelayedExpansion

where /q cl.exe || goto WIN32_SET_VCVARS
goto WIN32_RUN_DEBUG

:WIN32_SET_VCVARS
echo == Loading vcvarsall.bat x64 ==
set vcvarsallfile=
if not exist "C:\Program Files\Microsoft Visual Studio\" goto WIN32_SET_VCVARS_CHECK_C
pushd "C:\Program Files\Microsoft Visual Studio\"
for /f "delims=" %%a in ('dir vcvarsall.bat /s /p /b') do set "vcvarsallfile=%%a"
popd
if exist "%vcvarsallfile%" call "%vcvarsallfile%" x64
where /q cl.exe || goto WIN32_ERROR_VS_INSTALL
goto WIN32_RUN_DEBUG

:WIN32_SET_VCVARS_CHECK_C
echo Didn't find vcvarsall.bat in Program Files, doing deeper search...
set vcvarsallfile=
pushd "C:\"
for /f "delims=" %%a in ('dir vcvarsall.bat /s /p /b') do set "vcvarsallfile=%%a"
popd
if exist "%vcvarsallfile%" call "%vcvarsallfile%" x64
where /q cl.exe || goto WIN32_ERROR_VS_INSTALL
goto WIN32_RUN_DEBUG

:WIN32_ERROR_VS_INSTALL
echo == Error: Couldn't find Visual Studio installation ==
popd
exit /b 1

:WIN32_RUN_DEBUG
devenv build\debug\ascencia-d.exe src\ascencia_win32.c

popd