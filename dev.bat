@echo off
setlocal enabledelayedexpansion
pushd %~dp0
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64
set Project=Ascencia
cmd /k
popd