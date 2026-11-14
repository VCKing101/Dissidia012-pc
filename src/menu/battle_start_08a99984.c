typedef struct StartState {
    char pad_0000[0x910];
    int field_910;
    char pad_0914[0x81B4 - 0x914];
    unsigned int field_81B4;
    int field_81B8;
    char pad_81BC[8];
    int field_81C4;
} StartState;

typedef struct StartOwner {
    char pad_000[0x154];
    StartState *field_154;
    char pad_158[0x894 - 0x158];
    char field_894[4];
} StartOwner;

extern void func_08AA0D48(StartOwner *self, int value);
extern void func_08AA06A4(StartOwner *self);
extern void func_08AB58E0(char *menu, int kind, int value);
extern void func_08AB5790(char *menu, int kind);
extern void func_08A9C55C(StartOwner *self, int value);
extern void func_08A9BEA0(StartOwner *self);

void func_08A99984(StartOwner *self)
{
    func_08AA0D48(self, 0);
    func_08AA06A4(self);
    self->field_154->field_910 = 0x29;
    if ((self->field_154->field_81B4 & 1) != 0) {
        func_08AB58E0(self->field_894, 6, 0);
        func_08AB58E0(self->field_894, 8, self->field_154->field_81B8);
        func_08AB5790(self->field_894, 2);
    }
    func_08A9C55C(self, 0);
    self->field_154->field_81C4 = 0;
    func_08A9BEA0(self);
}
