typedef struct LevelInfo {
    unsigned char active;
    char pad_01[0xBB];
    unsigned int level;
} LevelInfo;

typedef struct TargetSet {
    unsigned char active;
    char pad_01[0x17];
    unsigned int targets[3];
} TargetSet;

extern "C" {

extern int D_08AAAD80;
extern LevelInfo *func_08AA79C4(int id);

int func_08A94798(TargetSet *self, unsigned int id)
{
    LevelInfo *info = func_08AA79C4(D_08AAAD80);

    if (info->active == 0) {
        return 0;
    }
    if (id >= info->level) {
        return 0;
    }
    int result = 0;
    for (int i = 0; i < 3; i++) {
        if (self->targets[i] == id) {
            result = 1;
            break;
        }
    }
    return result;
}

}
