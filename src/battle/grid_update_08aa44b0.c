typedef struct GridState {
    unsigned char active;
    char pad_01[0x1F];
    unsigned char mode;
} GridState;

extern void func_08AA48E0(GridState *self, int x, int y);
extern void func_08AA49D4(GridState *self, int y);

int func_08AA44B0(GridState *self, int x, int y)
{
    if (self->active == 0) {
        return -1;
    }
    func_08AA48E0(self, x, y);
    if (self->mode == 1) {
        func_08AA49D4(self, y);
    }
    return 0;
}
