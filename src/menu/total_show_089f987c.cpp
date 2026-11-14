/* Shows the total: help text, label position, scale 1, grey colour, number text; continues once the label is shown. */
struct TotalLabel {
    char pad_00[0x20];
    unsigned short field_20;
};

struct TotalState {
    char pad_000[0x88];
    char field_88[0x400];
    char field_488[0x438];
    TotalLabel *field_8C0;
    char pad_8C4[0x1C8];
    char field_A8C[0x4];
    char pad_A90[0x3238 - 0xA90];
    unsigned int field_3238;
    char pad_323C[0xC];
    int field_3248;
    int field_324C;
};

struct TotalOwner {
    char pad_000[0x14C];
    TotalState *field_14C;
    static bool hasFlag(unsigned int flags, unsigned int mask) { return (flags & mask) != 0; }
};

extern "C" {
void func_088D3F00(char *text, char *work, unsigned short *message);
void func_088D9C60(TotalLabel *label, int x, int y);
void func_088D9C8C(TotalLabel *label, float scale_x, float scale_y);
void func_088D9B48(TotalLabel *label, unsigned int color);
void func_088D44EC(char *work, char *out, unsigned short *format);
void func_089F9954(TotalOwner *self);
extern unsigned short D_08A0A59A[];
extern unsigned short D_08A0A4D2[];

void func_089F987C(TotalOwner *self)
{
    if ((self->field_14C->field_3238 & 0x80) == 0) {
        return;
    }
    func_088D3F00(self->field_14C->field_88, self->field_14C->field_488, D_08A0A59A);
    func_088D9C60(self->field_14C->field_8C0, self->field_14C->field_3248, self->field_14C->field_324C);
    func_088D9C8C(self->field_14C->field_8C0, 1.0f, 1.0f);
    func_088D9B48(self->field_14C->field_8C0, 0xFF808080);
    func_088D44EC(self->field_14C->field_488, self->field_14C->field_A8C, D_08A0A4D2);
    if (!TotalOwner::hasFlag(self->field_14C->field_8C0->field_20, 0x20)) {
        return;
    }
    func_089F9954(self);
}
}
