typedef struct CommandList08A69FE0 {
    unsigned char active;
} CommandList08A69FE0;

typedef struct CommandOwner08A69FE0 {
    unsigned char active;
    char pad_001[0x6F];
    CommandList08A69FE0 *list;
} CommandOwner08A69FE0;

extern int func_08A65A84(CommandList08A69FE0 *list);
extern void func_08A65AAC(CommandList08A69FE0 *list);
extern void func_08A65AFC(CommandList08A69FE0 *list);
extern void func_08A65B50(CommandList08A69FE0 *list, int command);
extern void func_08A65C08(CommandList08A69FE0 *list, int value);

int func_08A69FE0(CommandOwner08A69FE0 *self)
{
    if (!self->active) {
        return -1;
    }
    func_08A65A84(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 0x39);
    func_08A65C08(self->list, 0);
    func_08A65AFC(self->list);
    return 0;
}
