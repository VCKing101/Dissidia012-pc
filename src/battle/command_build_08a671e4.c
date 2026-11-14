typedef struct CommandList {
    unsigned char active;
} CommandList;

typedef struct CommandOwner671E4 {
    unsigned char active;
    char pad_001[0x6F];
    CommandList *list;
} CommandOwner671E4;

extern int func_08A65A84(CommandList *list);
extern void func_08A65AAC(CommandList *list);
extern void func_08A65B50(CommandList *list, int command);
extern void func_08A65C08(CommandList *list, int value);
extern void func_08A65AFC(CommandList *list);

int func_08A671E4(CommandOwner671E4 *self)
{
    if (!self->active) {
        return -1;
    }
    func_08A65A84(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 0xF);
    func_08A65C08(self->list, 0);
    func_08A65B50(self->list, 0x10);
    func_08A65B50(self->list, 0x11);
    func_08A65B50(self->list, 0x12);
    func_08A65B50(self->list, 0x48);
    func_08A65AFC(self->list);
    return 0;
}

int func_08A67278(CommandOwner671E4 *self)
{
    if (!self->active) {
        return -1;
    }
    func_08A65A84(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 0x13);
    func_08A65B50(self->list, 0x14);
    func_08A65B50(self->list, 0x15);
    func_08A65B50(self->list, 0x48);
    func_08A65AFC(self->list);
    return 0;
}
