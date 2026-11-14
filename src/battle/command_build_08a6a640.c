typedef struct CommandList08A6A640 {
    unsigned char active;
} CommandList08A6A640;

typedef struct CommandOwner08A6A640 {
    unsigned char active;
    char pad_001[0x6F];
    CommandList08A6A640 *list;
} CommandOwner08A6A640;

extern int func_08A65A84(CommandList08A6A640 *list);
extern void func_08A65AAC(CommandList08A6A640 *list);
extern void func_08A65AFC(CommandList08A6A640 *list);
extern void func_08A65B50(CommandList08A6A640 *list, int command);
extern void func_08A65BAC(CommandList08A6A640 *list, int count);

int func_08A6A640(CommandOwner08A6A640 *self)
{
    if (!self->active) {
        return -1;
    }
    func_08A65A84(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 0x41);
    func_08A65BAC(self->list, 0x13);
    func_08A65AFC(self->list);
    return 0;
}
