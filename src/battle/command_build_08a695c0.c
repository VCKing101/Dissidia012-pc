typedef struct CommandList08A695C0 {
    unsigned char active;
} CommandList08A695C0;

typedef struct CommandOwner08A695C0 {
    unsigned char active;
    char pad_001[0x6F];
    CommandList08A695C0 *list;
} CommandOwner08A695C0;

extern float D_08A7ABE8;
extern int func_08A65A84(CommandList08A695C0 *list);
extern void func_08A65AAC(CommandList08A695C0 *list);
extern void func_08A65AFC(CommandList08A695C0 *list);
extern void func_08A65B50(CommandList08A695C0 *list, int command);
extern void func_08A65BAC(CommandList08A695C0 *list, int count);
extern void func_08A65C08(CommandList08A695C0 *list, int value);
extern void func_08A65C64(CommandList08A695C0 *list, float value);

int func_08A695C0(CommandOwner08A695C0 *self, int value)
{
    if (!self->active) {
        return -1;
    }
    func_08A65A84(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 0x1E);
    func_08A65C08(self->list, 0);
    func_08A65B50(self->list, 0x1A);
    func_08A65AFC(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 2);
    func_08A65C64(self->list, D_08A7ABE8);
    func_08A65BAC(self->list, value);
    func_08A65AFC(self->list);
    return 0;
}
