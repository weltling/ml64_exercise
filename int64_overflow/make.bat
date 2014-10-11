
del mul.obj mul_asm.obj  mul_bench.exe

rem ml64 /c /Fomul_asm.obj mul.asm
rem cl /O2 /GL mul_bench.c /link mul_asm.obj

ml64 /c /Zi /Fomul_asm.obj mul.asm
cl /Zi /GL mul_bench.c /link mul_asm.obj
