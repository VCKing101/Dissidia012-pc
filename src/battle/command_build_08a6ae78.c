typedef struct CommandList6AE78 {
    unsigned char active;
} CommandList6AE78;

typedef struct CommandSlot6AE78 {
    char pad_000[0x20];
    int field_20;
} CommandSlot6AE78;

typedef struct CommandOwner6AE78 {
    unsigned char active;
    char pad_001[0x63];
    int field_64;
    char pad_068[0x8];
    CommandList6AE78 *list;
} CommandOwner6AE78;

extern float D_08A7ABEC;
extern CommandSlot6AE78 *func_08AB3264(int id);
extern int func_08A65A84(CommandList6AE78 *list);
extern void func_08A65AAC(CommandList6AE78 *list);
extern void func_08A65AFC(CommandList6AE78 *list);
extern void func_08A65B50(CommandList6AE78 *list, int command);
extern void func_08A65BAC(CommandList6AE78 *list, int count);
extern void func_08A65C64(CommandList6AE78 *list, float value);

int func_08A6AE78(CommandOwner6AE78 *self, int value)
{
    if (!self->active) {
        return -1;
    }
    func_08AB3264(self->field_64)->field_20 = value;
    func_08A65A84(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 0x1C);
    func_08A65B50(self->list, 3);
    func_08A65C64(self->list, D_08A7ABEC);
    func_08A65BAC(self->list, 0xD);
    func_08A65AFC(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 0x1A);
    func_08A65B50(self->list, 0x1D);
    func_08A65AFC(self->list);
    return 0;
}
