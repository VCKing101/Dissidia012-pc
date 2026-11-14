/* Sets the page title for the current page (alternate text when the page is marked). */
typedef struct PageTitleState {
    char pad_000[0xB88];
    int field_B88;
    char pad_B8C[0xBB0 - 0xB8C];
    unsigned char field_BB0[16];
} PageTitleState;

typedef struct PageTitleOwner {
    char pad_000[0x140];
    PageTitleState *field_140;
} PageTitleOwner;

extern unsigned short D_08B01C58[];
extern unsigned short D_08B01C78[];
extern void func_08AF7134(PageTitleOwner *self, int text);

void func_08AF7190(PageTitleOwner *self)
{
    int page = self->field_140->field_B88;
    unsigned short text;
    if (self->field_140->field_BB0[page] != 0) {
        text = D_08B01C58[page];
    } else {
        text = D_08B01C78[page];
    }
    func_08AF7134(self, text);
}
