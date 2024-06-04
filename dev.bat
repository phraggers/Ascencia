@echo off
for %%* in (.) do set ProjectName=%%~nx*
echo Project: %ProjectName%
pushd %~dp0
set path=%path%;%~dp0;%~dp0misc
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmd /k
set /P CommitMessage="git commit message: "
if ["%CommitMessage%"]==[""] goto _end
git init
git add src readme.md build.bat dev.bat
git commit -m "%CommitMessage%"
git push origin master
:_end
popd
pause