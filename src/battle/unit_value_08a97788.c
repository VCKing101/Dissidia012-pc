typedef struct ValueUnit {
    unsigned char active;
    char pad_001[0x14F];
    int value;
} ValueUnit;

extern int func_08A9771C(ValueUnit *self);

int func_08A97788(ValueUnit *self)
{
    if (self->active == 0) {
        return -1;
    }
    if (func_08A9771C(self) == 0) {
        return -1;
    }
    return self->value;
}
