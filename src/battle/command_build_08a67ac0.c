typedef struct CommandList67AC0 {
    unsigned char active;
} CommandList67AC0;

typedef struct CommandSlot67AC0 {
    char pad_000[0x4];
    void *unit;
} CommandSlot67AC0;

typedef struct CommandOwner67AC0 {
    unsigned char active;
    char pad_001[0x63];
    int field_64;
    char pad_068[0x8];
    CommandList67AC0 *list;
} CommandOwner67AC0;

extern float D_08A7ABE8;
extern int func_08A65A84(CommandList67AC0 *list);
extern void func_08A65AAC(CommandList67AC0 *list);
extern void func_08A65B50(CommandList67AC0 *list, int command);
extern void func_08A65BAC(CommandList67AC0 *list, int count);
extern void func_08A65C64(CommandList67AC0 *list, float value);
extern void func_08A65AFC(CommandList67AC0 *list);
extern CommandSlot67AC0 *func_08AB3264(int id);

int func_08A67B08(CommandOwner67AC0 *self, float value);

int func_08A67AC0(CommandOwner67AC0 *self)
{
    if (!self->active) {
        return -1;
    }
    if (func_08A67B08(self, D_08A7ABE8)) {
        return -2;
    }
    return 0;
}

int func_08A67B08(CommandOwner67AC0 *self, float value)
{
    if (!self->active) {
        return -1;
    }
    func_08A65A84(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 2);
    func_08A65C64(self->list, value);
    func_08A65AFC(self->list);
    return 0;
}

int func_08A67B78(CommandOwner67AC0 *self)
{
    if (!self->active) {
        return -1;
    }
    if (func_08A67AC0(self)) {
        return -2;
    }
    return 0;
}

int func_08A67BBC(CommandOwner67AC0 *self)
{
    if (!self->active) {
        return -1;
    }
    if (!func_08AB3264(self->field_64)->unit) {
        return -2;
    }
    func_08A65A84(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 0x4A);
    func_08A65AFC(self->list);
    return 0;
}

int func_08A67C30(CommandOwner67AC0 *self)
{
    if (!self->active) {
        return -1;
    }
    if (!func_08AB3264(self->field_64)->unit) {
        return -2;
    }
    func_08A65A84(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 0x1F);
    func_08A65B50(self->list, 0x20);
    func_08A65BAC(self->list, 3);
    func_08A65AFC(self->list);
    return 0;
}
