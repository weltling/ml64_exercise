

del mul*.obj mul*.exe mul*.pgd mul.pgd mul*.pgc

ml64 /c /Fomul_asm.obj mul.asm

cl /c /GL /O2 mul_bench.c
link /ltcg:pgi mul_asm.obj mul_bench.obj /out:mul_bench.exe

rem mul.exe > nul
mul_bench.exe %random% > nul

rem cl /c /GL /O2 mul_bench.c
link /ltcg:pgo mul_asm.obj mul_bench.obj /out:mul_bench.exe

