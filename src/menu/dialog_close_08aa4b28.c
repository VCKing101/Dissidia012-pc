typedef struct DialogBox {
    char pad_00[0xE];
    short field_0E;
} DialogBox;

typedef struct DialogState {
    char pad_000[0xC];
    char *field_0C;
    char field_10[0xF8];
    DialogBox *field_108;
} DialogState;

typedef struct DialogOwner {
    char pad_0000[0x1EE4];
    DialogState *field_1EE4;
} DialogOwner;

extern char D_08AA8168[];
extern int func_08AA4800(DialogOwner *self);
extern void func_088D3CF0(char *dst, char *src, char *name);

void func_08AA4B28(DialogOwner *self)
{
    if (func_08AA4800(self) == 0) {
        return;
    }
    func_088D3CF0(self->field_1EE4->field_0C, self->field_1EE4->field_10, D_08AA8168);
    self->field_1EE4->field_108->field_0E = 4;
}
