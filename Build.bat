@echo off
setlocal EnableDelayedExpansion

:: ==============================================================================
:: 
::      Build.bat
::
::      Build different configuration of the app
::
:: ==============================================================================
::   arsccriptum - made in quebec 2020 <guillaumeplante.qc@gmail.com>
:: ==============================================================================

goto :init

:init
    set "__scripts_root=%AutomationScriptsRoot%"
    call :read_script_root development\build-automation  BuildAutomation
    set "__script_file=%~0"
    set "__target=%~1"
    set "__script_path=%~dp0"
    set "__makefile=%__scripts_root%\make\make.bat"
    set "__lib_date=%__scripts_root%\batlibs\date.bat"
    set "__lib_out=%__scripts_root%\batlibs\out.bat"
    ::*** This is the important line ***
    set "__build_cfg=%__script_path%buildcfg.ini"
    set "__build_cancelled=0"
    goto :validate


:header
    echo. %__script_name% v%__script_version%
    echo.    This script is part of codecastor build wrappers.
    echo.
    goto :eof

:header_err
    echo.**************************************************
    echo.This script is part of codecastor build wrappers.
    echo.**************************************************
    echo.
    echo. YOU NEED TO HAVE THE BuildAutomation Scripts setup on you system...
    echo. https://github.com/codecastor/BuildAutomation
    goto :eof


:read_script_root
    set regpath=%OrganizationHKCU::=%
    for /f "tokens=2,*" %%A in ('REG.exe query %regpath%\%1 /v %2') do (
            set "__scripts_root=%%B"
        )
    goto :eof

:validate
    if not defined __scripts_root          call :header_err && call :error_missing_path __scripts_root & goto :eof
    if not exist %__makefile%  call :error_missing_script "%__makefile%" & goto :eof
    if not exist %__lib_date%  call :error_missing_script "%__lib_date%" & goto :eof
    if not exist %__lib_out%  call :error_missing_script "%__lib_out%" & goto :eof
    if not exist %__build_cfg%  call :error_missing_script "%__build_cfg%" & goto :eof

    goto :prebuild_header


:prebuild_header
    call %__lib_date% :getbuilddate
    call %__lib_out% :__out_underline_red " Compilation started for %cd%  %__target%"  
    call :build
    goto :eof


:: ==============================================================================
::   call make
:: ==============================================================================
:call_make_build
    set config=%1
    set platform=%2
    call %__makefile% /v /i %__build_cfg% /t Build /c %config% /p %platform%
    goto :finished

:: ==============================================================================
::   Build static
:: ==============================================================================
:build_static
    call :call_make_build Debug x86
    call :call_make_build Release x86
    call :call_make_build Debug x64
    call :call_make_build Release x64
    goto :eof

:: ==============================================================================
::   Build dll
:: ==============================================================================
:build_dll
    call :call_make_build DebugDll x86
    call :call_make_build ReleaseDll x86
    call :call_make_build DebugDll x64
    call :call_make_build ReleaseDll x64
    goto :eof

:: ==============================================================================
::   clean all
:: ==============================================================================
:clean
    call %__makefile% /v /i %__build_cfg% /t Clean /c Debug /p x86
    call %__makefile% /v /i %__build_cfg% /t Clean /c Release /p x86
    call %__makefile% /v /i %__build_cfg% /t Clean /c Debug /p x64
    call %__makefile% /v /i %__build_cfg% /t Clean /c Release /p x64
    goto :eof


:protek
    set APP_PATH=%cd%\bin\x64\Release
    call %__lib_out% :__out_underline_cya "STEP 2) ENCRYPTION"
    call %__lib_out% :__out_n_l_gry "    lecachotier.exe => lecachotier_p.exe"
    "%AXPROTECTOR_SDK%\bin\AxProtector.exe" -x -kcm -f6000010 -p101001 -cf0 -d:6.20 -fw:3.00 -slw -nn -cav -cas100 -wu1000 -we100 -eac -eec -eusc1 -emc -v -cag23 -caa7 -o:"%APP_PATH%\lecachotier_p.exe" "%APP_PATH%\lecachotier.exe" > Nul
    call %__lib_out% :__out_d_grn " SUCCESS"
    dir .\bin\x64\Release\lecachotier_p.exe | findstr lecacho
    goto :eof


:: ==============================================================================
::   Build
:: ==============================================================================
:build
    if "%__target%" == "clean" (
        call :clean
        goto :finished
        )
    if "%__target%" == "static" (
        call :build_static
        goto :finished
        )
    if "%__target%" == "dll" (
        call :build_dll
        goto :finished
        )    
    if "%__target%" == "" (
        call %__lib_out% :__out_d_yel "Please specify target (static, dll, clean)"
        goto :done
        ) 
    ::call :protek
    goto :finished


:error_missing_path
    echo.
    echo   Error
    echo    Missing path: %~1
    echo.
    goto :eof



:error_missing_script
    echo.
    echo    Error
    echo    Missing bat script: %~1
    echo.
    goto :eof


:done
    call %__lib_out% :__out_d_yel "Exit"
    goto :eof

:finished
    call %__lib_out% :__out_d_grn "Build complete"
