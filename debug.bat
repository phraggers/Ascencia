@echo off
pushd %~dp0
set path=%path%;"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE"
devenv %~dp0build\Ascencia.exe %~dp0code\win32_ascencia.c
popd
