typedef struct LevelState {
    char pad_0000[0x4C];
    short *field_4C;
    char pad_0050[0x2738 - 0x50];
    unsigned short field_2738;
} LevelState;

typedef struct LevelOwner {
    char pad_000[0x14C];
    LevelState *field_14C;
} LevelOwner;

unsigned char func_089F00C8(LevelOwner *self, unsigned short value)
{
    unsigned char level = 0;
    int i;
    for (i = 1; i < 6; i++) {
        if (value < self->field_14C->field_4C[i]) {
            break;
        }
        level++;
    }
    level = level + self->field_14C->field_2738;
    if (level > 5) {
        level = 5;
    }
    return level;
}
