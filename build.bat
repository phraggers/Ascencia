@echo off

set ProjectName=Ascencia

if not exist w:\build mkdir w:\build
del w:\build\%ProjectName%.exe

set InternalDefines=-DDEBUG=1
set CompilerSwitches=-nologo -W4 -wd4201 -wd4100 -wd4189 -FC -Fmwin32_%ProjectName%.map -Z7 -Od -Oi -GR- -Gm- -EHa- -MT
set CompilerInput=w:\src\%ProjectName%.cpp
set LinkerOptions=-subsystem:windows -opt:ref
set LinkerLibs=user32.lib gdi32.lib shell32.lib SDL2main.lib SDL2.lib SDL2_image.lib SDL2_mixer.lib SDL2_net.lib SDL2_net.lib
set AdditionalIncludePaths=/I..\deps\include
set AdditionalLibPaths=/LIBPATH:..\deps\lib
echo.
echo CPPCHECK:
cppcheck --template='{file}: {line}: {severity}: {message}' --quiet %CompilerInput%
echo.
echo MSVC:
pushd w:\build
cl %InternalDefines% %CompilerSwitches% %AdditionalIncludePaths% %CompilerInput% /link %LinkerOptions% %AdditionalLibPaths% %LinkerLibs%
popd

if not exist w:\build\%ProjectName%.exe exit /b 1
exit /b 0
