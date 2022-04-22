#include <iostream>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include "floppy.h"

Floppy floppyDisk;

void writeFileToFloppy(string fileName, bool bootable, int cylinder, int beginSec) { 

    if (access(fileName.c_str(), F_OK ) == -1) {
        return;
    }

    char buf[512] = {0};
    //std::cout << "now to process:" << fileName << std::endl;

    std::ifstream fin(fileName.c_str(), std::ifstream::in | std::ifstream::binary);
    while (true) {
        fin.read(buf, sizeof(buf));
        if (bootable) {
            buf[510] = 0x55;
            buf[511] = 0xaa;
        }

        floppyDisk.writeFloppy(MAGNETIC_HEAD_0, cylinder, beginSec, buf); //盘面  柱道(磁道)  扇区
        std::cout << "Load file to floppy, fileName:" << fileName << " cylinder:" << cylinder << " sector:" << beginSec << std::endl;

        if (beginSec >= MAX_SECTOR_NUM) {
            beginSec = 0;
            cylinder++;
        }

        beginSec++;
        memset(buf, 0, sizeof(buf));

        if (fin.eof()) {
            break;
        }
    }
}

int  main(int argc, char** argv)
{
    if (argc < 3) {
        std::cout << "usage: makefloppy boot.bat kernel.bat system.img" << std::endl;
        return 0;
    }

    std::string bootfile = argv[1];
    std::string kernelfile = argv[2];
    std::string imgfile = argv[3];

    writeFileToFloppy(bootfile, true, 0, 1); //boot.bat
    writeFileToFloppy(kernelfile, false, 1, 1); //kernel.bat
    floppyDisk.makeFloppy(imgfile); //system.img
}