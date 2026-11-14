struct ResetEntry {
    int field_00;
    int field_04;
    int field_08;
};

struct ResetCtx {
    char pad_0000[0x1EA0];
    int field_1EA0;
    char pad_1EA4[0x1EC8 - 0x1EA4];
    ResetEntry field_1EC8[2];
};

struct ResetOwner {
    char pad_0000[0x1EE4];
    ResetCtx *field_1EE4;
};

extern "C" {

void func_08AA488C(ResetOwner *self)
{
    self->field_1EE4->field_1EA0 = 0;
    for (int i = 0; i < 2; i++) {
        self->field_1EE4->field_1EC8[i].field_00 = 0;
        self->field_1EE4->field_1EC8[i].field_04 = -1;
        self->field_1EE4->field_1EC8[i].field_08 = -1;
    }
}

}
