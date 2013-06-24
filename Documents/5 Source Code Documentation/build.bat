@echo off

if not exist out mkdir out
doxygen Doxyfile > info.txt 2> warning.txt

exit 0
