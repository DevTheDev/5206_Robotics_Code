@echo off

cl -MT -Zi -Ot -Oi -nologo /Fefour_bar_linkage_solver "./four_bar_linkage_solver - brute force.cpp"

call "./four_bar_linkage_solver.exe"