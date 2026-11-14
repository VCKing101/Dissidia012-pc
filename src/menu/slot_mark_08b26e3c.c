/* Shows the slot's mark icon from its flags (0x1000 first icon, 0x2000
   second) for mode 0 or 1; hides it otherwise. */
struct Flags08B26E3C {
    unsigned short field_0;
    unsigned short field_2;
};

struct State08B26E3C {
    char pad_000[0xD8];
    void *field_D8[(0x29C - 0xD8) / 4];
    struct Flags08B26E3C field_29C[4];
    struct Flags08B26E3C field_2AC[4];
};

struct Owner08B26E3C {
    struct State08B26E3C *field_0;
};

extern unsigned short D_08B27F4C[2];
extern void func_088D9F5C(void *part, int id, int show);
extern void func_088D9CFC(void *part);

void func_08B26E3C(struct Owner08B26E3C *self, int mode, int index)
{
    void *part = self->field_0->field_D8[index];
    unsigned short flags;
    if (mode == 0) {
        flags = self->field_0->field_2AC[index].field_2;
    } else {
        flags = (mode == 1) ? self->field_0->field_29C[index].field_2 : 0;
    }
    if ((flags & 0x1000) != 0) {
        func_088D9F5C(part, D_08B27F4C[0], 1);
    } else if ((flags & 0x2000) != 0) {
        func_088D9F5C(part, D_08B27F4C[1], 1);
    } else {
        func_088D9CFC(part);
    }
}
