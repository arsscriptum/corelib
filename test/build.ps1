

function ConfigureEnv{
    pushd "$ENV:VS140COMNTOOLS"
    $IsConfigured = $ENV:VsMSBuildCmdConfigured
    Write-Host "[config] " -f Blue -n
    if($IsConfigured -eq 'COMPLETE'){
        Write-Host "VsMSBuildCmdConfigured => $ENV:VsMSBuildCmdConfigured" -f Magenta
    }else{
        & ".\VsMSBuildCmd.bat"
        Write-Host " VsMSBuildCmd => Ok" -f Cyan
    }
    Write-Host "[config] " -f Blue -n
    $IsConfigured = $ENV:VsDevCmdConfigured
    if($IsConfigured -eq 'COMPLETE'){
        Write-Host "VsDevCmdConfigured => $ENV:VsDevCmdConfigured" -f Magenta
    }else{
        & ".\VsDevCmd.bat"
        Write-Host " VsDevCmd => Ok" -f Cyan
    }

    $IsConfigured = $ENV:vsvars32Configured
    Write-Host "[config] " -f Blue -n
    if($IsConfigured -eq 'COMPLETE'){
        Write-Host "vsvars32Configured => $ENV:vsvars32Configured" -f Magenta
    }else{
        & ".\vsvars32.bat"
        Write-Host " vsvars32 => Ok" -f Cyan
    }

    popd
}

$SourceFile='P:\Development\corelib\test\test.cpp'
$OutFile = 'P:\Development\corelib\test\test.exe'

$defines='-D_USING_V110_SDK71_ -DSUBSYSTEM_CONSOLE -DDEBUG_OUTPUT'
$link_options='/link /FILEALIGN:512 /OPT:REF /OPT:ICF /INCREMENTAL:NO /subsystem:console,5.01'
$libs='gdiplus.lib user32.lib Gdi32.lib ws2_32.lib Wininet.lib'

$CLEXE="C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\cl.exe"

&"$CLEXE" /Ox $SourceFile $defines $link_options $libs /Fe:$OutFile