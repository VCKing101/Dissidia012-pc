typedef struct LevelInfo {
    unsigned char active;
    char pad_01[0xBB];
    unsigned int level;
} LevelInfo;

typedef struct LevelGate {
    unsigned char active;
    char pad_01[0x13];
    unsigned int level;
} LevelGate;

extern int D_08AAAD80;
extern LevelInfo *func_08AA79C4(int id);

int func_08A94648(LevelGate *self)
{
    LevelInfo *info;

    if (self->active == 0) {
        return 0;
    }
    info = func_08AA79C4(D_08AAAD80);
    if (info->active == 0) {
        return 0;
    }
    return !(self->level < info->level);
}
