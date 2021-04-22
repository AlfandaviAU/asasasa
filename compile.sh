echo Disk, Map, Sector, and Files Image
dd if=/dev/zero of=system.img bs=512 count=2880
dd if=/dev/zero of=map.img bs=512 count=1
dd if=/dev/zero of=files.img bs=512 count=2
dd if=/dev/zero of=sectors.img bs=512 count=1

echo Bootloader
nasm bootloader.asm -o bootloader

echo Bootloader to Disk Image
dd if=bootloader of=system.img bs=512 count=1 conv=notrunc

echo Compile loadFile
gcc -o loadFile loadFile.c

echo Compile Library to Object Code with BCC
bcc -ansi -c -o ./out/fileIO.o ./library/fileIO.c
bcc -ansi -c -o ./out/folderIO.o ./library/folderIO.c
bcc -ansi -c -o ./out/matematika.o ./library/matematika.c
bcc -ansi -c -o ./out/teks.o ./library/teks.c
bcc -ansi -c -o ./out/utility.o ./library/utility.c
bcc -ansi -c -o ./out/render.o ./library/render.c

echo Compile and Link shell
bcc -ansi -c -o ./out/shell_std.o ./shell_std.c
bcc -ansi -c -o ./out/shell.o ./shell.c
nasm -f as86 ./lib.asm -o out/lib.o
ld86 -o out/shell -d out/shell.o out/lib.o out/shell_std.o
./loadFile system.img shell 0

echo Compile Kernel to Object Code with BCC
bcc -ansi -c -o ./out/kernel.o kernel.c

echo Compile ASM Kernel to Object Code with NASM
nasm -f as86 kernel.asm -o ./out/kernel_asm.o

echo Link Object Code with ld86
ld86 -o out/kernel -d out/kernel.o out/kernel_asm.o out/fileIO.o out/folderIO.o out/matematika.o out/teks.o out/utility.o out/render.o


echo Kernel to Disk Image in Sector 1
dd if=out/kernel of=system.img bs=512 conv=notrunc seek=1
