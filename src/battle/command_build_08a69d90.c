typedef struct CommandList08A69D90 {
    unsigned char active;
} CommandList08A69D90;

typedef struct CommandOwner08A69D90 {
    unsigned char active;
    char pad_001[0x6F];
    CommandList08A69D90 *list;
} CommandOwner08A69D90;

extern int func_08A65A84(CommandList08A69D90 *list);
extern void func_08A65AAC(CommandList08A69D90 *list);
extern void func_08A65AFC(CommandList08A69D90 *list);
extern void func_08A65B50(CommandList08A69D90 *list, int command);

int func_08A69D90(CommandOwner08A69D90 *self)
{
    if (!self->active) {
        return -1;
    }
    func_08A65A84(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 0x3A);
    func_08A65AFC(self->list);
    return 0;
}
