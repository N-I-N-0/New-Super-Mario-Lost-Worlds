@echo off

for %%f in (*.S) do (
    powerpc-eabi-as -mregnames %%f -o %%~nf.o
    powerpc-eabi-ld -Ttext 0x800046F0 --oformat binary %%~nf.o -o %%~nf.bin
    del /f %%~nf.o
)
