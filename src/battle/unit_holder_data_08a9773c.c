typedef struct UnitHolder {
    unsigned char active;
    char pad_01[0x37];
    char data[0x10];
} UnitHolder;

extern int func_08A9771C(UnitHolder *self);

char *func_08A9773C(UnitHolder *self)
{
    if (self->active == 0) {
        return 0;
    }
    if (func_08A9771C(self) == 0) {
        return 0;
    }
    return self->data;
}
