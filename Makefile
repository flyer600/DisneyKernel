system: ckernel.asm boot.bat kernel.bat system.img 

ckernel.asm: ckernel_u.asm
	grep -E -v 'global|extern|SECTION' ckernel_u.asm > ckernel.asm
ckernel_u.asm : ckernel.o #最终生成的反汇编文件
	objconv -fnasm ckernel.o ckernel_u.asm
ckernel.o : write_vga_desktop.o mem_util.o win_sheet.o timer.o global_define.o multi_task.o
	ld -m elf_i386 -r write_vga_desktop.o mem_util.o -o ckernel.o win_sheet.o timer.o global_define.o multi_task.o
write_vga_desktop.o : write_vga_desktop.c mem_util.h
	gcc -m32 -fno-asynchronous-unwind-tables -s -c -o write_vga_desktop.o write_vga_desktop.c
mem_util.o : mem_util.h mem_util.c
	gcc -m32 -fno-asynchronous-unwind-tables -s -c -o mem_util.o mem_util.c
win_sheet.o : win_sheet.h win_sheet.c
	gcc -m32 -fno-asynchronous-unwind-tables -s -c -o win_sheet.o win_sheet.c
timer.o : timer.h timer.c
	gcc -m32 -fno-asynchronous-unwind-tables -s -c -o timer.o timer.c
global_define.o : global_define.h global_define.c
	gcc -m32 -fno-asynchronous-unwind-tables -s -c -o global_define.o global_define.c
multi_task.o : multi_task.h multi_task.c
	gcc -m32 -fno-asynchronous-unwind-tables -s -c -o multi_task.o multi_task.c

kernel.bat: #inlcude ckernel.asm
	nasm kernel.asm -o kernel.bat
boot.bat:
	nasm boot.asm -o boot.bat
system.img:
	./makefloppy/makefloppy boot.bat kernel.bat system.img

clean:
	rm -rf *.o *.bat ckernel_u.asm ckernel.asm system.img
