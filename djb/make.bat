
del djb.obj djb_asm.obj djb.exe djb_bench.exe

ml64 /c /Fodjb_asm.obj djb.asm
cl /O2 /GL djb.c /link djb_asm.obj
cl /O2 /GL djb_bench.c /link djb_asm.obj

