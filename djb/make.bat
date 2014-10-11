
del djb.obj djb_asm.obj djb.exe djb_bench.exe

rem ml64 /c /Fodjb_asm.obj djb.asm
rem cl /O2 /GL djb.c /link djb_asm.obj
rem cl /O2 /GL djb_bench.c /link djb_asm.obj

ml64 /c /Zi /Fodjb_asm.obj djb.asm
cl /Zi djb.c /link djb_asm.obj
cl /Zi /GL djb_bench.c /link djb_asm.obj
