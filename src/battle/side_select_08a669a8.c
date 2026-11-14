typedef struct SideSelect {
    unsigned char active;
    char pad_01[0x67];
    int handle;
} SideSelect;

extern void func_08A97060(int handle, int id);

int func_08A669A8(SideSelect *self, unsigned int side)
{
    if (self->active == 0) {
        return -1;
    }
    if (side >= 2) {
        return -2;
    }
    if (side == 0) {
        func_08A97060(self->handle, 0x1B);
    } else {
        func_08A97060(self->handle, 0x1C);
    }
    return 0;
}
