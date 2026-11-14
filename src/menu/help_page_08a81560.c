/* Shows help page index on both help panels (skipped when it is already shown, unless forced). */
typedef struct HelpPanel {
    char pad_00[0x10];
    unsigned int field_10;
} HelpPanel;

typedef struct HelpState {
    char pad_000[0x59C];
    HelpPanel *field_59C;
    char pad_5A0[0x618 - 0x5A0];
    int field_618;
} HelpState;

typedef struct HelpOwner {
    char pad_00[0x28];
    HelpState *field_28;
} HelpOwner;

extern void func_08A80B54(HelpOwner *self, int panel);
extern void func_08A7F474(HelpOwner *self, int panel, int text, int arg);
extern unsigned short D_08A85BB4[];

void func_08A81560(HelpOwner *self, int page, int force)
{
    if (force == 0 && page == self->field_28->field_618) {
        return;
    }
    func_08A80B54(self, 2);
    func_08A80B54(self, 3);
    func_08A7F474(self, 2, D_08A85BB4[page], -1);
    func_08A7F474(self, 3, D_08A85BB4[page], -1);
    self->field_28->field_59C->field_10 |= 0x800;
    self->field_28->field_618 = page;
}
