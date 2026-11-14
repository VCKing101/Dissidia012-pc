typedef struct CommandList08A68BB0 {
    unsigned char active;
} CommandList08A68BB0;

typedef struct CommandOwner08A68BB0 {
    unsigned char active;
    char pad_001[0x6F];
    CommandList08A68BB0 *list;
} CommandOwner08A68BB0;

extern int func_08A65A84(CommandList08A68BB0 *list);
extern void func_08A65AAC(CommandList08A68BB0 *list);
extern void func_08A65AFC(CommandList08A68BB0 *list);
extern void func_08A65B50(CommandList08A68BB0 *list, int command);
extern void func_08A65BAC(CommandList08A68BB0 *list, int count);
extern void func_08A65C08(CommandList08A68BB0 *list, int value);

int func_08A68BB0(CommandOwner08A68BB0 *self)
{
    if (!self->active) {
        return -1;
    }
    func_08A65A84(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 0x31);
    func_08A65B50(self->list, 0x30);
    func_08A65BAC(self->list, 0xC);
    func_08A65AFC(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 1);
    func_08A65C08(self->list, 0xA);
    func_08A65BAC(self->list, 0xC);
    func_08A65AFC(self->list);
    return 0;
}
