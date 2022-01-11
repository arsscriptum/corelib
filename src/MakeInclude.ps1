

$Script:Error = ''
 $ErrorActionPreference = "Ignore"
$Lines = [System.Collections.ArrayList]::new()
$HFiles = [System.Collections.ArrayList]::new()


$AllHeaders = (gci -Path . -Filter '*.h' -Recurse).Fullname

ForEach($h in $AllHeaders){
    $path = Resolve-Path $h -Relative
    $path = $path.Substring(2)
   

    $null = $HFiles.Add($path)
}

$HFiles = $HFiles | Sort
try{
$CurrentCat = ''
ForEach($h in $HFiles){
    $i = $h.IndexOf('\')
    $Script:Error = $h
    $name = $h.SubString(0, $i)
    if($CurrentCat -ne $name){
        $CurrentCat = $name
        $Null = $Lines.Add("`n//================================================")
        $Null = $Lines.Add("// Includes for $name")
        $Null = $Lines.Add("//================================================`n")
        #Write-Host "//================================================"
        #Write-Host "// Includes for $name"
        #Write-Host "//================================================"
    }

    $Null = $Lines.Add($h)
}
}catch{
    write-host "$Script:Error  $_"
}
Set-Content -Path "core.h" -Value $Lines