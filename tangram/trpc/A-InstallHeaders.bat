echo off

xcopy /D /Y /R tgmrpc*.h ..\..\include\


if not %1S EQU S goto end
pause
:end


 