

del poly*.obj poly*.exe poly*.pgd poly.pgd poly*.pgc

ml64 /c /Fopoly_asm.obj poly.asm

cl /c /GL /O2 poly_bench.c
link /ltcg:pgi poly_asm.obj poly_bench.obj /out:poly_bench.exe

rem poly.exe > nul
poly_bench.exe %random% > nul

rem cl /c /GL /O2 poly_bench.c
link /ltcg:pgo poly_asm.obj poly_bench.obj /out:poly_bench.exe

