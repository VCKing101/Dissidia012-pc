typedef struct CommandList08A69DE8 {
    unsigned char active;
} CommandList08A69DE8;

typedef struct CommandOwner08A69DE8 {
    unsigned char active;
    char pad_001[0x6F];
    CommandList08A69DE8 *list;
} CommandOwner08A69DE8;

extern int func_08A65A84(CommandList08A69DE8 *list);
extern void func_08A65AAC(CommandList08A69DE8 *list);
extern void func_08A65AFC(CommandList08A69DE8 *list);
extern void func_08A65B50(CommandList08A69DE8 *list, int command);
extern void func_08A65BAC(CommandList08A69DE8 *list, int count);

int func_08A69DE8(CommandOwner08A69DE8 *self)
{
    if (!self->active) {
        return -1;
    }
    func_08A65A84(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 0x31);
    func_08A65B50(self->list, 0x35);
    func_08A65BAC(self->list, 0xC);
    func_08A65AFC(self->list);
    return 0;
}
