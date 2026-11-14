/* Picks draw mode 10 or 11 from the selection lookup and redraws the page with it. */
typedef struct ModePage {
    char pad_000[0x400];
    char field_400[0x480];
    char field_880[0xED4 - 0x880];
    int field_ED4;
} ModePage;

typedef struct ModeOwner2 {
    char pad_000[0xF60];
    ModePage *field_F60;
} ModeOwner2;

extern void *D_08A23FF0;
extern int func_08A12F94(ModeOwner2 *self);
extern void func_08A22514(void *renderer, int mode, ModePage *page, void *layout, void *icons,
                          unsigned char flag, int selection, int zero);

void func_08A12244(ModeOwner2 *self, unsigned char flag)
{
    int selection = func_08A12F94(self);
    int mode = selection == 0 ? 10 : 11;
    ModePage *page;
    self->field_F60->field_ED4 = mode;
    page = self->field_F60;
    func_08A22514(D_08A23FF0, page->field_ED4, page, page->field_400, page->field_880, flag, selection, 0);
}
