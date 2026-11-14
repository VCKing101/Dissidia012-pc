typedef struct Info { char data_00[2]; unsigned char packed; } Info;
typedef struct Save { char data_00000[0x1DE04]; unsigned char levels[120]; } Save;
extern Save *D_089D712C;
extern int func_088F44DC(int id, int flags);
extern Info *func_088C5140(int id);

unsigned char func_088F47E0(int id)
{
    int levels = (int)D_089D712C->levels;
    if (id < 0 || id >= 120) {
        return 0;
    }
    if (func_088F44DC(id, 1) != 0) {
        return *(unsigned char *)(id + levels);
    } else {
        Info *info = func_088C5140(id);
        return (info->packed & 0xF0) >> 4;
    }
}
