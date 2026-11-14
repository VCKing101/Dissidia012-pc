/* Shows the two help labels when option 0x16 is available (text 0x95 or 0x96), hides them otherwise. */
typedef struct HelpLabel {
    char pad_00[0xE];
    short field_0E;
} HelpLabel;

typedef struct HelpState {
    char pad_000[0x514];
    HelpLabel *field_514;
    HelpLabel *field_518;
} HelpState;

typedef struct HelpOwner {
    char pad_000[0x14C];
    HelpState *field_14C;
} HelpOwner;

extern int func_089EFB50(HelpOwner *self, int entry, int index);
extern void func_088D9CFC(HelpLabel *label);
extern void func_088D9F5C(HelpLabel *label, int text, int redraw);
extern void func_088D9CD8(HelpLabel *label);

void func_089F8334(HelpOwner *self, unsigned char second)
{
    if (func_089EFB50(self, 0x16, -1) == 0) {
        func_088D9CFC(self->field_14C->field_514);
        func_088D9CFC(self->field_14C->field_518);
    } else {
        if (second) {
            func_088D9F5C(self->field_14C->field_514, 0x95, 1);
            self->field_14C->field_514->field_0E = 0x96;
        } else {
            func_088D9F5C(self->field_14C->field_514, 0x96, 1);
        }
        func_088D9CD8(self->field_14C->field_518);
    }
}
