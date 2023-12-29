@echo off
cls
title Compile BuildFolia-1.0

set "distFolder=dist"
set "tmpFolder=tmp"
set "libFolder=lib"
goto createFolders

:createFolders
if not exist "%distFolder%" (
    mkdir "%distFolder%"
    echo Added "dist/"
)

cd "%distFolder%"

if not exist "%tmpFolder%" (
    mkdir "%tmpFolder%"
    echo Added "dist/tmp/"
)

if not exist "%libFolder%" (
    mkdir "%libFolder%"
    echo Added "dist/lib/"
)

cd ..
goto build

:build
windres app_resources.rc dist/tmp/app_resources.o
echo Compiled resources file
g++ -c src/build-folia.cpp -o dist/tmp/main.o
echo Compiled main
cd "%distFolder%\%tmpFolder%"
g++ main.o app_resources.o -o "..\lib\Build Folia - 1.0.exe" -mwindows
echo Linked
echo Finished!
pause
exit