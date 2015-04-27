@echo off

mkdir "./build"
pushd "./build"
cl -MT -Zi -Od -Oi -fp:fast -nologo /FePathfinder /D DEBUG "../code/win32_main.cpp" User32.lib Gdi32.lib
popd
