/* Once the icon's flag 0x20 is set, opens notice 2 and moves to state 1. */
struct NoticeIcon {
    char pad_00[0x20];
    unsigned short field_20;

    static bool flagged(NoticeIcon *icon) { return (icon->field_20 & 0x20) != 0; }
};

struct NoticePanel {
    char pad_00[0x10];
    int field_10;
    char pad_14[0x20 - 0x14];
    NoticeIcon *field_20;
};

extern "C" void *D_08A4FFF4;
extern "C" char D_08A63830[];
extern "C" void func_08A4E2E4(void *dialog, int text, void *onYes, void *onNo, void *onCancel,
                              int sound, int a, int b, int c);

extern "C" {
void func_08A65128(NoticePanel *self)
{
    if (!NoticeIcon::flagged(self->field_20)) {
        return;
    }
    func_08A4E2E4(D_08A4FFF4, 2, 0, 0, D_08A63830, 0x9D, -1, -1, -1);
    self->field_10 = 1;
}
}
