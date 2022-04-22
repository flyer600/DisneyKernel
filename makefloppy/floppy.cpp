
#include "floppy.h"

/*
* 虚拟软盘是存粹的二进制文件，它的逻辑结构如下：
* 前512*18 字节的内容对应盘面0，柱面0的所有扇区内容
* 接着的512*18字节的内容对应盘面1，柱面0的所有扇区内容
* 再接着的512*18字节的内容对应盘面0，柱面1所有扇区内容
* 再接着512*18字节的内容对应盘面1，柱面1所有扇区内容
* 以此类推
*/

char to_diff_buf[512] = {0};

Floppy::Floppy()
{
    SECTOR_SIZE = 512; //单扇区大小
    CYLINDER_COUNT = 80; //80个柱面(磁道)
    SECTORS_COUNT = 18; //18个扇区

    magneticHead = MAGNETIC_HEAD_0;
    current_cylinder = 0;
    current_sector = 0;

    //一个磁盘有两个盘面
    floppy[MAGNETIC_HEAD_0] = initFloppyDisk();
    floppy[MAGNETIC_HEAD_1]  = initFloppyDisk();
}

char* Floppy::readFloppy(MAGNETIC_HEAD head, int cylinder_num, int sector_num) {
    setMagneticHead(head);
    setCylinder(cylinder_num);
    setSector(sector_num);

    vector<vector<char*> > disk;
    vector<char*> cylinder;
    char* sector = NULL;

    disk = floppy[magneticHead];
    if (disk.size() == CYLINDER_COUNT) {
        cylinder = disk[current_cylinder];
    }
    if (cylinder.size() == SECTORS_COUNT) {
        sector = cylinder[current_sector];
    }

    if (memcmp(to_diff_buf, sector, SECTOR_SIZE) == 0) {
        //std::cout << "to_diff_buf succ..." << std::endl;
    } else {
        //std::cout << "to_diff_buf fail " << magneticHead << " " << current_cylinder << " "  
        //    << current_sector << " --- " << head << " " << cylinder_num << " " << sector_num << std::endl;
    }
    return sector;
}

void Floppy::writeFloppy(MAGNETIC_HEAD head, int cylinder_num, int sector_num, char* buf) {
    setMagneticHead(head);
    setCylinder(cylinder_num);
    setSector(sector_num);

    vector<vector<char*> > disk;
    vector<char*> cylinder;
    char* sector = NULL;

    disk = floppy[magneticHead];
    if (disk.size() == CYLINDER_COUNT) {
        cylinder = disk[current_cylinder];
    }
    if (cylinder.size() == SECTORS_COUNT) {
        sector = cylinder[current_sector];
    }

    if (sector != NULL && buf != NULL) {
        memcpy(sector, buf, SECTOR_SIZE);
        //std::cout << "writeFloppy size:" << SECTOR_SIZE << " cylinder:" << cylinder_num << " sector:" << sector_num << std::endl;
    } else {
        //std::cout << "writeFloppy fail size:" << SECTOR_SIZE << " cylinder:" << cylinder_num << " sector:" << sector_num << std::endl;
    }
}

void Floppy::makeFloppy(string fileName) {
    std::ofstream out(fileName.c_str(), std::ios::binary | std::ios::out);
    for (int cylinder = 0; cylinder < CYLINDER_COUNT; cylinder++) {
        for (int head = 0; head <= MAGNETIC_HEAD_1; head++) {
            for (int sector = 1; sector <= SECTORS_COUNT; sector++) {
                char* buf = readFloppy(MAGNETIC_HEAD(head), cylinder, sector);
                if (buf != NULL) {
                    //std::cout << "makeFloppy write to disk, size:" << SECTOR_SIZE << " cylinder:" << cylinder << " head:" << head << " sector:" << sector << std::endl;
                    out.write(buf, SECTOR_SIZE);
                } else {
                    //std::cout << "makeFloppy write to disk fail, cylinder:" << cylinder << " head:" << head << " sector:" << sector << std::endl;
                }
            }
        }
    }
}

vector<vector<char*> > Floppy::initFloppyDisk() {
    vector<vector<char*> > floppyDisk; //磁盘的一个面 一个磁盘面有80个柱面
    for(int i = 0; i < CYLINDER_COUNT; i++) {
        floppyDisk.push_back(initCylinder());
    }
    return floppyDisk;
}

vector<char*> Floppy::initCylinder() {
    vector<char*> cylinder;
    for (int i = 0; i < SECTORS_COUNT; i++) {
        char* sector = new char[SECTOR_SIZE];
        //std::cout << "sizeof sector:" << sizeof(sector) << std::endl; //sizeof just mean: the size of ponter!!!!!
        memset(sector, 0, SECTOR_SIZE);
        cylinder.push_back(sector);
    }
    return cylinder;
}

void Floppy::setMagneticHead(MAGNETIC_HEAD head) {
    magneticHead = head;
}
    
void Floppy::setCylinder(int cylinder) {
    if (cylinder < 0) {
        current_cylinder = 0;
    }
    else if (cylinder >= 80) {
        current_cylinder = 79;
    }
    else {
        current_cylinder = cylinder;
    }
}
    
void Floppy::setSector(int sector) {
    if (sector < 0) {
        current_sector = 0;
    }
    else if (sector > 18) {
        current_sector = 18 - 1;
    }
    else {
        current_sector = sector - 1;
    }
}

 