struct SHEET {
    unsigned char *buf; //窗口的像素信息
    int bxsize, bysize; //窗口的宽高
    int vx0, vy0; //窗口的左上角坐标
    int col_inv; //窗口不显示的部分（透明，显示底下层的颜色）
    int height; //图层的高度
    int flags; //图层的状态
	struct TASK *task;
};

#define MAX_SHEETS  256
struct SHTCTL {
    unsigned char *vram, *map;
    int xsize, ysize, top;
    struct SHEET *sheets[MAX_SHEETS];
    struct SHEET sheets0[MAX_SHEETS];
};

#define SIZE_OF_SHEET  32
#define SIZE_OF_SHTCTL 9232

struct SHEET *sheet_alloc(struct SHTCTL *ctl);
struct SHTCTL *shtctl_init(struct MEMMAN *memman, unsigned char *vram,
  int xsize, int ysize);
void sheet_setbuf(struct SHEET *sht, unsigned char *buf, int xsize, int ysize,
    int col_inv);
void sheet_updown(struct SHTCTL *ctl, struct SHEET *sht, int height);
int sheet_refresh(struct SHTCTL *ctl, struct SHEET *sht, int bx0, int by0, int bx1, int by1);
void sheet_slide(struct SHTCTL *ctl, struct SHEET *sht, int vx0, int vy0);
void sheet_refreshsub(struct SHTCTL *ctl, int vx0, int vy0, int vx1, int vy1, int h0, int h1);

void sheet_refreshmap(struct SHTCTL *ctl, int vx0, int vy0, int vx1, int vy1, int h0);

void sheet_free(struct SHTCTL* shtctl,struct SHEET *sht);
