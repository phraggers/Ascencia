@echo off

set ProjectName=Ascencia

if not exist w:\build mkdir w:\build
if exist w:\build\%ProjectName%.exe del w:\build\%ProjectName%.exe

set InternalDefines=-DWIN32
set CompilerSwitches=-nologo
set CompilerInput=w:\src\%ProjectName%.cpp
set LinkerOptions=
set LinkerLibs=
echo.
echo CPPCHECK:
cppcheck --template='{file}: {line}: {severity}: {message}' --quiet %CompilerInput%

echo.
echo MSVC:
pushd w:\build
cl %InternalDefines% %CompilerSwitches% %CompilerInput% /link %LinkerOptions% %LinkerLibs%
popd

if not exist w:\build\%ProjectName%.exe exit /b 1
exit /b 0
