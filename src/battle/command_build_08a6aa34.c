typedef struct CommandList08A6AA34 {
    unsigned char active;
} CommandList08A6AA34;

typedef struct CommandOwner08A6AA34 {
    unsigned char active;
    char pad_001[0x6F];
    CommandList08A6AA34 *list;
} CommandOwner08A6AA34;

extern int func_08A65A84(CommandList08A6AA34 *list);
extern void func_08A65AAC(CommandList08A6AA34 *list);
extern void func_08A65AFC(CommandList08A6AA34 *list);
extern void func_08A65B50(CommandList08A6AA34 *list, int command);
extern void func_08A65BAC(CommandList08A6AA34 *list, int count);

int func_08A6AA34(CommandOwner08A6AA34 *self)
{
    if (!self->active) {
        return -1;
    }
    func_08A65A84(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 0x44);
    func_08A65BAC(self->list, 0xE);
    func_08A65AFC(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 0x45);
    func_08A65BAC(self->list, 0xE);
    func_08A65AFC(self->list);
    return 0;
}
