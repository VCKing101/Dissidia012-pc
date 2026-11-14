/* Shows or hides every part listed in D_08B1CA40 (0xFFFF-terminated). */
struct State08B0785C {
    char pad_000[0x404];
    void *field_404[1];
};

struct Owner08B0785C {
    char pad_000[0x138];
    struct State08B0785C *field_138;
};

extern unsigned short D_08B1CA40[];
extern void func_088D9CD8(void *part);
extern void func_088D9CFC(void *part);

void func_08B0785C(struct Owner08B0785C *self, unsigned char show)
{
    unsigned short *id = D_08B1CA40;
    for (;;) {
        if (*id == 0xFFFF) {
            return;
        }
        if (show != 0) {
            func_088D9CD8(self->field_138->field_404[*id]);
        } else {
            func_088D9CFC(self->field_138->field_404[*id]);
        }
        id++;
    }
}
