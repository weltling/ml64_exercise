

del djb*.obj djb*.exe djb*.pgd djb.pgd djb*.pgc

ml64 /c /Fodjb_asm.obj djb.asm

cl /c /GL /O2 djb.c
link /ltcg:pgi djb_asm.obj djb.obj /out:djb.exe
cl /c /GL /O2 djb_bench.c
link /ltcg:pgi djb_asm.obj djb_bench.obj /out:djb_bench.exe

rem djb.exe > nul
djb_bench.exe > nul

rem del djb.obj djb_bench.obj
rem cl /c /GL /O2 djb.c
link /ltcg:pgo djb_asm.obj djb.obj /out:djb.exe
rem cl /c /GL /O2 djb_bench.c
link /ltcg:pgo djb_asm.obj djb_bench.obj /out:djb_bench.exe

