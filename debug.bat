@echo off
pushd %~dp0
set path=%path%;"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE"
devenv %~dp0build\debug\Ascencia-d.exe %~dp0code\ascencia.c
popd
