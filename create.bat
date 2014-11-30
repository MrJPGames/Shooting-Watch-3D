@echo off
make
set push=n
set /p push=Push to 3ds? (Y/N):
if "%push%" == "y" goto cat
goto nocat
:cat
cat tpsgui.3dsx | nc 192.168.137.137 9000
:nocat
@echo on