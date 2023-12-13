@echo off 
pushd build

cl -Zi -FC ..\source\win32_platform.c user32.lib gdi32.lib

popd

