@echo off

mkdir "../build"
pushd "../build"

del Pathfinder.exe

cl -MT -Zi -Od -Oi -fp:fast -nologo /FePathfinder /D DEBUG "../code/win32_main.cpp" User32.lib Gdi32.lib

Pathfinder.exe

popd

