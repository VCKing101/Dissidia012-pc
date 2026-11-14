typedef struct SlotValue7F10C {
    unsigned char active;
    char pad_001[0x97];
    int field_98;
} SlotValue7F10C;

int func_08A7F10C(SlotValue7F10C *self)
{
    if (!self->active) {
        return -1;
    }
    return self->field_98;
}
