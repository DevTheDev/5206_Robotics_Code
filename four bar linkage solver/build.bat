@echo off

cl -MT -Zi -O2 -Oi -fp:precise -nologo /Fefour_bar_linkage_solver "./four_bar_linkage_solver - gear ratio.cpp"

call "./four_bar_linkage_solver.exe"