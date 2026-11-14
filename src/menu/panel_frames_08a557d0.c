/* Refreshes the panel and sets its four icon frames from the frame table. */
typedef struct FrameEntry {
    unsigned short field_00;
    unsigned short field_02;
} FrameEntry;

typedef struct FramePanel {
    char pad_00[0x4];
    void *field_04;
    void *field_08;
    void *field_0C;
    void *field_10;
    void *field_14;
    char pad_18[0xFC - 0x18];
    int field_FC;
} FramePanel;

extern FrameEntry D_08A5863C[];
extern void func_088D9CFC(void *icon);
extern void func_088D9F5C(void *icon, int frame, int one);

void func_08A557D0(FramePanel *self)
{
    func_088D9CFC(self->field_14);
    func_088D9F5C(self->field_08, D_08A5863C[self->field_FC].field_00, 1);
    func_088D9F5C(self->field_0C, D_08A5863C[7].field_00, 1);
    func_088D9F5C(self->field_10, D_08A5863C[9].field_00, 1);
    func_088D9F5C(self->field_04, D_08A5863C[6].field_00, 1);
}
