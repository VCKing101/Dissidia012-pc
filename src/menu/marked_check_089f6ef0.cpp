struct CheckInfo {
    char pad_00[0x20];
    unsigned short field_20;

    static bool isMarked(const CheckInfo *info) { return (info->field_20 & 0x20) != 0; }
};

struct CheckState {
    char pad_000[0x6AC];
    CheckInfo *field_6AC;
    char pad_6B0[0x3210 - 0x6B0];
    int field_3210;
};

struct CheckOwner {
    char pad_000[0x14C];
    CheckState *field_14C;
};

extern "C" int func_089F2758(CheckOwner *self, int *slot);

extern "C" {
int func_089F6EF0(CheckOwner *self)
{
    CheckState *state = self->field_14C;
    int result = 1;
    if (CheckInfo::isMarked(state->field_6AC)) {
        if (func_089F2758(self, &state->field_3210) == 0) {
            result = 2;
        }
    }
    return result;
}
}
