typedef struct LightInfo {
    char pad00[0x20];
    unsigned char kind;     /* 0x20 */
} LightInfo;

typedef struct LightOwner {
    char pad00[0x14];
    int handle;             /* 0x14 */
} LightOwner;

extern int D_08B2C238[];
extern LightInfo *func_088CA290(int handle);

int func_08B2C1CC(LightOwner *owner)
{
    LightInfo *info = func_088CA290(owner->handle);

    if (info == 0) {
        return 0x2D;
    }
    if (info->kind >= 10u) {
        return 0x2D;
    }
    return D_08B2C238[info->kind];
}
