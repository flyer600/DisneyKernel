#include "mem_util.h"

void memman_init(struct MEMMAN *man) {
    man->frees = 0;  //当前有多少个空闲块（FREEINFO对象数）可以使用的
    man->maxfrees = 0; //当前可用内存对象
    man->lostsize = 0;
    man->losts = 0;
}

unsigned int memman_total(struct MEMMAN *man) {
    unsigned int i, t = 0;
    for (i = 0; i < man->frees; i++) {
        t += man->free[i].size;
    }

    return t;
}

unsigned int memman_alloc(struct MEMMAN *man, unsigned int size) {
    unsigned int i, a;
    for (i = 0; i < man->frees; i++) {
        if (man->free[i].size >= size) {
            a = man->free[i].addr;
            man->free[i].addr += size; //bug
            man->free[i].size -= size;
            if (man->free[i].size == 0) {
                man->frees--;
                for (; i < man->frees; i++) {
                    man->free[i] = man->free[i+1];
                }
            }

            return a;
        }
    }

    return 0;
}

int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size) {
    int i, j;
    for (i = 0; i < man->frees; i++) {
        if (man->free[i].addr > addr) {
            break;
        }
    }

    if (i > 0) { //合并待释放内存以及前面的连续内存
        if (man->free[i-1].addr + man->free[i-1].size == addr) {
           man->free[i-1].size += size;
           if (i < man->frees) {
               if (addr + size == man->free[i].addr) {
                   man->free[i-1].size += man->free[i].size;
                   man->frees--;
                   for (; i < man->frees; i++) {
                       man->free[i] = man->free[i+1];
                   }
               }
           }
     
           return 0;
        }
    }

    if (i < man->frees) { //合并待释放内存以及后面的连续内存
        if (addr + size == man->free[i].addr) {
           man->free[i].addr = addr;
           man->free[i].size += size;
           return 0;
        }
    }

    if (man->frees < MEMMAN_FREES) {
        //调整frees结构的地址顺序，保证地址从小到大递增
        //调整的目的是为了将回收对象插入到i位置，原来i位置的对象依次往后移动
        for (j = man->frees; j > i; j--) {
            man->free[j] = man->free[j-1];
        }

        man->frees++;
        if (man->maxfrees < man->frees) {
            man->maxfrees = man->frees; //更正当前最大对象数
        }

        man->free[i].addr = addr; //插入回收对象
        man->free[i].size = size;
        return 0;
    }

    //frees结构用完，没有资源存储新的回收对象
    man->losts++;
    man->lostsize += size;
    return -1;
}

unsigned int memman_alloc_4k(struct MEMMAN *man, unsigned int size) {
    unsigned int a;
    size = (size + 0xfff) & 0xfffff000; //4k对齐
    a = memman_alloc(man, size);
    return a;
}

int memman_free_4k(struct MEMMAN *man, unsigned int addr, unsigned int size) {
    int i ;
    size = (size + 0xfff) & 0xfffff000; //4k对齐
    i = memman_free(man, addr, size);
    return i;
}
