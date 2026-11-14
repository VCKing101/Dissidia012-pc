typedef struct CommandList {
    unsigned char active;
} CommandList;

typedef struct CommandOwner {
    unsigned char active;
    char pad_001[0x63];
    int unit;
    char pad_068[0x8];
    CommandList *list;
} CommandOwner;

extern int func_08AB327C(int unit);
extern int func_08A65A84(CommandList *list);
extern void func_08A65AAC(CommandList *list);
extern void func_08A65B50(CommandList *list, int command);
extern void func_08A65BAC(CommandList *list, int value);
extern void func_08A65C08(CommandList *list, int value);
extern void func_08A65C64(CommandList *list, float value);
extern void func_08A65AFC(CommandList *list);
extern int func_08A67CBC(CommandOwner *self);

int func_08A69328(CommandOwner *self)
{
    if (!self->active) {
        return -1;
    }
    if (func_08A67CBC(self) != 0) {
        return -2;
    }
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 0);
    func_08A65C08(self->list, 2);
    func_08A65C08(self->list, 2);
    func_08A65C08(self->list, 2);
    func_08A65C08(self->list, 1);
    func_08A65AFC(self->list);
    return 0;
}
