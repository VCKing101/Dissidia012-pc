typedef struct CommandList67988 {
    unsigned char active;
} CommandList67988;

typedef struct CommandOwner67988 {
    unsigned char active;
    char pad_001[0x6F];
    CommandList67988 *list;
} CommandOwner67988;

extern int func_08A65A84(CommandList67988 *list);
extern void func_08A65AAC(CommandList67988 *list);
extern void func_08A65B50(CommandList67988 *list, int command);
extern void func_08A65BAC(CommandList67988 *list, int count);
extern void func_08A65C08(CommandList67988 *list, int value);
extern void func_08A65AFC(CommandList67988 *list);

int func_08A67988(CommandOwner67988 *self)
{
    if (!self->active) {
        return -1;
    }
    func_08A65A84(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 0xF);
    func_08A65C08(self->list, 0);
    func_08A65B50(self->list, 0x16);
    func_08A65B50(self->list, 0x17);
    func_08A65B50(self->list, 0x12);
    func_08A65B50(self->list, 0x48);
    func_08A65BAC(self->list, 0x10);
    func_08A65AFC(self->list);
    return 0;
}

int func_08A67A28(CommandOwner67988 *self, unsigned char value)
{
    if (!self->active) {
        return -1;
    }
    func_08A65A84(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 0x19);
    func_08A65AFC(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 0);
    func_08A65C08(self->list, 0xC);
    func_08A65C08(self->list, value);
    func_08A65AFC(self->list);
    return 0;
}
