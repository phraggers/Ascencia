@echo off
setlocal enabledelayedexpansion
echo Writing icon...
%~dp0rh\rh.exe -open %1 -resource %~dp0rh\Ascencia.ico -mask ICONGROUP,MAINICON, -action addskip -save %1