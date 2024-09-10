
:: === ASCENCIA WIN32 DEV SCRIPT ===

:: To build release, just run this script with no arguments

::  usage: dev.bat code build debug run
::  code : open code editor (defined in dev.bat)
::  build : build Ascencia using dev.bat build settings
::  debug : open debug build in Visual Studio
::  run : run release build

:: === BUILD REQUIREMENTS: ===

:: Windows 7 x64 or higher
:: Visual Studio Community (https://visualstudio.microsoft.com/) installed on C:\ drive
:: powershell (comes with Windows since Vista)
:: bitsadmin (comes with Windows since XP)

:: === RESOURCEHACKER.EXE LICENCE ===

:: DISCLAIMER: I am NOT distributng ResourceHacker.exe with this repository,
:: I am using this script to acquire ResourceHacker directly from the ResourceHacker website.
:: Please see https://www.angusj.com/resourcehacker/ for more details about this freeware.
:: ---------------------------------------------------------------------------------------------
::This Resource HackerTM software is released as freeware provided that you agree to the following terms and conditions:
::
::    This software is not to be distributed via any website domain or any other media without the prior written approval of the copyright owner.
::    This software is not to be used in any way to illegally modify software. 
::
::DISCLAIMER: A user of this Resource HackerTM software acknowledges that he or she is receiving this software on an "as is" basis and the user is not relying on the accuracy or functionality of the software for any purpose. The user further acknowledges that any use of this software will be at the user's own risk and the copyright owner accepts no responsibility whatsoever arising from the use or application of the software.
::
::The above licence terms constitute "copyright management information" within the meaning of Section 1202 of Title 17 of the United States Code and must not be altered or removed from the licensed works. Their alteration or removal from the licensed works, and the distribution of licensed works without all the above licence terms in an unaltered way, may contravene Section 1202 and give rise civil and/or criminal consequences.
::------------------------------------------------------------------------------------------------

@echo off
pushd %~dp0
setlocal EnableDelayedExpansion
set path=%~dp0assets;%path%

:: === Settings ===

set code_editor=code

set build_debug=1
set build_release=1
set cleanup_after_build=0

set ascencia_exe_input_files=win32_ascencia.c win32\interface.c win32\win_config.c
set ascencia_dll_input_files=ascencia.c
set ascencia_dll_export_funcs=placeholder
set asc_stdlib_dll_input_files=win32\asc_stdlib.c
set asc_stdlib_dll_export_funcs=STD_puts STD_memcpy STD_strlen STD_strcpy STD_strcat

set disable_warnings=4100 4189 4201 4820 4711 4668
set shared_compiler_flags=-TC -MP -Oi -FC -GR- -WX -Wall -DWIN32
set stdlib_compiler_flags=-TC -MP -Oi -FC -GR- -WX -wd4100 -W4 -DWIN32

:: Notable Warnings:
:: 4100 : unreferenced formal parameter (ie not all params from function header are used)
:: 4189 : local variable is initialized but not referenced
:: 4201 : nameless struct or union
:: 4211 : nonstandard extension used : redefined extern to static
:: 4711 : function was selected for inlining, but inline keyword not present
:: 4820 : alignment padding bytes were added to struct
:: 4668 : define is not defined as preprocessor macro, replacing with 0

:: Notable Compiler flags:
:: TC/TP : all source files are C / CPP
:: X : ignore PATH and INCLUDE for include path
:: MP : multiple source files built simultaneously
:: Oi : intrinsics
:: FC : show full path in output display
:: WX : warnings fail the build
:: utf-8
:: Od/O1/O2 : optimizer disable / small code / fast code
:: Z7/Zi/ZI : debug info: in obj / in pdb / pdb with edit & continue support
:: Fm : create symbol map file
:: GR- : disable runtime type info
:: LD/LDd/

:: Notable Linker flags:
:: opt:ref : removes all symbols which are not referenced by runnable code
:: nodefaultlib : exclude all default libraries

:: === VCVARS ===

where /q cl.exe || goto WIN32_SET_VCVARS
goto WIN32_ARGS_START

:WIN32_SET_VCVARS
echo == Loading vcvarsall.bat x64 ==
set vcvarsallfile=
if not exist "C:\Program Files\Microsoft Visual Studio\" goto WIN32_SET_VCVARS_CHECK_C
pushd "C:\Program Files\Microsoft Visual Studio\"
for /f "delims=" %%a in ('dir vcvarsall.bat /s /p /b') do set "vcvarsallfile=%%a"
popd
if exist "%vcvarsallfile%" call "%vcvarsallfile%" x64
where /q cl.exe || goto WIN32_ERROR_VS_INSTALL
goto WIN32_ARGS_START

:WIN32_SET_VCVARS_CHECK_C
echo Didn't find vcvarsall.bat in Program Files, doing deeper search...
set vcvarsallfile=
pushd "C:\"
for /f "delims=" %%a in ('dir vcvarsall.bat /s /p /b') do set "vcvarsallfile=%%a"
popd
if exist "%vcvarsallfile%" call "%vcvarsallfile%" x64
where /q cl.exe || goto WIN32_ERROR_VS_INSTALL
goto WIN32_ARGS_START

:WIN32_ERROR_VS_INSTALL
echo == Error: Couldn't find Visual Studio installation ==
popd
exit /b 1

:: === Args ===

:: "code" : open vscode
:: "build" : build using build settings
:: "debug" : open devenv with debug exe
:: "run" : run release build

:WIN32_ARGS_START
cls
echo.
echo === ASCENCIA DEV SCRIPT ===
echo vcvarsall.bat x64 loaded
echo.
set arg_code=0
set arg_build=0
set arg_debug=0
set arg_run=0

for %%x in (%*) do (
    if "%%~x"=="code" set arg_code=1
    if "%%~x"=="build" set arg_build=1
    if "%%~x"=="debug" set arg_debug=1
    if "%%~x"=="run" set arg_run=1
)

:: default config: build release
if %arg_code%==0 if %arg_build%==0 if %arg_debug%==0 if %arg_run%==0 (
    set build_debug=0
    set build_release=1
    set arg_build=1
)

:WIN32_ARG_SELECT
if %arg_code%==1 goto WIN32_ARG_CODE
if %arg_build%==1 goto WIN32_ARG_BUILD
if %arg_debug%==1 goto WIN32_ARG_DEBUG
if %arg_run%==1 goto WIN32_ARG_RUN
goto WIN32_EXIT

:WIN32_ARG_CODE
set arg_code=0
echo Opening Code Editor...
%code_editor% .
goto WIN32_ARG_SELECT

:WIN32_ARG_DEBUG
set arg_debug=0
devenv build\debug\ascencia-d.exe src\win32_ascencia.c
goto WIN32_ARG_SELECT

:WIN32_ARG_RUN
set arg_run=0
build\release\ascencia.exe
goto WIN32_ARG_SELECT

:: === Build Script ===

:WIN32_ARG_BUILD
set arg_build=0
if not exist build mkdir build
pushd build
if exist int del /q int\*.*
popd
echo.
echo == ASCENCIA BUILD SCRIPT ==
echo.
if %build_debug%==0 if %build_release%==0 (
    echo == dev.bat: No build configuration selected. ==
    popd
    exit /b 1
)
goto WIN32_BUILD_START

:WIN32_BUILD_START
set disable_warnings= %disable_warnings%
set disable_warnings=%disable_warnings: = -wd%
set shared_compiler_flags=%shared_compiler_flags% %disable_warnings%

set ascencia_dll_input_files= %ascencia_dll_input_files%
set ascencia_dll_input_files=%ascencia_dll_input_files: = ..\..\src\%

set asc_stdlib_dll_input_files= %asc_stdlib_dll_input_files%
set asc_stdlib_dll_input_files=%asc_stdlib_dll_input_files: = ..\..\src\%

set ascencia_exe_input_files= %ascencia_exe_input_files%
set ascencia_exe_input_files=%ascencia_exe_input_files: = ..\..\src\%

set ascencia_dll_export_funcs= %ascencia_dll_export_funcs%
set ascencia_dll_export_funcs=%ascencia_dll_export_funcs: = -export:%

set asc_stdlib_dll_export_funcs= %asc_stdlib_dll_export_funcs%
set asc_stdlib_dll_export_funcs=%asc_stdlib_dll_export_funcs: = -export:%

set build_ascencia_dll=1
set build_asc_stdlib_dll=1
set build_ascencia_exe=1
set build_finalize=1
if not exist build\int mkdir build\int
pushd build\int
goto WIN32_DO_BUILD

:WIN32_DO_BUILD
if %build_ascencia_dll%==1 goto WIN32_BUILD_ASCENCIA_DLL
if %build_asc_stdlib_dll%==1 goto WIN32_BUILD_ASC_STDLIB_DLL
if %build_ascencia_exe%==1 goto WIN32_BUILD_ASCENCIA_EXE
popd
if %build_finalize%==1 goto WIN32_BUILD_FINALIZE
echo.
echo == BUILD FINISHED ==
echo.
if %build_debug%==1 (
    if exist build\debug\ascencia-d.exe (
        echo build\debug\ascencia-d.exe    SUCCEEDED
    ) else (
        echo build\debug\ascencia-d.exe    FAILED
    )
    if exist build\debug\ascencia-d.dll (
        echo build\debug\ascencia-d.dll    SUCCEEDED
    ) else (
        echo build\debug\ascencia-d.dll    FAILED
    )
    if exist build\debug\asc_stdlib-d.dll (
        echo build\debug\asc_stdlib-d.dll  SUCCEEDED
    ) else (
        echo build\debug\asc_stdlib-d.dll  FAILED
    )
)
echo.
if %build_release%==1 (
    if exist build\release\ascencia.exe (
        echo build\release\ascencia.exe    SUCCEEDED
    ) else (
        echo build\release\ascencia.exe    FAILED
    )
    if exist build\release\ascencia.dll (
        echo build\release\ascencia.dll    SUCCEEDED
    ) else (
        echo build\release\ascencia.dll    FAILED
    )
    if exist build\release\asc_stdlib.dll (
        echo build\release\asc_stdlib.dll  SUCCEEDED
    ) else (
        echo build\release\asc_stdlib.dll  FAILED
    )
)
goto WIN32_ARG_SELECT

:: === ASCENCIA_DLL ===

:WIN32_BUILD_ASCENCIA_DLL
set build_ascencia_dll_debug=%build_debug%
set build_ascencia_dll_release=%build_release%
goto WIN32_DO_BUILD_ASCENCIA_DLL

:WIN32_DO_BUILD_ASCENCIA_DLL
if %build_ascencia_dll_debug%==1 goto WIN32_BUILD_ASCENCIA_DLL_DEBUG
if %build_ascencia_dll_release%==1 goto WIN32_BUILD_ASCENCIA_DLL_RELEASE
set build_ascencia_dll=0
goto WIN32_DO_BUILD

:WIN32_BUILD_ASCENCIA_DLL_DEBUG
echo.
echo == ASCENCIA.DLL DEBUG BUILD ==
echo building ascencia-d.dll...
cl -nologo -I..\..\src -Fe"ascencia-d.dll" %shared_compiler_flags% -Od -Zi -LDd -DDEBUG=1 %ascencia_dll_input_files% -link -debug -WX -DLL %ascencia_dll_export_funcs%
if exist ascencia-d.dll echo ascencia-d.dll built
if not exist ascencia-d.dll echo ascencia-d.dll build failed
set build_ascencia_dll_debug=0
goto WIN32_DO_BUILD_ASCENCIA_DLL

:WIN32_BUILD_ASCENCIA_DLL_RELEASE
echo.
echo == ASCENCIA.DLL RELEASE BUILD ==
echo building ascencia.dll...
cl -nologo -I..\..\src -Fe"ascencia.dll" %shared_compiler_flags% -O2 -LD -DRELEASE=1 %ascencia_dll_input_files% -link -release -WX -DLL %ascencia_dll_export_funcs%
if exist ascencia.dll echo ascencia.dll built
if not exist ascencia.dll echo ascencia.dll build failed
set build_ascencia_dll_release=0
goto WIN32_DO_BUILD_ASCENCIA_DLL

:: === ASC_STDLIB_DLL ===

:WIN32_BUILD_ASC_STDLIB_DLL
set build_asc_stdlib_dll_debug=%build_debug%
set build_asc_stdlib_dll_release=%build_release%
goto WIN32_DO_BUILD_ASC_STDLIB_DLL

:WIN32_DO_BUILD_ASC_STDLIB_DLL
if %build_asc_stdlib_dll_debug%==1 goto WIN32_BUILD_ASC_STDLIB_DLL_DEBUG
if %build_asc_stdlib_dll_release%==1 goto WIN32_BUILD_ASC_STDLIB_DLL_RELEASE
set build_asc_stdlib_dll=0
goto WIN32_DO_BUILD

:WIN32_BUILD_ASC_STDLIB_DLL_DEBUG
echo.
echo == ASC_STDLIB.DLL DEBUG BUILD ==
echo building asc_stdlib-d.dll...
cl -nologo -I..\..\src -Fe"asc_stdlib-d.dll" %stdlib_compiler_flags% -Od -Zi -LDd -DDEBUG=1 %asc_stdlib_dll_input_files% -link -debug -WX -DLL %asc_stdlib_dll_export_funcs%
if exist asc_stdlib-d.dll echo asc_stdlib-d.dll built
if not exist asc_stdlib-d.dll echo asc_stdlib-d.dll build failed
set build_asc_stdlib_dll_debug=0
goto WIN32_DO_BUILD_ASC_STDLIB_DLL

:WIN32_BUILD_ASC_STDLIB_DLL_RELEASE
echo.
echo == ASC_STDLIB.DLL RELEASE BUILD ==
echo building asc_stdlib.dll...
cl -nologo -I..\..\src -Fe"asc_stdlib.dll" %stdlib_compiler_flags% -O2 -LD -DRELEASE=1 %asc_stdlib_dll_input_files% -link -release -WX -DLL %asc_stdlib_dll_export_funcs%
if exist asc_stdlib.dll echo asc_stdlib.dll built
if not exist asc_stdlib.dll echo asc_stdlib.dll build failed
set build_asc_stdlib_dll_release=0
goto WIN32_DO_BUILD_ASC_STDLIB_DLL

:: === ASCENCIA_EXE ===

:WIN32_BUILD_ASCENCIA_EXE
set build_ascencia_exe_debug=%build_debug%
set build_ascencia_exe_release=%build_release%
goto WIN32_DO_BUILD_ASCENCIA_EXE

:WIN32_DO_BUILD_ASCENCIA_EXE
if %build_ascencia_exe_debug%==1 goto WIN32_BUILD_ASCENCIA_EXE_DEBUG
if %build_ascencia_exe_release%==1 goto WIN32_BUILD_ASCENCIA_EXE_RELEASE
set build_ascencia_exe=0
goto WIN32_DO_BUILD

:WIN32_BUILD_ASCENCIA_EXE_DEBUG
echo.
echo == ASCENCIA.EXE DEBUG BUILD ==
echo building ascencia-d.exe...
cl -nologo -I..\..\src -Fe"ascencia-d.exe" %shared_compiler_flags% -Od -Zi -DDEBUG=1 %ascencia_exe_input_files% -link -debug -WX -subsystem:console kernel32.lib
if exist ascencia-d.exe (
    echo ascencia-d.exe built
) else (
    echo ascencia-d.exe build failed
)
set build_ascencia_exe_debug=0
goto WIN32_DO_BUILD_ASCENCIA_EXE

:WIN32_BUILD_ASCENCIA_EXE_RELEASE
echo.
echo == ASCENCIA.EXE RELEASE BUILD ==
echo building ascencia.exe...
cl -nologo -I..\..\src -Fe"ascencia.exe" %shared_compiler_flags% -O2 -DRELEASE=1 %ascencia_exe_input_files% -link -release -WX -subsystem:windows kernel32.lib
if exist ascencia.exe (
    echo ascencia.exe built
) else (
    echo ascencia.exe build failed
)
set build_ascencia_exe_release=0
goto WIN32_DO_BUILD_ASCENCIA_EXE

:: === FINALIZE ===

:WIN32_BUILD_FINALIZE
echo.
echo == FINALIZE ==

if not exist assets\rh.exe (
	if not exist assets\tmp mkdir assets\tmp
	bitsadmin.exe /transfer "ResourceHacker" https://www.angusj.com/resourcehacker/resource_hacker.zip %~dp0assets\tmp\rh.zip
	powershell "Expand-Archive -Path """%~dp0assets\tmp\rh.zip""" -DestinationPath """%~dp0assets\tmp\rh""""
	copy assets\tmp\rh\ResourceHacker.exe assets\rh.exe
	del /q assets\tmp\rh\*.*
	del /q assets\tmp\rh\help\*.*
	rmdir assets\tmp\rh\help
	del /q assets\tmp\rh\samples\*.*
	rmdir assets\tmp\rh\samples
	rmdir assets\tmp\rh\
	del /q assets\tmp\*.*
	rmdir assets\tmp
)

if %build_debug%==1 (
    if not exist %~dp0build\debug mkdir %~dp0build\debug
    del /q %~dp0build\debug\*.*
    if exist %~dp0build\int\ascencia-d.dll copy %~dp0build\int\ascencia-d.dll %~dp0build\debug\ascencia-d.dll
    if exist %~dp0build\int\ascencia-d.exe copy %~dp0build\int\ascencia-d.exe %~dp0build\debug\ascencia-d.exe
    if exist %~dp0build\int\asc_stdlib-d.dll copy %~dp0build\int\asc_stdlib-d.dll %~dp0build\debug\asc_stdlib-d.dll
    rh -open %~dp0build\debug\ascencia-d.exe -resource %~dp0assets\ascencia.ico -mask ICONGROUP,MAINICON, -action addskip -save %~dp0build\debug\ascencia-d.exe
    robocopy %~dp0data %~dp0build\debug /S /J /NDL /NJH /NJS /nc /ns /np
)

if %build_release%==1 (
    if not exist %~dp0build\release mkdir %~dp0build\release
    del /q %~dp0build\release\*.*
    if exist %~dp0build\int\ascencia.exe copy %~dp0build\int\ascencia.exe %~dp0build\release\ascencia.exe
    if exist %~dp0build\int\ascencia.dll copy %~dp0build\int\ascencia.dll %~dp0build\release\ascencia.dll
    if exist %~dp0build\int\asc_stdlib.dll copy %~dp0build\int\asc_stdlib.dll %~dp0build\release\asc_stdlib.dll
    rh -open %~dp0build\release\ascencia.exe -resource %~dp0assets\ascencia.ico -mask ICONGROUP,MAINICON, -action addskip -save %~dp0build\release\ascencia.exe
    robocopy %~dp0data %~dp0build\release /S /J /NDL /NJH /NJS /nc /ns /np
)

if %cleanup_after_build%==1 goto WIN32_DO_CLEANUP
set build_finalize=0
goto WIN32_DO_BUILD

:WIN32_DO_CLEANUP
echo cleanup intermediate files...
if exist build\int del /q build\int\*.*
if exist build\int rmdir build\int
echo cleanup done
set build_finalize=0
goto WIN32_DO_BUILD

:: === EXIT ===

:WIN32_Exit
popd