#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <string.h>

using namespace std;

const int MAX_MAGNETIC_NUM = 2; //共两个盘面
const int MAX_CYLINDER_NUM = 80; //单盘面共80磁道
const int MAX_SECTOR_NUM   = 18; //单磁道共18扇区

enum MAGNETIC_HEAD 
{
    MAGNETIC_HEAD_0,
    MAGNETIC_HEAD_1
};

class Floppy 
{
public:
    int SECTOR_SIZE;
    int CYLINDER_COUNT;
    int SECTORS_COUNT;

    MAGNETIC_HEAD magneticHead;
    int current_cylinder;
    int current_sector;

    std::map<int, std::vector<std::vector<char*> > > floppy;

    Floppy();

    void makeFloppy(string fileName);
    char* readFloppy(MAGNETIC_HEAD head, int cylinder_num, int sector_num);
    void writeFloppy(MAGNETIC_HEAD head, int cylinder_num, int sector_num, char* buf);

    void setMagneticHead(MAGNETIC_HEAD head);
    void setCylinder(int cylinder);
    void setSector(int sector);
    vector<char*> initCylinder();
    vector<vector<char*> > initFloppyDisk();
};
    