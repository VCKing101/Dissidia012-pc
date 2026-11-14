typedef struct CommandList08A6ABEC {
    unsigned char active;
} CommandList08A6ABEC;

typedef struct CommandOwner08A6ABEC {
    unsigned char active;
    char pad_001[0x6F];
    CommandList08A6ABEC *list;
} CommandOwner08A6ABEC;

extern int func_08A65A84(CommandList08A6ABEC *list);
extern void func_08A65AAC(CommandList08A6ABEC *list);
extern void func_08A65AFC(CommandList08A6ABEC *list);
extern void func_08A65B50(CommandList08A6ABEC *list, int command);

int func_08A6ABEC(CommandOwner08A6ABEC *self)
{
    if (!self->active) {
        return -1;
    }
    func_08A65A84(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 0x47);
    func_08A65AFC(self->list);
    return 0;
}
