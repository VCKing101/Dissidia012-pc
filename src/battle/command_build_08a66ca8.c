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
extern float func_08AB00A0(int id);

int func_08A66CA8(CommandOwner *self)
{
    float value;
    if (!self->active) {
        return -1;
    }
    value = func_08AB00A0(func_08AB327C(self->unit));
    func_08A65A84(self->list);
    if (value > 0.0f) {
        func_08A65AAC(self->list);
        func_08A65B50(self->list, 6);
        func_08A65B50(self->list, 2);
        func_08A65C64(self->list, value);
        func_08A65AFC(self->list);
    }
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 0);
    func_08A65C08(self->list, 1);
    func_08A65AFC(self->list);
    return 0;
}
