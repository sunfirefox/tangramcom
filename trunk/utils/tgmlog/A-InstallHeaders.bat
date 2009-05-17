echo off

xcopy /D /Y /R tgmlog*.h ..\..\include\


if not %1S EQU S goto end
pause
:end


 