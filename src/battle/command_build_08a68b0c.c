typedef struct CommandList08A68B0C {
    unsigned char active;
} CommandList08A68B0C;

typedef struct CommandOwner08A68B0C {
    unsigned char active;
    char pad_001[0x6F];
    CommandList08A68B0C *list;
} CommandOwner08A68B0C;

extern int func_08A65A84(CommandList08A68B0C *list);
extern void func_08A65AAC(CommandList08A68B0C *list);
extern void func_08A65AFC(CommandList08A68B0C *list);
extern void func_08A65B50(CommandList08A68B0C *list, int command);
extern void func_08A65BAC(CommandList08A68B0C *list, int count);
extern void func_08A65C08(CommandList08A68B0C *list, int value);

int func_08A68B0C(CommandOwner08A68B0C *self)
{
    if (!self->active) {
        return -1;
    }
    func_08A65A84(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 0x31);
    func_08A65B50(self->list, 0x2F);
    func_08A65BAC(self->list, 0xC);
    func_08A65AFC(self->list);
    func_08A65AAC(self->list);
    func_08A65B50(self->list, 1);
    func_08A65C08(self->list, 9);
    func_08A65BAC(self->list, 0xC);
    func_08A65AFC(self->list);
    return 0;
}
