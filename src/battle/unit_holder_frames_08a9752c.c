typedef struct UnitHolder {
    unsigned char active;
    char pad_01[0x1B];
    int frames;
} UnitHolder;

extern int D_08AA5828;
extern float func_08AA57D0(int id);

int func_08A9752C(UnitHolder *self)
{
    if (self->active == 0) {
        return -1;
    }
    self->frames = (int)func_08AA57D0(D_08AA5828);
    return 0;
}
