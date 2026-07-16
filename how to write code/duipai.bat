@echo off
set i=0
:loop
set /a i+=1
echo case %i%
gen.exe > in.txt
brute.exe < in.txt > ans.txt
std.exe < in.txt > out.txt
fc ans.txt out.txt || goto wa
goto loop
:wa
pause