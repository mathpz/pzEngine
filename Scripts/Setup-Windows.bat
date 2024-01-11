@echo off

pushd ..
Vendor\Binaries\Premake\Windows\premake5.exe --file=Build.lua gmake2
popd
pause