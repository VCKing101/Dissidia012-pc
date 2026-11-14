typedef struct CommandList08A6AC44 {
    unsigned char active;
} CommandList08A6AC44;

typedef struct CommandOwner08A6AC44 {
    unsigned char active;
    char pad_001[0x6F];
    CommandList08A6AC44 *list;
} CommandOwner08A6AC44;

extern int func_08A65A84(CommandList08A6AC44 *list);
extern void func_08A65AAC(CommandList08A6AC44 *list);
extern void func_08A65AFC(CommandList08A6AC44 *list);
extern void func_08A65B50(CommandList08A6AC44 *list, int command);

int func_08A6AC44(CommandOwner08A6AC44 *self)
{
    if (!self->active) {
        return -1;
    }
    func_08A65A84(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 0x49);
    func_08A65AFC(self->list);
    return 0;
}
