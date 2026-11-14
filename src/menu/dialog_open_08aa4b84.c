typedef struct DialogState {
    char pad_000[0xC];
    char *field_0C;
    char field_10[4];
} DialogState;

typedef struct DialogOwner {
    char pad_0000[0x1EE4];
    DialogState *field_1EE4;
} DialogOwner;

extern char D_08AA8168[];
extern int func_08AA4800(DialogOwner *self);
extern void func_088D3F00(char *dst, char *src, char *name);
extern void func_08AA4A04(DialogOwner *self, int index);

void func_08AA4B84(DialogOwner *self)
{
    if (func_08AA4800(self) == 0) {
        return;
    }
    func_088D3F00(self->field_1EE4->field_0C, self->field_1EE4->field_10, D_08AA8168);
    func_08AA4A04(self, 0);
    func_08AA4A04(self, 1);
}
