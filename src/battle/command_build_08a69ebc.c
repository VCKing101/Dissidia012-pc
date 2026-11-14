typedef struct CommandList08A69EBC {
    unsigned char active;
} CommandList08A69EBC;

typedef struct CommandOwner08A69EBC {
    unsigned char active;
    char pad_001[0x6F];
    CommandList08A69EBC *list;
} CommandOwner08A69EBC;

extern int func_08A65A84(CommandList08A69EBC *list);
extern void func_08A65AAC(CommandList08A69EBC *list);
extern void func_08A65AFC(CommandList08A69EBC *list);
extern void func_08A65B50(CommandList08A69EBC *list, int command);
extern void func_08A65BAC(CommandList08A69EBC *list, int count);
extern void func_08A65C08(CommandList08A69EBC *list, int value);

int func_08A69EBC(CommandOwner08A69EBC *self, int value)
{
    if (!self->active) {
        return -1;
    }
    func_08A65A84(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 0x31);
    func_08A65B50(self->list, 0x37);
    func_08A65C08(self->list, value);
    func_08A65BAC(self->list, 0xC);
    func_08A65AFC(self->list);
    return 0;
}
