@echo off

set ProjectName=Ascencia
set BuildType=1
:: 1=BUILD_SLOWDEBUG, 2=BUILD_FASTDEBUG, 3=BUILD_RELEASE

if not exist w:\build mkdir w:\build
del w:\build\%ProjectName%.exe

set CompilerWarnings=-W4 -wd4201 -wd4100 -wd4189

if '%BuildType%'=='1' (
   set InternalDefines=-DBUILD_DEBUG=1 -DBUILD_SLOW=1
   set CompilerSwitches=-nologo %CompilerWarnings% -FC -Fmwin32_%ProjectName%.map -Z7 -Od -Oi -GR- -Gm- -EHa- -MT
)

if '%BuildType%'=='2' (
   set InternalDefines=-DBUILD_DEBUG=1
   set CompilerSwitches=-nologo %CompilerWarnings% -FC -Fmwin32_%ProjectName%.map -Z7 -Oi -GR- -Gm- -EHa- -MT
)

if '%BuildType%'=='3' (
   set InternalDefines=-DBUILD_RELEASE=1
   set CompilerSwitches=-nologo %CompilerWarnings% -FC -Oi -GR- -Gm- -EHa- -MT -O2
)

set CompilerInput=w:\src\%ProjectName%.cpp
set LinkerOptions=-subsystem:windows -opt:ref
set LinkerLibs=user32.lib gdi32.lib shell32.lib opengl32.lib SDL2main.lib SDL2.lib SDL2_image.lib SDL2_mixer.lib SDL2_net.lib SDL2_net.lib libcurl.lib zlib.lib
set AdditionalIncludePaths=/I..\deps\include
set AdditionalLibPaths=/LIBPATH:..\deps\lib
echo.

set COMPILE_COMMAND=cl %InternalDefines% %CompilerSwitches% %AdditionalIncludePaths% %CompilerInput% /link %LinkerOptions% %AdditionalLibPaths% %LinkerLibs%
echo %COMPILE_COMMAND%
echo.

pushd w:\build
%COMPILE_COMMAND%
popd

if not exist w:\build\%ProjectName%.exe exit /b 1

if '%BuildType%'=='3' (
   w:\res\ResourceHacker.exe -open w:\build\%ProjectName%.exe -resource w:\res\%ProjectName%.ico -mask ICONGROUP,MAINICON, -action addskip -save w:\build\%ProjectName%.exe
)

exit /b 0

:: W4 = warning level 4
:: wd(num) = ignore specific warning
:: 4201 anonymous struct/union used
:: 4100 unreferenced formal parameter
:: 4189 unused local variable
:: FC = full path of files in output
:: Fm = create symbol map
:: //Zi = output debug data pdb
:: Z7 = inject debug data in obj
:: Od = disable ALL compiler optimizations
:: Oi = use compiler intrinsics (direct asm func calls) instead of CRT versions
:: GR- = turn off C++ runtime type info
:: -Gm- = disable incremental rebuild
:: EHa = disable C++ exception handling (try catch)
:: MT = static link CRT
:: opt:ref = only compile external functions which are referenced
