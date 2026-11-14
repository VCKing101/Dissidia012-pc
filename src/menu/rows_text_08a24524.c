/* Builds the six row texts (name and value), then the footer, and refreshes the rows. */
typedef struct TextState {
    char field_000[0x400];
    char field_400[4];
    char pad_404[0x47C];
    char field_880[4];
    char pad_884[0x17A4];
    char field_2028[4];
    char pad_202C[0x60];
    unsigned short field_208C[6][16];
    unsigned short field_214C[6][20];
} TextState;

typedef struct TextOwner {
    char pad_0000[0x28F0];
    TextState *field_28F0;
} TextOwner;

extern void func_088D3F00(char *text, char *work, unsigned short *message);
extern void func_088D44EC(char *work, char *out, unsigned short *format);
extern void func_088D4B74(char *text, int flags);
extern void func_08A244A4(TextOwner *self);

void func_08A24524(TextOwner *self)
{
    int i;
    for (i = 0; i < 6; i++) {
        func_088D3F00(self->field_28F0->field_000, self->field_28F0->field_400, self->field_28F0->field_208C[i]);
        func_088D44EC(self->field_28F0->field_400, self->field_28F0->field_880, self->field_28F0->field_214C[i]);
    }
    func_088D4B74(self->field_28F0->field_2028, 0);
    func_08A244A4(self);
}
