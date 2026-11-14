typedef struct LevelState {
    char pad_0000[0x488];
    int field_488[1];
    char pad_048C[0x2736 - 0x48C];
    unsigned short field_2736;
    char pad_2738[0x3224 - 0x2738];
    int field_3224;
    int field_3228;
    short field_322C;
    short field_322E;
} LevelState;

typedef struct LevelOwner {
    char pad_000[0x14C];
    LevelState *field_14C;
} LevelOwner;

extern unsigned short D_08A0B33C[];
extern unsigned char func_089F00C8(LevelOwner *self, unsigned short value);
extern void func_088D9F5C(int target, int kind, int value);
extern void func_089F3B70(LevelOwner *self);

void func_089F37F8(LevelOwner *self)
{
    int count = func_089F00C8(self, self->field_14C->field_2736);
    int i;
    self->field_14C->field_3224 = count;
    self->field_14C->field_3228 = 0;
    self->field_14C->field_322C = 1;
    self->field_14C->field_322E = 0;
    for (i = 0; i < count; i++) {
        func_088D9F5C(self->field_14C->field_488[D_08A0B33C[i]], 0x36, 1);
    }
    func_089F3B70(self);
}
