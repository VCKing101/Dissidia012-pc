/* Sends the help text for the screen (0x1B, or 0x1A / 0x1C in mode 0x8168). */
struct State08B1DAE8 {
    char pad_0000[0x80F8];
    int field_80F8;
    char pad_80FC[0x8168 - 0x80FC];
    int field_8168;
};

struct Owner08B1DAE8 {
    char pad_000[0x138];
    struct State08B1DAE8 *field_138;
};

extern void *D_08B01CF8;
extern void func_08AF6E10(void *queue, int text);

void func_08B1DAE8(struct Owner08B1DAE8 *self)
{
    int text = 0x1B;
    if (self->field_138->field_8168 != 0) {
        text = (self->field_138->field_80F8 == 0) ? 0x1A : 0x1C;
    }
    func_08AF6E10(D_08B01CF8, text);
}
