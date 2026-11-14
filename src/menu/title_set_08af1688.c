/* Sets the page title (alternate text when func_08AF96B0 reports set) and redraws the page. */
typedef struct TitleText {
    short field_00;
    unsigned short field_02;
    char pad_04[0x4];
} TitleText;

typedef struct TitlePage {
    char pad_000[0x41C];
    char field_41C[0x48C];
    char field_8A8[0x2C4];
    char field_B6C[0x1C];
    int field_B88;
} TitlePage;

typedef struct TitleOwner {
    char pad_000[0x140];
    TitlePage *field_140;
} TitleOwner;

extern TitleText D_08B00AAA[];
extern TitleText D_08B00A3A[];
extern int func_08AF96B0(TitleOwner *self);
extern void func_088D3AE8(void *layout, int line, int text, int arg);
extern void func_088D44EC(void *layout, void *icons, void *script);

void func_08AF1688(TitleOwner *self)
{
    unsigned short text;
    TitlePage *page;
    if (func_08AF96B0(self) != 0) {
        text = D_08B00AAA[self->field_140->field_B88].field_02;
    } else {
        text = D_08B00A3A[self->field_140->field_B88].field_02;
    }
    func_088D3AE8(self->field_140->field_41C, 0x1F, text, 0x41);
    page = self->field_140;
    func_088D44EC(page->field_41C, page->field_8A8, page->field_B6C);
}
