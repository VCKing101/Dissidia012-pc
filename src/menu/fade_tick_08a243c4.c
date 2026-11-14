/* Counts the page's delay down (finishing it at zero) and redraws both layers. */
typedef struct TickPage {
    char pad_0000[0x400];
    char field_400[0x480];
    char field_880[0x25A0 - 0x880];
    int field_25A0;
} TickPage;

typedef struct TickOwner {
    char pad_0000[0x28F0];
    TickPage *field_28F0;
} TickOwner;

extern char D_08A28828[];
extern char D_08A28838[];
extern void func_08A25A90(TickOwner *self);
extern void func_088D44EC(void *layout, void *icons, void *script);

void func_08A243C4(TickOwner *self)
{
    TickPage *page = self->field_28F0;
    if (page->field_25A0 != 0) {
        page->field_25A0 = page->field_25A0 - 1;
        page = self->field_28F0;
        if (page->field_25A0 <= 0) {
            page->field_25A0 = 0;
            func_08A25A90(self);
            page = self->field_28F0;
        }
    }
    func_088D44EC(page->field_400, page->field_880, D_08A28828);
    func_088D44EC(self->field_28F0->field_400, self->field_28F0->field_880, D_08A28838);
}
