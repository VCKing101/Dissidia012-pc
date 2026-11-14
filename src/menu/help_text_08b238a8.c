/* Sends the help text for the current mode, unless it is 0xFFFF. */
struct State08B238A8 {
    char pad_000[0x9B0];
    int field_9B0;
};

struct Owner08B238A8 {
    char pad_000[0x138];
    struct State08B238A8 *field_138;
};

extern void *D_08B01CF8;
extern unsigned short D_08B243B2[];
extern void func_08AF7134(void *queue, int text);

void func_08B238A8(struct Owner08B238A8 *self)
{
    unsigned short text = D_08B243B2[self->field_138->field_9B0];
    if (text != 0xFFFF) {
        func_08AF7134(D_08B01CF8, text);
    }
}
