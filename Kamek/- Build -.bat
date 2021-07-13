@echo OFF
set mypath=%~dp0
set PATH=%PATH%;%mypath:~0,-1%\tools\devkitPPC\bin
rmdir /s /q Build
rmdir /s /q NewerASM
mkdir Build
mkdir NewerASM
if %errorlevel%==0 goto mapfile
pause
exit

:mapfile
py -3 tools/mapfile_tool.py
if %errorlevel%==0 goto kamek
pause
exit

:kamek
py -3 tools/kamek.py NewerProjectKP.yaml --no-rels --use-mw --gcc-type=powerpc-eabi --gcc-path=tools\devkitPPC\bin --gcc-append-exe --mw-path=tools --fast-hack
if %errorlevel%==0 goto rename
pause
exit

:rename
py -3 tools/renameNewer.py
if %errorlevel%==0 goto move
pause
exit

:move
move "%~dp0\Build\*" "%NSMBWer%\DATA\files\NewerRes" >nul
if %errorlevel%==0 goto end
pause
exit

:end
echo Built all! %date% %time%
pause
