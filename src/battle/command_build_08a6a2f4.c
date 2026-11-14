typedef struct CommandList08A6A2F4 {
    unsigned char active;
} CommandList08A6A2F4;

typedef struct CommandOwner08A6A2F4 {
    unsigned char active;
    char pad_001[0x6F];
    CommandList08A6A2F4 *list;
} CommandOwner08A6A2F4;

extern int func_08A65A84(CommandList08A6A2F4 *list);
extern void func_08A65AAC(CommandList08A6A2F4 *list);
extern void func_08A65AFC(CommandList08A6A2F4 *list);
extern void func_08A65B50(CommandList08A6A2F4 *list, int command);
extern void func_08A65BAC(CommandList08A6A2F4 *list, int count);

int func_08A6A2F4(CommandOwner08A6A2F4 *self)
{
    if (!self->active) {
        return -1;
    }
    func_08A65A84(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 0x3B);
    func_08A65BAC(self->list, 0x11);
    func_08A65AFC(self->list);
    return 0;
}
