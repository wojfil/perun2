@echo off
cd ..\build

set FOLDER_PATH=%~dp0..\build\src\Release
explorer "%FOLDER_PATH%"

cd ..\scripts
