  
源自codingDisney大佬的内核课程源码，代码略有一点调整，用于归档

## 增加了c++版本的makefloppy

使用前先构建makefloppy，进入makefloppy目录，执行build.sh生成  
需要安装nasm以及objconv工具，使用Makefile构建


使用Makefile编译，构建执行 make，生成system.img，使用virtualbox启动


## 显示模式改成了1024*768  
kernel.asm
```
LABEL_MEM_CHK_OK:
     ;mov   al, 0x13
     ;mov   ah, 0x0
     mov   bx, 0x4105
     mov   ax, 0x4f02
     int   0x10
```
write_vga_desktop.c
```
void initBootInfo(struct BOOTINFO *pBootInfo) {
    // pBootInfo->vgaRam = (char*)0xa0000;
    // pBootInfo->screenX = 320;
    // pBootInfo->screenY = 200;
	pBootInfo->vgaRam = (char*)0xe0000000;
    pBootInfo->screenX = 1024;
    pBootInfo->screenY = 768;
}
```

## virtualbox虚拟机内存设置为16m
write_vga_desktop.c
```
void CMain(void) {
 
    //virtualbox虚拟机分配16m内存，可用内存段范围 base:0x00100000 size:0x00DE0000
    //内存管理器基地址 0x00100000，分配0x8000长度
    memman_free(memman, 0x00108000, 0x00DE0000 - 0x00008000); 

    //...

}
```
