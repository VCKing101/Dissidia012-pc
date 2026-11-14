typedef struct Setup08A062BC {
    unsigned char active;
    char pad_01[0x1B];
    unsigned int field_1C;
    void *field_20;
    void *field_24;
    void *field_28;
    void *field_2C;
} Setup08A062BC;

extern void func_08A06264(Setup08A062BC *self);

/* Validates the arguments and records them; -1 when one is out of range. */
int func_08A062BC(Setup08A062BC *self, unsigned int side, void *first, void *second, void *third, void *fourth)
{
    func_08A06264(self);
    if (side >= 2 || first == 0 || second == 0 || third == 0 || fourth == 0) {
        return -1;
    }
    self->field_1C = side;
    self->field_20 = first;
    self->field_24 = second;
    self->field_28 = third;
    self->field_2C = fourth;
    self->active = 1;
    return 0;
}
