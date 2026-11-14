/* Sends the help text for the cursor (from the slot tables in mode 2, else
   0x49 / 0x4E), unless it is 0xFFFF. */
struct State08B1EEC0 {
    char pad_0000[0x80A4];
    int field_80A4;
    char pad_80A8[0x80F0 - 0x80A8];
    unsigned int field_80F0;
    char pad_80F4[4];
    int field_80F8;
};

struct Owner08B1EEC0 {
    char pad_000[0x138];
    struct State08B1EEC0 *field_138;
};

extern void *D_08B01CF8;
extern unsigned short D_08B22E84[];
extern unsigned short D_08B22E8C[];
extern void func_08AF7134(void *queue, int text);

void func_08B1EEC0(struct Owner08B1EEC0 *self)
{
    unsigned short text;
    if ((self->field_138->field_80F0 & 2) != 0) {
        if (self->field_138->field_80F8 == 0) {
            text = D_08B22E8C[self->field_138->field_80A4];
        } else {
            text = D_08B22E84[self->field_138->field_80A4];
        }
    } else {
        text = (self->field_138->field_80F8 == 0) ? 0x4E : 0x49;
    }
    if (text != 0xFFFF) {
        func_08AF7134(D_08B01CF8, text);
    }
}
