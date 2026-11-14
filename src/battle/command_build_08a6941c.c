typedef struct CommandList08A6941C {
    unsigned char active;
} CommandList08A6941C;

typedef struct CommandOwner08A6941C {
    unsigned char active;
    char pad_001[0x6F];
    CommandList08A6941C *list;
} CommandOwner08A6941C;

extern int func_08A65A84(CommandList08A6941C *list);
extern void func_08A65AAC(CommandList08A6941C *list);
extern void func_08A65AFC(CommandList08A6941C *list);
extern void func_08A65B50(CommandList08A6941C *list, int command);
extern void func_08A65C08(CommandList08A6941C *list, int value);

int func_08A6941C(CommandOwner08A6941C *self)
{
    if (!self->active) {
        return -1;
    }
    func_08A65A84(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 0xF);
    func_08A65C08(self->list, 7);
    func_08A65B50(self->list, 0x34);
    func_08A65AFC(self->list);
    return 0;
}
